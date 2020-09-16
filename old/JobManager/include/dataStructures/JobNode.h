//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_JOBNODE_H
#define RHEA_JOBNODE_H

#include <string>
#include <utility>

namespace Rhea {
    class JobNode {
    public:
        std::string TaskName;
        int parallelism;

        JobNode(std::string TaskName, int parallelism) : TaskName(std::move(TaskName)), parallelism(parallelism) {}

        JobNode() = default;

        bool operator==(const JobNode &other) {
            return this->TaskName == other.TaskName;
        }
    };
}

#endif //RHEA_JOBNODE_H
