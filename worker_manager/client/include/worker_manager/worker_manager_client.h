//
// Created by lukemartinlogan on 9/16/20.
//

#ifndef RHEA_WORKER_MANAGER_CLIENT_H
#define RHEA_WORKER_MANAGER_CLIENT_H

namespace Rhea::WorkerManager {

class Client {
private:
    std::shared_ptr<RPC> server_rpc;
public:
    Client();
    void Init();
    void AssignTask(int server_index, int task_id);
    void TerminateWorkerManager(int server_index);
    void FinalizeWorkerManager(int server_index);
    void Finalize();
};

};

#endif //RHEA_WORKER_MANAGER_CLIENT_H
