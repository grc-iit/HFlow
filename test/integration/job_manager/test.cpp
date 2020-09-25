//
// Created by yejie on 9/23/20.
//

#include <mpi.h>
#include <sentinel/job_manager/Server.h>
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

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage,char* data="") {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%s %3d%% [%.*s%*s]",data, val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

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
    SENTINEL_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    SENTINEL_CONF->LoadConfiguration();

    sentinel::job_manager::Server server;
    JobId jobId=0;
    MPI_Barrier(MPI_COMM_WORLD);
    auto spawn_time = Timer();
    auto kill_time = Timer();
    for(int j = 0; j < count; ++j){
        ResourceAllocation defaultResourceAllocation = SENTINEL_CONF->DEFAULT_RESOURCE_ALLOCATION;
        defaultResourceAllocation.job_id_ = jobId;
        auto vect = std::vector<WorkerManagerResource>();
        server.used_resources.insert(std::make_pair(jobId, vect));
        auto threads = defaultResourceAllocation.num_nodes_ * defaultResourceAllocation.num_procs_per_node * defaultResourceAllocation.num_threads_per_proc;
        spawn_time.resumeTime();
        server.SpawnWorkerManagers(threads, jobId);
        spawn_time.pauseTime();
        sleep(1);
        kill_time.resumeTime();
        server.TerminateJob(jobId);
        kill_time.pauseTime();
        sleep(1);
        printProgress((double (j+1))/count,"job_test");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
        printf("\nSpawn Time %f, Kill Time %f\n",spawn_time.getElapsedTime(),kill_time.getElapsedTime());
    MPI_Finalize();
}