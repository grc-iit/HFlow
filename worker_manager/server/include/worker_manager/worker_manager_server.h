//
// Created by lukemartinlogan on 9/16/20.
//

#ifndef RHEA_WORKER_MANAGER_SERVER_H
#define RHEA_WORKER_MANAGER_SERVER_H

#include <string>
#include <worker_manager/thread_pool.h>
#include <basket.h>

namespace Rhea::WorkerManager {

class WorkerThread {
private:
    std::list<int> q_;
private:
    int GetTask();
    void ExecuteTask(int task_id);
public:
    WorkerThread();
    void Run(int task_id, std::future<void> loop_cond);
    void Enqueue(int task_id);
    int GetQueueDepth();
};

class Server {
private:
    Rhea::ThreadPool pool_;
    std::shared_ptr<RPC> client_rpc_;
    int num_tasks_exec_, min_tasks_exec_update_;
    //TODO: Add clock_t for epoch
private:
    bool ReadyToUpdateJobManager();
    void UpdateJobManager();
    WorkerThread &FindMinimumQueue();
public:
    Server();
    void Init();
    void AssignTask(int task_id);
    void Terminate();
    void Finalize();
};

};

#endif //RHEA_WORKER_MANAGER_SERVER_H
