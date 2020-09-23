//
// Created by hdevarajan on 9/19/20.
//

#include <mpi.h>
#include <rhea/rhea.h>
#include <basket/common/singleton.h>
#include <sentinel/worker_manager/client.h>
#include <common/enumerations.h>
//#include <common/class_loader.h>

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

int main(int argc, char * argv[]){
    MPI_Init(&argc,&argv);

    std::vector<uint32_t> request_number = {4};
    std::vector<uint32_t> request_size = {4};

    uint payload_size=0;
    for(int size : request_size) { for (int number : request_number) {payload_size+=(size*number);}}
    std::string payload = random_string(payload_size);

    std::cout << payload << std::endl;

    uint32_t jobId = 0;
    rhea::Client write_client(jobId);
//    sleep(5);
    Parcel write_parcel;
    write_parcel.storage_index_=0;
    write_parcel.type_=OperationType::PUBLISH;
    for(int size : request_size) {
        write_parcel.data_size_=size;
        for (int number : request_number) {
            int offset=0;
            write_parcel.id_= "/home/jaime/projects/rhea/test/test_results/test_" + std::to_string(size) + "_" + std::to_string(number);
            for(int i=0; i < number; i++){
                write_parcel.position_=offset;
                auto current_payload = payload.substr(offset, size);
//                write_parcel.buffer_=current_payload.data();
                char* rhea_data = current_payload.data();
                write_client.Publish(write_parcel, rhea_data);
                offset+=size;
//                sleep(5);
            }
        }
    }

//    for(int size : request_size) {
//        write_parcel.data_size_=size;
//        for (int number : request_number) {
//            int offset=0;
//            write_parcel.id_= "/home/jaime/projects/rhea/test/test_results/test_" + std::to_string(size) + "_" + std::to_string(number);
//            for(int i=0; i < number; i++){
//                write_parcel.position_=offset;
//                auto rhea_data = write_client.GetData(write_parcel);
//                offset+=size;
//                std::cout << write_parcel.position_ << ": " << rhea_data << std::endl;
//            }
//        }
//    }

//    uint32_t read_jobId = 1;
//    rhea::Client read_client(read_jobId);
//
//    std::string results;
//    Parcel read_parcel;
//    read_parcel.storage_index_=0;
//    read_parcel.type_=OperationType::SUBSCRIBE;
//    for(int size : request_size) {
//        read_parcel.data_size_=size;
//        for (int number : request_number) {
//            int offset=0;
//            read_parcel.id_= "/home/jaime/projects/rhea/test/test_results/test_" + std::to_string(size) + "_" + std::to_string(number);
//            for(int i=0; i < number; i++){
//                read_parcel.position_=offset;
//                auto current_payload=payload.substr(offset, size);
//                read_parcel.buffer_=new char[size];
//                char return_data[size];
//                write_client.Subscribe(read_parcel, return_data);
//                results+=return_data;
//                offset+=size;
//            }
//        }
//    }
//
//    if(payload == results){
//        exit(EXIT_FAILURE);
//    }
//    read_client.FinalizeClient();
    write_client.FinalizeClient();
    MPI_Finalize();
}

