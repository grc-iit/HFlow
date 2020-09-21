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
    MPI_Barrier(MPI_COMM_WORLD);

    uint32_t jobId = 0;

//    COMMON_CONF;
//    auto classLoader = ClassLoader();
//    std::shared_ptr<Job<Event>> job = classLoader.LoadClass<Job<Event>>(jobId);

    rhea::Client client(jobId);

    Parcel parcel;
    parcel.id_="/home/jaime/projects/rhea/test/test_file.txt";
    parcel.storage_index_=0;
    parcel.type_=OperationType::PUBLISH;
    parcel.position_=0;
    parcel.data_size_=4;
    parcel.buffer_="abcd";
    char* data = "abcd";
    client.Publish(parcel, data);

    client.FinalizeClient();

    MPI_Finalize();
}
