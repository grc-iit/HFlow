//
// Created by jaime on 4/15/20.
//

#ifndef RHEA_FUNCTIONMANAGER_H
#define RHEA_FUNCTIONMANAGER_H

#include <rapidjson/pointer.h>
#include <rapidjson/filereadstream.h>
#include <iostream>
#include <string>
#include <dlfcn.h>
#include "dataStructures/Job.h"

namespace Rhea {
    class FunctionManager {
    public:
        FunctionManager() = default;

        std::pair<bool, Job> generateJob(const std::string &jobPath);

    private:
        static std::pair<bool, Job> parseJob(const std::string &jobPath);

        bool validateJob(const Job &job, const std::string &jobPath);

        static rapidjson::Document openJob(const std::string &jobPath);

        bool parseSymbol(void *handle, const char *symbol);
    };
}


#endif //RHEA_FUNCTIONMANAGER_H
