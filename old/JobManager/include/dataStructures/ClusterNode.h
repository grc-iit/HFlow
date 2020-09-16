//
// Created by jaime on 4/24/20.
//

#ifndef RHEA_CLUSTERNODE_H
#define RHEA_CLUSTERNODE_H

#include <string>
#include <utility>

namespace Rhea {
    class ClusterNode {
    public:
        std::string ip;
        int nodeId;

        ClusterNode(std::string ip, int nodeId) : ip(std::move(ip)), nodeId(nodeId) {};

        bool operator==(const ClusterNode &cn) const {
            return (nodeId == cn.nodeId);
        }
    };
}

namespace std {
    template<>
    struct hash<Rhea::ClusterNode> {
        size_t operator()(const Rhea::ClusterNode &cn) const {
            // Compute individual hash values for two data members and combine them using XOR and bit shifting
            return ((hash<int>()(cn.nodeId) ^ hash<std::string>()(cn.ip) << 1) >> 1);
        }
    };
}

#endif //RHEA_CLUSTERNODE_H
