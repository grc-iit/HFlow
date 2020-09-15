//
// Created by jaime on 4/16/20.
//

#ifndef RHEA_RESOURCEREPORT_H
#define RHEA_RESOURCEREPORT_H

namespace Rhea {
    class ResourceReport {
    public:
        int serverId;
        float memoryUsage;
        double cpuUsage;
        float queueUsage;

        ResourceReport(int serverId, float memoryUsage, double cpuUsage, float queueUsage) :
                serverId(serverId), memoryUsage(memoryUsage), cpuUsage(cpuUsage), queueUsage(queueUsage) {}

        ResourceReport() = default;

        bool operator<(const ResourceReport &rr) const {
            //For now, we just compare memory usage, we can make this more complex
            return (memoryUsage < rr.memoryUsage);
        }

        bool operator>(const ResourceReport &rr) const {
            //For now, we just compare memory usage, we can make this more complex
            return (memoryUsage > rr.memoryUsage);
        }

        bool operator==(const ResourceReport &rr) {
            int memVariability = 0.1; //I can accept that memory can vary 10% without calling them different
            int memRange = memoryUsage * memVariability;
            return (memoryUsage + memRange) > rr.memoryUsage && (memoryUsage - memRange) < rr.memoryUsage;
        }
    };
}
#endif //RHEA_RESOURCEREPORT_H
