//
// Created by lukemartinlogan on 9/22/20.
//

#include <mpi.h>
#include <rhea/rhea.h>
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
        std::cout << "-bs [size]: Block size of I/O requests. Default is 4KB." << std::endl;
        std::cout << "-m [int]: mode to decide which job to run. [0-> IOF 1->Rhea]" << std::endl;
        std::cout << "-r [int]: ranks per node" << std::endl;
        std::cout << "-base [string]: The directory that will be used to store temp data. Default /tmp." << std::endl;
        std::cout << "-conf [string]: The config file for App1. Default is no config." << std::endl;
    }

    TestArgs(int argc, char **argv) {
        AddOpt("-m", common::args::ArgType::kInt, 0);
        AddOpt("-r", common::args::ArgType::kInt, 1);
        AddOpt("-bs", common::args::ArgType::kSize, 4 * (1ul << 10));
        AddOpt("-base", common::args::ArgType::kString, "/tmp");
        AddOpt("-conf", common::args::ArgType::kString, "");
        ArgIter(argc, argv);
        VerifyArgs();
    }
};

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage, char *data = "") {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%s %3d%% [%.*s%*s]", data, val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank,comm_size;

    //Get the number of clients
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    //Get arguments
    TestArgs args(argc, argv);
    int mode = args.GetIntOpt("-m");
    size_t bs = args.GetSizeOpt("-bs");
    int sink_group = rank / 1;
    int sink_member = rank % 1;
    std::string base = args.GetStringOpt("-base") + "/test" + std::to_string(sink_group) + ".txt";
    int num_procs_per_app = ceil(comm_size/3.0);
    //Allocate block
    char *data = (char *) malloc(bs);
    memset(data, 'a', bs);
    char *data_save = (char *) malloc(bs);
    memset(data_save, 'a', bs);
    auto conf = args.GetStringOpt("-conf");
    //Set configuration files
    if(conf != ""){
        COMMON_CONF->CONFIGURATION_FILE = conf;
        SENTINEL_CONF->CONFIGURATION_FILE = conf;
        RHEA_CONF->CONFIGURATION_FILE = conf;
    }
    RHEA_CONF->LoadConfiguration();
    int count = 1024;
    JobId job_id = 0;
    rhea::Client write_client(job_id,true, rank);

    for(int request_id=0;request_id < count;++request_id){
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::PUBLISH;
        parcel.position_ = sink_member * bs * count + request_id * bs;
        parcel.data_size_ = bs;
        write_client.Publish(parcel, data);
        write_client.Wait(parcel);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    write_client.FinalizeClient();
    job_id = 1;
    rhea::Client read_client(job_id,true, rank);
    double percentage = .1;
    if(mode == 0) percentage = 1.0;
    auto rest_parcels = std::vector<Parcel>();
    auto partial_parcels = std::vector<Parcel>();
    auto p1_timer = Timer();
    p1_timer.resumeTime();
    for(int request_id=0;request_id < count;++request_id){
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::SUBSCRIBE;
        parcel.position_ = sink_member * bs * count + request_id * bs;
        parcel.data_size_ = bs;
        read_client.AsyncSubscribe(parcel);
        if(request_id < double(count) * percentage)
            partial_parcels.push_back(parcel);
        else rest_parcels.push_back(parcel);
    }

    read_client.WaitAll(partial_parcels);

    int j=0;
    int compute_us = 10000;
    for(int i=0;i<count;++i) {
        usleep(compute_us);
        if(i > double(count) * percentage){
            read_client.Wait(rest_parcels[j]);
            read_client.GetSubscribedData(rest_parcels[j],data);
            j++;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    p1_timer.pauseTime();
    auto time = p1_timer.getElapsedTime();
    if (rank == 0) {
        printf("\nTotal Time %f, Compute time %f ms\n", time,compute_us*count/1000.0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    read_client.FinalizeClient();
    MPI_Finalize();
}
