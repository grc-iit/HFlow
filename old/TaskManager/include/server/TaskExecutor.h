//
// Created by jaime on 4/20/20.
//

#ifndef RHEA_TASKEXECUTOR_H
#define RHEA_TASKEXECUTOR_H


#include <string>
#include <events/Event.h>
#include "tasks/Task.h"
#include <common/TaskLocation.h>
#include <client/Client.h>
#include <utility>

namespace Rhea {
    class TaskExecutor {
    private:
        double timeout = 10000; //ms

        void init();

        void exec(std::string taskId, std::function<Rhea::Task<Rhea::Event, Rhea::Event> *()> taskGenerator,
                  const TaskLocation &origin,
                  int currentTaskMangerId, std::future<void> futureObj);

        void finalize(int currentTasManagerId);

        void send(std::string taskId, Event *result, Event original, int currentTaskMangerId);

    public:
        /**
        * What do we need:
        *  The task to run.
         *  Location of destination (TM1 and T1)
         *  Location of orgin (TM2 and T2). this T2 is our task ID
         *  Current TM3, the task manager where we are running.
         *  Ususally, we pool from ourselves, so TM@=TM3, doesnt apply for collectors
         *  Also. keyby events return destinations so there shoudl be speciall logic
        */
        void run(std::string taskId, std::function<Rhea::Task<Rhea::Event, Rhea::Event> *()> taskGenerator,
                 TaskLocation origin, int currentTaskMangerId,
                 std::future<void> futureObj);
    };
}


#endif //RHEA_TASKEXECUTOR_H
