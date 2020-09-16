//
// Created by jaime on 4/15/20.
//

#include <client/Client.h>

Client::TaskManager::TaskManager() :
        num_servers(BASKET_CONF->NUM_SERVERS) {
    mtx.lock();
    rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(RHEA_CONF->JOB_MANAGER_SERVER_PORT);
    mtx.unlock();
}

bool Client::TaskManager::InstructTaskManager(int server_index, Rhea::JobNode node, opType OP) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "InstructTaskManager", node, OP).as<bool>();
    return check;
}

bool Client::TaskManager::TerminateTaskManager(int server_index) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "TerminateTaskManager").as<bool>();
    return check;
}

double Client::TaskManager::measureCPU(int server_index) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "measureCPU").as<double>();
    return check;
}

int Client::TaskManager::measureMemory(int server_index) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "measureMemory").as<int>();
    return check;
}

std::pair<bool, Rhea::Event> Client::TaskManager::GetTask(int server_index, std::string taskId) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "GetTask",
                                                          std::move(taskId)).as<std::pair<bool, Rhea::Event>>();
    return check;
}

bool Client::TaskManager::PutTask(int server_index, std::string taskId, Rhea::Event event) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "PutTask", std::move(taskId), std::move(event),
                                                          server_index).as<bool>();
    return check;
}

bool Client::TaskManager::UpdateDestination(int server_index, std::string taskId, Rhea::TaskLocation newDestination) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "UpdateDestination", std::move(taskId),
                                                          std::move(newDestination)).as<bool>();
    return check;
}

Rhea::TaskLocation Client::TaskManager::GetDestination(int server_index, std::string taskId) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "GetDestination",
                                                          std::move(taskId)).as<Rhea::TaskLocation>();
    return check;
}

bool Client::TaskManager::SpawnTask(int server_index, std::string taskId, Rhea::TaskLocation origin,
                                    Rhea::TaskLocation destination) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "SpawnTask", std::move(taskId),
                                                          std::move(origin), std::move(destination),
                                                          server_index).as<bool>();
    return check;
}

bool Client::TaskManager::DestroyTask(int server_index, std::string taskId) {
    auto check = rpc->call<RPCLIB_MSGPACK::object_handle>(server_index, "PutTask", std::move(taskId)).as<bool>();
    return check;
}

/**
 * updateDestination(std::string taskId, Rhea::TaskLocation newDestination)
 * bool Server::TaskManager::SpawnTask(const std::string& taskId, Rhea::TaskLocation origin, Rhea::TaskLocation destination, int currentTaskManagerId)
 * bool Server::TaskManager::DestroyTask(const std::string& taskId) {
 * Rhea::TaskLocation Server::TaskManager::GetDestination(std::string taskId)
 */