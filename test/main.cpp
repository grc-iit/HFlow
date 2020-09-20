//
// Created by hdevarajan on 9/19/20.
//

#include <mpi.h>
#include <rhea/rhea.h>
#include <basket/common/singleton.h>

int main(int argc, char * argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);

    uint32_t job_id = 0;
    rhea::Client client(job_id);

    Parcel parcel;
    char* data = "   ";
    client.Publish(parcel, data);


    MPI_Finalize();
}
