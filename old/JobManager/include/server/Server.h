//TaskExecutor
// Created by jaime on 4/15/20.
//

#ifndef RHEA_SERVER_H
#define RHEA_SERVER_H

#include <string>
#include <vector>
#include <future>
#include <basket.h>
#include <dataStructures/OperationType.h>
#include "dataStructures/ClusterConfig.h"
#include "dataStructures/ClusterNode.h"
#include "dataStructures/JobNode.h"
#include "dataStructures/ResourceReport.h"

namespace Server {
    class JobManager {
    private:
        std::unordered_map<Rhea::ClusterNode, Rhea::ResourceReport> loadMap;
        Rhea::ClusterNode findMinLoad();

        Rhea::ClusterNode GiveMeDestination(int taskId);

        Rhea::ClusterNode CurrentNode;
        float MemoryLimit;
        Rhea::ResourceReport maxLoad = Rhea::ResourceReport(0, MemoryLimit, 0, 0);

        bool UpdateLoad(const Rhea::ClusterNode &cn, Rhea::ResourceReport rr);

        basket::unordered_map<Rhea::ClusterNode, std::vector<int>> taskDeploymentMap; //Maps a culterNode to a collection of tasks
        std::unordered_map<int, taskStatus> taskMap;
        std::unordered_map<int, int> destinationMap;

        std::shared_ptr<RPC> client_rpc;

        void RunInternal(std::future<void> futureObj);

        Rhea::ClusterConfig clusterConfig;

        Rhea::ClusterConfig parseClusterConfig(std::string configPath);

        std::string taskManagerExecutable = "../TaskManager/TaskManager";
        MPI_Comm taskManagerComm;
    public:
        bool SubmitJob(std::string job);

        bool StopJob();

        JobManager();

        void StopJobManager();

        void BindRPC();

        void FinalizeTaskManager(int taskManagerId);

        void SpawnTaskManagers();
    };
}


#endif //RHEA_SERVER_H
