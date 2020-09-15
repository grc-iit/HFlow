/* manager */
#include <iostream>
#include "mpi.h"
#include "../../jobPrimitives/include/dag/Dag.h"
#include "basket/configuration_manager.h"
#include "client/client.h"

int main(int argc, char *argv[])
{
    Rhea::Dag dag;

//    std::vector<std::function<void()>> funcs;
//    std::set<node_id> dag_set = Dag.next_of(0);
//    for(int node : dag_set) {
//        Dag.data_of(node).func();
//        std::cout << "---------" << std::endl;
//    }

    int world_size, *universe_size, flag, available_size;
    MPI_Comm everyone;           /* intercommunicator */
    std::string taskManager = "../TaskManager/TaskManager";

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_size, &flag);;

    available_size = *universe_size - world_size;

    std::cout << *universe_size << " " << world_size << " " << available_size << std::endl;

    std::cout << system("pwd");

    if (available_size <= 0){
        std::cerr << "No more space" << std::endl;
        MPI_Finalize();
        return -1;
    }

    std::cout << "Master pre-spawn" << std::endl;

    //Queue
    RHEA_CONF->CONFIGURATION_FILE = CharStruct(conf));
    RHEA_CONF->LoadConfiguration();
    RHEA_CONF->ConfigureQueueClient();
    auto client = basket::Singleton<Rhea::Client>::GetInstance(false);

    MPI_Comm_spawn(taskManager.c_str(), MPI_ARGV_NULL, available_size,
                   MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,
                   MPI_ERRCODES_IGNORE);

    std::cout << "Master post-spawn" << std::endl;
    /*
     * Parallel code here. The communicator "everyone" can be used
     * to communicate with the spawned processes, which have ranks 0,..
     * MPI_UNIVERSE_SIZE-1 in the remote group of the intercommunicator
     * "everyone".
     */

    MPI_Finalize();
    return 0;
} 