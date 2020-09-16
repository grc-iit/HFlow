//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_RESOURCEMANAGER_H
#define RHEA_RESOURCEMANAGER_H


#include <future>
#include <utility>
#include <unistd.h>
#include "dataStructures/Job.h"
#include "dataStructures/ResourceReport.h"
#include "dataStructures/ClusterConfig.h"

#include "client/Client.h"


namespace Rhea {
    class ResourceManager {
    private:
        std::promise<void> exit_server_signal;
        std::thread worker;
        void RunInternal(std::future<void> futureObj);
        Job job;
        Client::TaskManager tm;
        ClusterConfig clusterConfig;
        std::vector<ResourceReport> RequestGlobalReport();
        ResourceReport RequestTaskManagerReport(int server_id);
        ResourceReport RequestClientReport(int server_id);
        std::vector<std::string, int> GenerateModificationRequest(std::vector<Rhea::ResourceReport> Report); //Task Name, +1 (etc.)
    public:
        ResourceManager(Job job, ClusterConfig clusterConfig): job(std::move(job)), clusterConfig(std::move(clusterConfig)){}
        void Run();
        void Stop();
    };
}


#endif //RHEA_RESOURCEMANAGER_H
