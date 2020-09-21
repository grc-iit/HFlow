#include <mpi.h>
#include <basket/common/data_structures.h>
#include <rhea/common/configuration_manager.h>
#include <basket/common/macros.h>
#include <common/daemon.h>
#include <basket/common/singleton.h>
#include <rhea/client_service/rhea_client.h>

//
// Created by hdevarajan on 9/18/20.
//
int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    if(argc > 1) RHEA_CONF->CONFIGURATION_FILE = argv[1];
    BASKET_CONF->BACKED_FILE_DIR = RHEA_CONF->RHEA_CLIENT_SERVICE_DIR;
    CharStruct log = "./single_node_byteflow_regulator.log";
    RHEA_CONF->ConfigureRheaClientService();
    auto daemon = basket::Singleton<common::Daemon<rhea::RheaClient>>::GetInstance(log);
    daemon->Run();
    return 0;
}