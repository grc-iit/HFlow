//
// Created by yejie on 9/23/20.
//

#include <mpi.h>
#include <rhea/byte_flow_regulator/server.h>
#include <basket/common/singleton.h>
#include <sentinel/worker_manager/client.h>
#include <common/arguments.h>

class TestArgs : public common::args::ArgMap {
private:
    void VerifyArgs(void) {
    }

public:
    void Usage(void) {
        std::cout << "Usage: ./test -[param-id] [value] ... " << std::endl;
        std::cout << std::endl;
        std::cout << "-c [int]: The number of clients that make requests. Default is 1." << std::endl;
        std::cout << "-bs [size]: Block size of I/O requests. Default is 4KB." << std::endl;
        std::cout << "-n [int]: The number of decisions to make. Default is 1." << std::endl;
        std::cout << "-conf [string]: The config file for Rhea. Default is no config." << std::endl;
    }

    TestArgs(int argc, char **argv) {
        AddOpt("-c", common::args::ArgType::kInt, 1);
        AddOpt("-bs", common::args::ArgType::kSize, 4*(1ul<<10));
        AddOpt("-n", common::args::ArgType::kInt, 1);
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
    int count = args.GetIntOpt("-n");
    int clients = args.GetIntOpt("-c");
    size_t bs = args.GetSizeOpt("-bs");

    //Set configuration files
    COMMON_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->LoadConfiguration();

    rhea::byteflow_regulator::Server server;
    JobId job=0;
    if(BASKET_CONF->MPI_RANK == 0)
        server.RegisterJob(job);
    MPI_Barrier(MPI_COMM_WORLD);
    auto decision_rate = Timer();
    decision_rate.resumeTime();
    for(int j = 0; j < count; ++j){
        server.CalculateRate();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    decision_rate.pauseTime();
    if(BASKET_CONF->MPI_RANK == 0)
        printf("Decision time %f\n",decision_rate.getElapsedTime());
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
        server.FinalizeJob(job);
    MPI_Finalize();
}