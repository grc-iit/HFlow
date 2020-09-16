//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_CLIENT_H
#define RHEA_JOBMANAGERCLIENT_H

#include <basket/common/macros.h>
#include <basket/communication/rpc_factory.h>
#include <rpc/client.h>
#include <basket/configuration_manager.h>
#include <dataStructures/JobNode.h>
#include <dataStructures/OperationType.h>
#include <events/Event.h>
#include <common/TaskLocation.h>

namespace Client {
    class TaskManager {
    private:
        std::shared_ptr<RPC> rpc;
        int num_servers;
        boost::mutex mtx;
    public:
        TaskManager();

        bool InstructTaskManager(int server_index, Rhea::JobNode node, opType OP);

        bool TerminateTaskManager(int server_index);

        double measureCPU(int server_index);

        int measureMemory(int server_index);

        std::pair<bool, Rhea::Event> GetTask(int server_index, std::string taskId);

        bool PutTask(int server_index, std::string taskId, Rhea::Event event);

        bool UpdateDestination(int server_index, std::string taskId, Rhea::TaskLocation newDestination);

        Rhea::TaskLocation GetDestination(int server_index, std::string taskId);

        bool SpawnTask(int server_index, std::string taskId, Rhea::TaskLocation origin, Rhea::TaskLocation destination);

        bool DestroyTask(int server_index, std::string taskId);
    };
}


#endif //RHEA_CLIENT_H
