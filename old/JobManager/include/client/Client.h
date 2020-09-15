//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_CLIENT_H
#define RHEA_CLIENT_H

#include <basket.h>
#include <basket/common/macros.h>
#include <basket/communication/rpc_factory.h>
#include <basket/configuration_manager.h>
#include <rpc/client.h>
#include "../dataStructures/ClusterNode.h"

namespace Client {
    class JobManager {
    private:
        basket::unordered_map<Rhea::ClusterNode, Rhea::ResourceReport> loadMap;

        std::shared_ptr<RPC> rpc;
        int num_servers;
        boost::mutex mtx;
    public:
        JobManager();

        bool Put(Rhea::ClusterNode, Rhea::ResourceReport);

        Rhea::ResourceReport Get(Rhea::ClusterNode);

        bool Delete(Rhea::ClusterNode node);

        bool SubmitJob(std::string job, int server_index);

        bool StopJob(int server_index);

        bool UpdateLoad(int jobManager_index, Rhea::ClusterNode cn, Rhea::ResourceReport rr);
    };
}


#endif //RHEA_CLIENT_H
