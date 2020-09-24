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

    std::vector<uint32_t> request_number = {1000};
    std::vector<uint32_t> request_size = {4};

    uint payload_size=0;
    for(int size : request_size) { for (int number : request_number) {payload_size+=(size*number);}}
    std::string payload = random_string(payload_size);

    std::cout << payload << std::endl;
    auto filename = RHEA_CONF->ReplaceEnvVariable("${HOME}/test_file.txt");
    JobId jobId = 0;
    rhea::Client write_client(jobId);
    auto parcels = std::vector<Parcel>();
    auto timer = Timer();
    timer.resumeTime();
    for(int size : request_size) {
        for (int number : request_number) {
            int offset=0;
            for(int i=0; i < number; i++){
                Parcel write_parcel;
                write_parcel.data_size_=size;
                write_parcel.id_= filename;
                write_parcel.storage_index_=0;
                write_parcel.type_=OperationType::PUBLISH;
                write_parcel.position_=offset;
                auto current_payload = payload.substr(offset, size);
                char* rhea_data = current_payload.data();
                write_client.Publish(write_parcel, rhea_data);
                parcels.push_back(write_parcel);
                offset+=size;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    timer.pauseTime();
    auto async_time = timer.getElapsedTime();
    timer.resumeTime();
    write_client.WaitAll(parcels);
    MPI_Barrier(MPI_COMM_WORLD);
    timer.pauseTime();
    printf("Async Write Time %f, Sync Write time %f\n",async_time,timer.getElapsedTime());
    write_client.FinalizeClient();
    MPI_Finalize();
}

