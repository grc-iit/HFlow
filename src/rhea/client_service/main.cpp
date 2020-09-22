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
    RHEA_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    BASKET_CONF->BACKED_FILE_DIR = RHEA_CONF->RHEA_CLIENT_SERVICE_DIR;
    CharStruct log = "./single_node_byteflow_regulator.log";
    RHEA_CONF->ConfigureRheaClientService();
    auto daemon = basket::Singleton<common::Daemon<rhea::RheaClient>>::GetInstance(log);
    daemon->Run();
    return 0;
}