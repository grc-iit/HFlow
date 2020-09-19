//
// Created by mani on 9/14/2020.
//

#include <mpi.h>
#include <basket.h>
#include <common/daemon.h>
#include <rhea/common/configuration_manager.h>
#include "rhea/server/byte_flow_regulator_server.h"

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    if(argc > 1) RHEA_CONF->CONFIGURATION_FILE=argv[1];
    BASKET_CONF->BACKED_FILE_DIR=RHEA_CONF->BYTEFLOW_REGULATOR_DIR;
    CharStruct log = "./single_node_byteflow_regulator.log";
    auto daemon = basket::Singleton<common::Daemon<rhea::ByteFlowRegulatorServer>>::GetInstance(log);
    daemon->Run();
    return 0;
}