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
    int rank_per_node = args.GetIntOpt("-r");
    int num_nodes = comm_size/rank_per_node;
    size_t bs = args.GetSizeOpt("-bs");
    int sink_group = rank / 1;
    int sink_member = rank % 1;
    std::string base = args.GetStringOpt("-base") + "/test" + std::to_string(sink_group) + ".txt";
    int num_procs_per_app = ceil(comm_size/3.0);
    auto is_app1 = rank < num_procs_per_app;
    auto is_app2 = rank >= num_procs_per_app && rank < 2*num_procs_per_app;
    auto is_app3 = rank >= 2*num_procs_per_app;

    COMMON_DBGVAR5(d1,rank,is_app1,is_app2,is_app3,num_nodes);
    MPI_Comm app1_comm,app2_comm,app3_comm;
    MPI_Comm_split(MPI_COMM_WORLD, !is_app1, rank, &app1_comm);
    MPI_Comm_split(MPI_COMM_WORLD, !is_app2, rank, &app2_comm);
    MPI_Comm_split(MPI_COMM_WORLD, !is_app3, rank, &app3_comm);
    int app1_comm_size,app2_comm_size,app3_comm_size;
    MPI_Comm_size(app1_comm, &app1_comm_size);
    MPI_Comm_size(app2_comm, &app2_comm_size);
    MPI_Comm_size(app3_comm, &app3_comm_size);
    int app1_rank,app2_rank,app3_rank;
    MPI_Comm_rank(app1_comm, &app1_rank);
    MPI_Comm_rank(app2_comm, &app2_rank);
    MPI_Comm_rank(app3_comm, &app3_rank);

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

    uint32_t app1_queues = (RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT*2.0/3.0);
    uint32_t app2_queues = floor((RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT*1.0/3.0)*2.0/3.0);
    uint32_t app3_queues = RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT - app1_queues - app2_queues;
    uint32_t start_app1_queue = 0, end_app1_queue=app1_queues-1;
    uint32_t start_app2_queue = app1_queues, end_app2_queue=start_app2_queue + app2_queues - 1;
    uint32_t start_app3_queue = end_app2_queue + 1, end_app3_queue=start_app3_queue + app3_queues - 1;
    if(rank == 0){
        COMMON_DBGVAR3(a1,app1_queues,start_app1_queue,end_app1_queue);
        COMMON_DBGVAR3(a2,app2_queues,start_app2_queue,end_app2_queue);
        COMMON_DBGVAR3(a3,app3_queues,start_app3_queue,end_app3_queue);
    }


    if(is_app1){
        // App 1: simulation + I/O (I/O intensive) like VPIC
        JobId job_id;
        if(mode == 0){
            job_id = 2;
        }else job_id = 0;
        rhea::Client write_client(job_id,true, rank);
        if(mode == 0){
            job_id = 2;
        }else{
            job_id = 0;
            BASKET_CONF->MY_SERVER = rank % app1_queues + start_app1_queue;
        }
        COMMON_DBGVAR3(d4,rank,SENTINEL_CONF->COLLECTORS_PER_SOURCE,BASKET_CONF->MY_SERVER);
        int count = 32;
        auto parcels = std::vector<Parcel>();
        auto timer = Timer();
        timer.resumeTime();
        int compute_us = 100;
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
        MPI_Barrier(app1_comm);
        timer.pauseTime();
        auto async_time = timer.getElapsedTime();
        timer.resumeTime();
        write_client.WaitAll(parcels);
        MPI_Barrier(app1_comm);
        timer.pauseTime();
        if (app1_rank == 0) {
            printf("\nApp1, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, async_time, timer.getElapsedTime());
        }
        MPI_Barrier(MPI_COMM_WORLD);
        write_client.FinalizeClient();
    }else if(is_app2){
        // App 2: simulation + Checkpoint (balanced) like HACC with periodic checkpointing not too frequent
        JobId job_id;
        if(mode == 0){
            job_id = 2;
        }else job_id = 0;
        rhea::Client write_client(job_id,true, rank);
        if(mode == 0){
            job_id = 2;
        }else{
            job_id = 0;
            BASKET_CONF->MY_SERVER = rank % app2_queues + start_app2_queue;
        }
        COMMON_DBGVAR3(d4,rank,SENTINEL_CONF->COLLECTORS_PER_SOURCE,BASKET_CONF->MY_SERVER);
        int count = 16;
        auto parcels = std::vector<Parcel>();
        auto timer = Timer();
        timer.resumeTime();
        int compute_us = 1000;
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
        MPI_Barrier(app2_comm);
        timer.pauseTime();
        auto async_time = timer.getElapsedTime();
        timer.resumeTime();
        write_client.WaitAll(parcels);
        MPI_Barrier(app2_comm);
        timer.pauseTime();
        if (app2_rank == 0) {
            printf("\nApp2, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, async_time, timer.getElapsedTime());
        }
        MPI_Barrier(MPI_COMM_WORLD);
        write_client.FinalizeClient();
    }else{
        // App 3: Clustering Algorithm + write cluster (compute more) like Kmeans with in-memory data and writing only cluster points.
        JobId job_id;
        int num_queues = RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT;
        if(mode == 0){
            job_id = 2;
        }else job_id = 0;
        rhea::Client write_client(job_id,true, rank);
        if(mode == 0){
            job_id = 2;
        }else{
            job_id = 0;
            BASKET_CONF->MY_SERVER = rank % app3_queues + start_app3_queue;
        }
        COMMON_DBGVAR3(d4,rank,SENTINEL_CONF->COLLECTORS_PER_SOURCE,BASKET_CONF->MY_SERVER);

        int count = 8;
        auto parcels = std::vector<Parcel>();
        auto timer = Timer();
        timer.resumeTime();
        int compute_us = 10000;
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
        MPI_Barrier(app3_comm);
        timer.pauseTime();
        auto async_time = timer.getElapsedTime();
        timer.resumeTime();
        write_client.WaitAll(parcels);
        MPI_Barrier(app3_comm);
        timer.pauseTime();
        if (app3_rank == 0) {
            printf("\nApp3, Compute %f, Async Write Time %f, Sync Write time %f\n", compute_us*count/1000.0, async_time, timer.getElapsedTime());
        }
        MPI_Barrier(MPI_COMM_WORLD);
        write_client.FinalizeClient();
    }
    //Finalize
    MPI_Finalize();
}
