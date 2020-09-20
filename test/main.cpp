//
// Created by hdevarajan on 9/19/20.
//

#include <mpi.h>
#include <rhea/rhea.h>
#include <basket/common/singleton.h>
#include <sentinel/worker_manager/client.h>
//#include <common/class_loader.h>

int main(int argc, char * argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);

    uint32_t jobId = 2;

//    COMMON_CONF;
//    auto classLoader = ClassLoader();
//    std::shared_ptr<Job<Event>> job = classLoader.LoadClass<Job<Event>>(jobId);

    rhea::Client client(jobId);

    Parcel parcel;
    char* data = "   ";
    client.Publish(parcel, data);


    MPI_Finalize();
}
