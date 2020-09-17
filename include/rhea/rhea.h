//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_RHEA_H
#define RHEA_RHEA_H

#include <sentinel/job_manager/client.h>
#include <rhea/common/data_structures.h>

namespace rhea{

class Client {
private:
    std::vector<struct DataSource> sources;
    std::vector<struct StorageSolution> sinks;
public:
    Client();
    ~Client();

    // Initialization
    bool SubmitJob(uint32_t jid);
    bool ReleaseJob(uint32_t jid);

    // Admin API

    // Source API
    bool CreateSource(std::string name);
    bool DeleteSource(std::string name);
    bool Publish(std::string src, CharStruct message);

    // Sink API
    bool CreateSink(std::string name);
    bool DeleteSink(std::string name);
    bool SubscribeSink(std::string src);
    bool UnsubscribeSink(std::string src);
};
}


#endif //RHEA_RHEA_H
