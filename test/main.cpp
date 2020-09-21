//
// Created by hdevarajan on 9/19/20.
//

#include <mpi.h>
#include <rhea/rhea.h>
#include <basket/common/singleton.h>
#include <sentinel/worker_manager/client.h>
#include <common/enumerations.h>
//#include <common/class_loader.h>

int main(int argc, char * argv[]){
    MPI_Init(&argc,&argv);
    uint32_t jobId = 0;
    rhea::Client write_client(jobId);
    Parcel parcel;
    parcel.id_="/home/jaime/projects/rhea/test/test_file.txt";
    parcel.storage_index_=0;
    parcel.type_=OperationType::PUBLISH;
    parcel.position_=0;
    parcel.data_size_=4;
    parcel.buffer_="abcd";
    char* data = "abcd";
    write_client.Publish(parcel, data);
    uint32_t read_jobId = 1;
    rhea::Client read_client(read_jobId);

    char* return_data = (char*)malloc(strlen(data)+1);
    read_client.Subscribe(parcel, data);

    if(strcmp(data,return_data) != 0){
        exit(EXIT_FAILURE);
    }
    read_client.FinalizeClient();
    write_client.FinalizeClient();
    MPI_Finalize();
}
