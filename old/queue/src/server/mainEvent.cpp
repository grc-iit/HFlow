//
// Created by hariharan on 8/15/19.
//

#include <mpi.h>
#include <basket/common/macros.h>
#include <basket/configuration_manager.h>
#include <basket/daemonize.h>
#include <basket/data_structure.h>
#include <server/server.h>

void finalize(){
    MPI_Barrier(MPI_COMM_WORLD);
    auto server = basket::Singleton<Rhea::Server<std::vector<Event>>>::GetInstance();
    server->Stop();
    MPI_Finalize();
}

int main(int argc, char* argv[]){
    MPI_Init_thread(&argc,&argv,true,NULL);
    MPI_Barrier(MPI_COMM_WORLD);
    if (argc > 1) RHEA_CONF->CONFIGURATION_FILE = argv[1];
    BASKET_CONF->BACKED_FILE_DIR = RHEA_CONF->QUEUE_DIR;
    RHEA_CONF->ConfigureQueueServer();
    auto server = basket::Singleton<Rhea::Server<std::vector<Event>>>::GetInstance();
    server->Run();
    strcpy(main_log_file,"Rhea queue server.log");
    catch_all_signals();
    while(true) sleep(1);
}
