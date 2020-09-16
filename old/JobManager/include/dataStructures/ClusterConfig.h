//
// Created by jaime on 4/16/20.
//

#ifndef RHEA_CLUSTERCONFIG_H
#define RHEA_CLUSTERCONFIG_H

#include <vector>

namespace Rhea {
    class ClusterConfig {
    public:
        std::vector<int> clientNodes;
        std::vector<int> taskManagerNodes;
        std::vector<int> jobMangerNodes;
    };
}
#endif //RHEA_CLUSTERCONFIG_H
