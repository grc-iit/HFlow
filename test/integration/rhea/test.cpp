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
        if(rc > wc) {
            std::cout << "The number of reads must be less than the number of writes" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

public:
    void Usage(void) {
        std::cout << "Usage: ./test -[param-id] [value] ... " << std::endl;
        std::cout << std::endl;
        std::cout << "-c [int]: The number of clients that will write to the same file. Default is 1." << std::endl;
        std::cout << "-bs [size]: Block size of I/O requests. Default is 4KB." << std::endl;
        std::cout << "-wc [int]: The number of write requests to make. Default is 1." << std::endl;
        std::cout << "-rc [int]: The number of read requests to make. Default is 1." << std::endl;
        std::cout << "-base [string]: The directory that will be used to store temp data. Default /tmp." << std::endl;
        std::cout << "-conf [string]: The config file for Rhea. Default is no config." << std::endl;
    }

    TestArgs(int argc, char **argv) {
        AddOpt("-c", common::args::ArgType::kInt, 1);
        AddOpt("-bs", common::args::ArgType::kSize, 4*(1ul<<10));
        AddOpt("-rc", common::args::ArgType::kInt, 1);
        AddOpt("-wc", common::args::ArgType::kInt, 1);
        AddOpt("-base", common::args::ArgType::kString, "/tmp");
        AddOpt("-config", common::args::ArgType::kString, "");
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
    int rc = args.GetIntOpt("-rc");
    int wc = args.GetIntOpt("-wc");
    int clients_per_file = args.GetIntOpt("-c");
    size_t bs = args.GetSizeOpt("-bs");
    int sink_group = rank/clients_per_file;
    int sink_member = rank%clients_per_file;
    std::string base = args.GetStringOpt("-base") + "/test" + std::to_string(sink_group) + ".txt";

    //Allocate block
    char *data = (char*)malloc(bs);
    memset(data, 'a', bs);
    char *data_save = (char*)malloc(bs);
    memset(data_save, 'a', bs);

    //Write to file
    rhea::Client write_client(0);
    for(int i = 0; i < wc; ++i) {
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::PUBLISH;
        parcel.position_ = sink_member*bs;
        parcel.data_size_ = bs;
        parcel.buffer_ = data;
        write_client.Publish(parcel, data);
    }

    //Read from file
    rhea::Client read_client(1);
    for(int i = 0; i < rc; ++i) {
        Parcel parcel;
        parcel.id_ = base;
        parcel.storage_index_ = 0;
        parcel.type_ = OperationType::SUBSCRIBE;
        parcel.position_ = sink_member*bs;
        parcel.data_size_ = bs;
        parcel.buffer_ = data;
        memset(data, 0, bs);
        read_client.Subscribe(parcel, data);
        if (strncmp(data, data_save, bs) != 0) {
            exit(EXIT_FAILURE);
        }
    }

    //Finalize
    read_client.FinalizeClient();
    write_client.FinalizeClient();
    MPI_Finalize();
}
