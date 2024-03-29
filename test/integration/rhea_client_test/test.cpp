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
        int wc = GetIntOpt("-wc");
        int rc = GetIntOpt("-rc");
//        if(rc > wc) {
//            std::cout << "The number of reads must be less than the number of writes" << std::endl;
//            exit(EXIT_FAILURE);
//        }
    }

public:
    void Usage(void) {
        std::cout << "Usage: ./test -[param-id] [value] ... " << std::endl;
        std::cout << std::endl;
        std::cout << "-c [int]: The number of clients that will write to the same file. Default is 1." << std::endl;
        std::cout << "-bs [size]: Block size of I/O requests. Default is 4KB." << std::endl;
        std::cout << "-wc [int]: The number of write requests to make. Default is 1." << std::endl;
        std::cout << "-rc [int]: The number of read requests to make. Default is 0." << std::endl;
        std::cout << "-base [string]: The directory that will be used to store temp data. Default /tmp." << std::endl;
        std::cout << "-conf [string]: The config file for Rhea. Default is no config." << std::endl;
    }

    TestArgs(int argc, char **argv) {
        AddOpt("-c", common::args::ArgType::kInt, 1);
        AddOpt("-bs", common::args::ArgType::kSize, 4 * (1ul << 10));
        AddOpt("-rc", common::args::ArgType::kInt, 1);
        AddOpt("-wc", common::args::ArgType::kInt, 1);
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
    int rc = args.GetIntOpt("-rc");
    int wc = args.GetIntOpt("-wc");
    int clients_per_file = args.GetIntOpt("-c");
    size_t bs = args.GetSizeOpt("-bs");
    int sink_group = rank / clients_per_file;
    int sink_member = rank % clients_per_file;
    std::string base = args.GetStringOpt("-base") + "/test" + std::to_string(sink_group) + ".txt";

    auto conf = args.GetStringOpt("-conf");
    //Set configuration files
    COMMON_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    SENTINEL_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->LoadConfiguration();

    //Allocate block
    char *data = (char *) malloc(bs);
    memset(data, 'a', bs);
    char *data_save = (char *) malloc(bs);
    memset(data_save, 'a', bs);

    //Write to file
    if(wc > 0){
        rhea::Client write_client(0);
        auto write_parcels = std::vector<Parcel>();
        auto writer_timer = Timer();
        writer_timer.resumeTime();
        for (int i = 0; i < wc; ++i) {
            Parcel parcel;
            parcel.id_ = base;
            parcel.storage_index_ = 0;
            parcel.type_ = OperationType::PUBLISH;
            parcel.position_ = sink_member * bs * wc + i * bs;
            parcel.data_size_ = bs;
            write_client.Publish(parcel, data);
            write_parcels.push_back(parcel);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        writer_timer.pauseTime();
        auto write_async_time = writer_timer.getElapsedTime();
        writer_timer.resumeTime();
        for (int i = 0; i < wc; ++i) {
            if (BASKET_CONF->MPI_RANK == 0)
                printProgress((double(i + 1)) / wc, "write");
            write_client.Wait(write_parcels[i]);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        writer_timer.pauseTime();
        if (BASKET_CONF->MPI_RANK == 0) {
            printf("\nWrite Rate %f, Async Write Time %f, Sync Write time %f\n", (wc*comm_size*1000.0), write_async_time, writer_timer.getElapsedTime());
        }
        write_client.FinalizeClient();
        sleep(5);
    }
    if (rc > 0) {
        //Read from file
        rhea::Client read_client(1);
        auto read_parcels = std::vector<Parcel>();
        auto read_timer = Timer();
        read_timer.resumeTime();
        for (int i = 0; i < rc; ++i) {
            Parcel parcel;
            parcel.id_ = base;
            parcel.storage_index_ = 0;
            parcel.type_ = OperationType::SUBSCRIBE;
            parcel.position_ = sink_member * bs * wc + i * bs;
            parcel.data_size_ = bs;
            read_client.AsyncSubscribe(parcel);
            read_parcels.push_back(parcel);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        read_timer.pauseTime();
        auto read_async_time = read_timer.getElapsedTime();
        read_timer.resumeTime();
        memset(data, 0, bs);
        for (int i = 0; i < rc; ++i) {
            if (BASKET_CONF->MPI_RANK == 0)
                printProgress((double(i + 1)) / wc, "read");
            read_client.Wait(read_parcels[i]);
            read_client.GetSubscribedData(read_parcels[i], data);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        read_timer.pauseTime();
        if (BASKET_CONF->MPI_RANK == 0) {
            printf("\nAsync Read Time %f, Sync Read time %f\n", read_async_time, read_timer.getElapsedTime());
        }
        read_client.FinalizeClient();
    }

    //Finalize
    MPI_Finalize();
}
