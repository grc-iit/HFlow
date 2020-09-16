//
// Created by jaime on 4/21/20.
//

#ifndef RHEA_TASKLOCATION_H
#define RHEA_TASKLOCATION_H

#include <string>

namespace Rhea {
    class TaskLocation {
    public:
        int taskManagerId;
        std::string taskId;

        TaskLocation(int taskManagerId, std::string taskId) : taskManagerId(taskManagerId), taskId(taskId) {};
    };
}


#endif //RHEA_TASKLOCATION_H
