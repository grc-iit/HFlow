//
// Created by jaime on 4/15/20.
//

#include <server/Server.h>
#include "ResourceManager.h"

void Rhea::ResourceManager::RunInternal(std::future<void> futureObj) {
    bool count=true;
    std::vector<Rhea::ResourceReport> state;
    std::vector<std::string, int> mod;
    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
        usleep(10000);
        state = RequestGlobalReport();
        mod = GenerateModificationRequest(state);
        jobManager.ClusterModification(mod);  //TODO: update the unordered map
        if(count){
            count=false;
        }
    }
}

std::vector<Rhea::ResourceReport> Rhea::ResourceManager::RequestGlobalReport() {
    std::vector<Rhea::ResourceReport> reports;
    for(auto client : clusterConfig.clientNodes){
        reports.push_back(RequestClientReport(client));
    }

    for(auto taskManager: clusterConfig.taskManagerNodes){
        reports.push_back(RequestTaskManagerReport(taskManager));
    }
    return reports;
}

Rhea::ResourceReport Rhea::ResourceManager::RequestTaskManagerReport(int server_id) {
    auto mem = tm.measureMemory(server_id);
    auto cpu = tm.measureCPU(server_id);
    return ResourceReport(server_id, true, mem, cpu, mem);
}

Rhea::ResourceReport Rhea::ResourceManager::RequestClientReport(int server_id) {
    return Rhea::ResourceReport();
}

std::vector<std::string, int> Rhea::ResourceManager::GenerateModificationRequest(std::vector<Rhea::ResourceReport> Report) {
    std::vector<std::string, int> r;
    return r;
}

void Rhea::ResourceManager::Run(){
    std::future<void> futureObj = exit_server_signal.get_future();
    worker=std::thread (&ResourceManager::RunInternal, this, std::move(futureObj));
}

void Rhea::ResourceManager::Stop(){
    exit_server_signal.set_value();
    worker.join();
}
