//parsedJob
// Created by jaime on 4/15/20.
//

#ifndef RHEA_JOB_H
#define RHEA_JOB_H

#include <unordered_map>
#include <utility>
#include <vector>
#include "JobNode.h"

namespace Rhea {
    class Job {
    public:
        std::string jobPath;
        std::unordered_map<std::string, std::vector<std::string>> adjacency;
        std::unordered_map<std::string, JobNode> taskData;

        Job(std::string jobPath, std::unordered_map<std::string, std::vector<std::string>> adjacency,
            std::unordered_map<std::string, JobNode> taskData) : adjacency(std::move(adjacency)),
                                                                 taskData(std::move(taskData)),
                                                                 jobPath(std::move(jobPath)) {}

        Job() = default;;
    };
}


#endif //RHEA_JOB_H
