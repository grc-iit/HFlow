//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_SERVER_H
#define RHEA_SERVER_H

#include <dataStructures/OperationType.h>
#include <dataStructures/JobNode.h>
#include <future>
#include <basket.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include <events/Event.h>
#include "tasks/Task.h"
#include "common/TaskLocation.h"
#include <dataStructures/Job.h>

#include "server/TaskExecutor.h"
#include "../../../JobManager/include/dataStructures/ResourceReport.h"
#include <cstring>
#include <basket/configuration_manager.h>
#include <sys/times.h>
#include <boost/bind.hpp>
#include <bits/dlfcn.h>
#include <dlfcn.h>


namespace Server {
    class TaskManager {
    private:

        /* RPC Server */
        std::promise<void> exit_server_signal;
        std::thread worker;
        int num_servers;
        std::shared_ptr<RPC> client_rpc;
        boost::mutex io_mutex;

        void RunInternal(std::future<void> futureObj);

        void Bind();

        int my_id;
        int jm_id;

        /* Queue Server */
        std::unordered_map<std::string, basket::queue<Rhea::Event>> queueMap;

        std::pair<bool, Rhea::Event> GetTask(const std::string &taskId);

        bool PutTask(std::string taskId, Rhea::Event event, int taskManagerId);

        /* Spawn Task */
        boost::asio::thread_pool pool;
        std::unordered_map<std::string, std::promise<void> > executorStop;

        bool SpawnTask(const std::string &taskId, Rhea::TaskLocation origin, Rhea::TaskLocation destination,
                       int currentTaskManagerId);

        bool DestroyTask(const std::string &taskId);

        /* Task Managment */
        std::unordered_map<std::string, Rhea::TaskLocation> executorDestination;

        bool updateDestination(std::string taskId, Rhea::TaskLocation newDestination);

        Rhea::TaskLocation GetDestination(std::string taskId);

        /* Library */
        bool LoadLibrary(Rhea::Job job);

        bool loadSymbol(void *handle, std::string task);

        std::map<std::string, Rhea::Task<Rhea::Event, Rhea::Event> *(*)()> taskGenerators;

        /* Measures */
        Rhea::ResourceReport previousReport;
        static clock_t lastCPU, lastSysCPU, lastUserCPU;
        static int numProcessors;

        static void InitCPUMeasure();

        int ParseLine(char *line);

        double MeasureCpu();

        int MeasureMemory();

        Rhea::ResourceReport generateLoadReport();

    public:
        //Change to submit Job by JobManager
        TaskManager(Rhea::Job job);

        bool TerminateTaskManager();
    };
}


#endif //RHEA_SERVER_H