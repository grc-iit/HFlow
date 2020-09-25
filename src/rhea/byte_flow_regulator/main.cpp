//
// Created by mani on 9/14/2020.
//

#include <mpi.h>
#include <basket.h>
#include <common/daemon.h>
#include <common/arguments.h>
#include <rhea/common/configuration_manager.h>
#include "rhea/byte_flow_regulator/server.h"

class RheaServerArgs : public common::args::ArgMap {
private:
    void VerifyArgs(void) {}

public:
    void Usage(void) {
        std::cout << "Usage: ./rhea -[param-id] [value] ... " << std::endl;
        std::cout << "-conf [string]: The config file for rhea. Default is no config." << std::endl;
    }

    RheaServerArgs(int argc, char **argv) {
        AddOpt("-conf", common::args::ArgType::kString, "");
        ArgIter(argc, argv);
        VerifyArgs();
    }
};

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    RheaServerArgs args(argc, argv);
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    BASKET_CONF->BACKED_FILE_DIR=RHEA_CONF->BYTEFLOW_REGULATOR_DIR;
    CharStruct log = "./single_node_byteflow_regulator.log";
    auto daemon = basket::Singleton<common::Daemon<rhea::byteflow_regulator::Server>>::GetInstance(log);
    daemon->Run();
    return 0;
}