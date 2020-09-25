//
// Created by yejie on 9/23/20.
//

#include <mpi.h>
#include <rhea/byte_flow_regulator/client.h>
#include <basket/common/singleton.h>
#include <sentinel/worker_manager/client.h>
#include <common/arguments.h>

class TestArgs : public common::args::ArgMap {
private:
    void VerifyArgs(void) {
        int in_r = GetIntOpt("-ir");
        int out_r = GetIntOpt("-or");
        if(in_r <= 0 || out_r <= 0) {
            std::cout << "The number of reads must be less than the number of writes" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

public:
    void Usage(void) {
        std::cout << "Usage: ./test -[param-id] [value] ... " << std::endl;
        std::cout << std::endl;
        std::cout << "-c [int]: The number of clients that make requests. Default is 1." << std::endl;
        std::cout << "-bs [size]: Block size of I/O requests. Default is 4KB." << std::endl;
        std::cout << "-ir [int]: The number of in requests to make. Default is 1." << std::endl;
        std::cout << "-or [int]: The number of out requests to make. Default is 1." << std::endl;
        std::cout << "-conf [string]: The config file for Rhea. Default is no config." << std::endl;
    }

    TestArgs(int argc, char **argv) {
        AddOpt("-c", common::args::ArgType::kInt, 1);
        AddOpt("-bs", common::args::ArgType::kSize, 4*(1ul<<10));
        AddOpt("-ir", common::args::ArgType::kInt, 1);
        AddOpt("-or", common::args::ArgType::kInt, 1);
        AddOpt("-conf", common::args::ArgType::kString, "");
        ArgIter(argc, argv);
        VerifyArgs();
    }
};

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    int rank;

    //Get the number of clients
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Get arguments
    TestArgs args(argc, argv);
    int in_r = args.GetIntOpt("-ir");
    int out_r = args.GetIntOpt("-or");
    int clients = args.GetIntOpt("-c");
    size_t bs = args.GetSizeOpt("-bs");

    //Set configuration files
    COMMON_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->LoadConfiguration();

    rhea::byteflow_regulator::Client client;
    JobId job=0;
    if(BASKET_CONF->MPI_RANK == 0)
        client.RegisterJob(job);
    MPI_Barrier(MPI_COMM_WORLD);
    auto in_rate_timer = Timer();
    in_rate_timer.resumeTime();
    for(int j = 0; j < in_r; ++j){
        client.SetInRate(job, bs);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    in_rate_timer.pauseTime();
    if(BASKET_CONF->MPI_RANK == 0)
        printf("In rate time %f\n",in_rate_timer.getElapsedTime());
    auto out_rate_timer = Timer();
    out_rate_timer.resumeTime();
    for(int j = 0; j < out_r; ++j){
        client.SetOutRate( job, bs);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    out_rate_timer.pauseTime();
    if(BASKET_CONF->MPI_RANK == 0)
        printf("Out rate time %f\n",out_rate_timer.getElapsedTime());
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
        client.Finalize(job);
    MPI_Finalize();
}