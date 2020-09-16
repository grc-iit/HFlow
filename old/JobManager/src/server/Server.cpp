//
// Created by jaime on 4/15/20.
//

#include "server/Server.h"
#include "../../../TaskManager/include/server/Server.h"

#include <server/FunctionManager.h>
#include <dataStructures/Job.h>
#include <zconf.h>
#include <basket/configuration_manager.h>
#include "../../../TaskManager/include/client/Client.h"
#include <mpi.h>

Server::JobManager::JobManager() : deploymentMap("DeploymentMap"), loadMap("LoadMap") {

    clusterConfig = parseClusterConfig(); //TODO: Argv

    std::future<void> futureObj = exit_server_signal.get_future();
    RunInternal(std::move(futureObj));

}

void Server::JobManager::RunInternal(std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        usleep(10000);

        //DO STUFF
    }
}

void Server::JobManager::StopJobManager() {
    for (int taskManagerId: clusterConfig.taskManagerNodes) {
        FinalizeTaskManager(taskManagerId);
    }
    exit_server_signal.set_value();
}

bool Server::JobManager::SubmitJob(std::string job) {
    Rhea::FunctionManager functionManager;
    auto parsedJob = functionManager.generateJob(job);

    Client::TaskManager taskManagerClient;
    for (int taskManagerId: clusterConfig.taskManagerNodes) {
        taskManagerClient.SubmitJob();
    }

    //TODO: Spawn Collectors

    return false;
}

bool Server::JobManager::StopJob() {
    Client::TaskManager taskManagerClient;
    for (int taskManagerId: clusterConfig.taskManagerNodes) {
        taskManagerClient.StopJob();
    }

    //TODO: Delete Metadata
    return true;
}

void Server::JobManager::SpawnTaskManagers() {
    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "hostfile",
                 ""); //TODO: path to taskManager hosts (https://www.open-mpi.org/doc/current/man3/MPI_Comm_spawn.3.php)

    MPI_Comm_spawn(taskManagerExecutable.c_str(), MPI_ARGV_NULL, clusterConfig.taskManagerNodes.size(),
                   info, 0, MPI_COMM_SELF, &taskManagerComm,
                   MPI_ERRCODES_IGNORE);
}

void Server::JobManager::FinalizeTaskManager(int taskManagerId) {
    Client::TaskManager taskManagerClient;
    taskManagerClient.TerminateTaskManager(taskManagerId);
}

void Server::JobManager::BindRPC() {
    client_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(RHEA_CONF->JOB_MANAGER_SERVER_PORT);

    auto functionUpdateDestination(
            std::bind(&::Server::JobManager::updateDestination, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc->bind("UpdateDestination", functionUpdateDestination);

    auto functionSpawnTask(
            std::bind(&::Server::JobManager::SpawnTask, this, std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3, std::placeholders::_4));
    client_rpc->bind("SpawnTask", functionSpawnTask);

    auto functionDestroyTask(std::bind(&::Server::JobManager::DestroyTask, this, std::placeholders::_1));
    client_rpc->bind("DestroyTask", functionDestroyTask);

    auto functionUpdateLoad(
            std::bind(&::Server::JobManager::UpdateLoad, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc->bind("UpdateLoad", functionUpdateLoad);
}

Rhea::ClusterConfig Server::JobManager::parseClusterConfig(std::string configPath) {

}

//LoadMap
Rhea::ClusterNode Server::JobManager::findMinLoad() {
    std::pair<Rhea::ClusterNode, Rhea::ResourceReport> min
            = *min_element(loadMap.begin(), loadMap.end(),
                           [](const auto &l, const auto &r) { return l.second < r.second; });
    return min.first;
}

bool Server::JobManager::UpdateLoad(const Rhea::ClusterNode &cn, Rhea::ResourceReport rr) {
    loadMap[cn] = rr;
    return true;
}

//Deployment Map
Rhea::ClusterNode Server::JobManager::GiveMeDestination(int taskId) {
    if (loadMap.at(CurrentNode) > maxLoad || destinationMap.find(taskId) == destinationMap.end() ||
        taskMap.at(destinationMap.at(taskId))) {
        Rhea::ClusterNode newNode = findMinLoad();
        CurrentNode = newNode;
        return newNode;
    }
    return CurrentNode;
}
