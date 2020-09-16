//
// Created by jaime on 4/15/20.
//

#include "client/Client.h"


Client::JobManager::JobManager() :
        loadMap("LoadMap"),
        num_servers(BASKET_CONF->NUM_SERVERS) {

    rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(RHEA_CONF->JOB_MANAGER_SERVER_PORT);
}

bool Client::JobManager::SubmitJob(std::string job, int server_index) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "SubmitJob", job).as<bool>();
    return check;
}

bool Client::JobManager::StopJob(int server_index) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "StopJob").as<bool>();
    return check;
}


bool Client::JobManager::Put(Rhea::ClusterNode, Rhea::ResourceReport) {
    auto b_str = bip::string(value.c_str());
    bool status = loadMap.Put(key, b_str) && meta_set.Put(key);
    return status;
}

Rhea::ResourceReport Client::JobManager::Get(Rhea::ClusterNode) {
    auto p = loadMap.Get(key);
    auto status = std::pair<bool, std::string>(p.first, p.second.c_str());
    return status;
}

bool Client::JobManager::Delete(Rhea::ClusterNode node) {
    return loadMap.Erase(node);
}

bool Client::JobManager::UpdateLoad(int jobManager_index, Rhea::ClusterNode cn, Rhea::ResourceReport rr) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(jobManager_index, "UpdateLoad", cn, rr).as<bool>();
    return check;
}
