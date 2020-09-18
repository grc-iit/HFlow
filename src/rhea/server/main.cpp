//
// Created by mani on 9/14/2020.
//

#include <mpi.h>
#include <basket.h>
#include <sentinel/common/daemon.h>
#include "server.h"

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    if(argc > 1) SENTINEL_CONF->CONFIGURATION_FILE=argv[1];
    BASKET_CONF->BACKED_FILE_DIR=SENTINEL_CONF->JOBMANAGER_DIR;
    CharStruct log = "./single_node_byteflow_regulator.log";
    auto daemon = basket::Singleton<common::Daemon<rhea::server()>>::GetInstance(log);
    daemon->Run();
    return 0;
}