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
    Parcel write_parcel;
    write_parcel.id_ = RHEA_CONF->ReplaceEnvVariable("${HOME}/test_file.txt");
    write_parcel.storage_index_=0;
    write_parcel.type_=OperationType::PUBLISH;
    write_parcel.position_=0;
    write_parcel.data_size_=4;
    char* data = "abcd";
    write_client.Publish(write_parcel, data);
    write_client.Wait(write_parcel);
    uint32_t read_jobId = 1;
    Parcel read_parcel;
    read_parcel.id_ = RHEA_CONF->ReplaceEnvVariable("${HOME}/test_file.txt");
    read_parcel.storage_index_=0;
    read_parcel.type_=OperationType::PUBLISH;
    read_parcel.position_=0;
    read_parcel.data_size_=4;
    rhea::Client read_client(read_jobId);
    char return_data[4];
    read_client.Subscribe(read_parcel, return_data);
    read_client.Wait(read_parcel);
    read_client.FinalizeClient();
    write_client.FinalizeClient();
    MPI_Finalize();
}
