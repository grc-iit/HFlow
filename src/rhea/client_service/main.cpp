#include <mpi.h>
#include <basket/common/data_structures.h>
#include <rhea/common/configuration_manager.h>
#include <basket/common/macros.h>
#include <common/daemon.h>
#include <common/arguments.h>
#include <basket/common/singleton.h>
#include <rhea/client_service/rhea_client.h>

class RheaClientArgs : public common::args::ArgMap {
private:
    void VerifyArgs(void) {}

public:
    void Usage(void) {
        std::cout << "Usage: ./rhea -[param-id] [value] ... " << std::endl;
        std::cout << "-conf [string]: The config file for rhea. Default is no config." << std::endl;
    }

    RheaClientArgs(int argc, char **argv) {
        AddOpt("-conf", common::args::ArgType::kString, "");
        ArgIter(argc, argv);
        VerifyArgs();
    }
};

//
// Created by hdevarajan on 9/18/20.
//
int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    RheaClientArgs args(argc, argv);
    COMMON_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    CharStruct log = "./single_node_byteflow_regulator.log";
    COMMON_CONF->LoadConfiguration();
    auto daemon = basket::Singleton<common::Daemon<rhea::RheaClient>>::GetInstance(log);
    daemon->Run();
    return 0;
}