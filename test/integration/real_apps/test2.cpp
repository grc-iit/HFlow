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
    JobId job_id;
    if(mode == 0){
        job_id = 2;
    }else job_id = 0;
    rhea::Client write_client(job_id,true, rank);
    int count = 32;
    auto parcels = std::vector<Parcel>();
    auto p1_timer = Timer();
    p1_timer.resumeTime();
    int compute_us = 0;
    for(int request_id=0;request_id < count;++request_id){
        usleep(compute_us);
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::PUBLISH;
        parcel.position_ = sink_member * bs * count + request_id * bs;
        parcel.data_size_ = bs;
        write_client.Publish(parcel, data);
        parcels.push_back(parcel);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    p1_timer.pauseTime();
    auto p1_async_time = p1_timer.getElapsedTime();
    p1_timer.resumeTime();
    write_client.WaitAll(parcels);
    MPI_Barrier(MPI_COMM_WORLD);
    p1_timer.pauseTime();
    if (rank == 0) {
        printf("\nP1, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, p1_async_time-compute_us*count/1000.0, p1_timer.getElapsedTime()-compute_us*count/1000.0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    //P2
    compute_us = 1000;
    auto p2_timer = Timer();
    p2_timer.resumeTime();
    for(int request_id=0;request_id < count;++request_id){
        usleep(compute_us);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    p2_timer.pauseTime();
    auto p2_async_time = p2_timer.getElapsedTime();
    if (rank == 0) {
        printf("\nP2, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, p2_async_time-compute_us*count/1000.0, p2_timer.getElapsedTime()-compute_us*count/1000.0);
    }
    //P3
    count = 4;
    parcels = std::vector<Parcel>();
    auto p3_timer = Timer();
    p3_timer.resumeTime();
    for(int request_id=0;request_id < count;++request_id){
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::PUBLISH;
        parcel.position_ = sink_member * bs * count + request_id * bs;
        parcel.data_size_ = bs;
        write_client.Publish(parcel, data);
        parcels.push_back(parcel);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    p3_timer.pauseTime();
    auto p3_async_time = p3_timer.getElapsedTime();
    p3_timer.resumeTime();
    write_client.WaitAll(parcels);
    MPI_Barrier(MPI_COMM_WORLD);
    p3_timer.pauseTime();
    if (rank == 0) {
        printf("\nP3, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, p3_async_time-compute_us*count/1000.0, p3_timer.getElapsedTime()-compute_us*count/1000.0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    write_client.FinalizeClient();
    MPI_Finalize();
}
