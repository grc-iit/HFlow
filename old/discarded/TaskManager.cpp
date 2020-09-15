#include <iostream>
#include <mpi.h>
#include "../../jobPrimitives/include/dag/Dag.h"
#include <future>
#include "basket/configuration_manager.h"
#include "client/client.h"

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    Rhea::Dag dag;
    int size, rank;
    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);
    if (parent == MPI_COMM_NULL){
        std::cerr << "Something's wrong with the parent" << std::endl;
        MPI_Finalize();
        return -1;
    }
    MPI_Comm_remote_size(parent, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size != 1){
        std::cerr << "Something's wrong with the parent" << std::endl;
        MPI_Finalize();
        return -1;
    }

    std::cout << "Rank: " << rank << " of " << size << " : ";

    //Queue
    QUEUE_CONF->CONFIGURATION_FILE = CharStruct(conf));
    QUEUE_CONF->LoadConfiguration();
    auto client = basket::Singleton<Rhea::Client>::GetInstance(false);

    std::vector<int> tasks(3);

    std::generate(tasks.begin(), tasks.end(), [] () { return std::rand()%5; });

    std::vector<std::future<void>> buffers(tasks.size());
    for(auto task : tasks){
        auto func = dag.data_of(task).func;
        buffers.push_back(std::async(std::launch::async, func, rank));
    }

    MPI_Finalize();
    return 0;
} 