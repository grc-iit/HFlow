//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_RHEA_H
#define RHEA_RHEA_H

#include <sentinel/job_manager/client.h>
#include <rhea/common/data_structures.h>
#include <sentinel/common/daemon.h>
#include <basket/common/singleton.h>
#include <basket/queue/queue.h>
#include <basket/unordered_map/unordered_map.h>
#include <rhea/client/ByteFlow_Regulator_Client.h>
#include <rhea/common/configuration_manager.h>

namespace rhea{

class ByteflowPusher {
private:
    std::shared_ptr<rhea::ByteFlow_Regulator_Client> client;
public:
    void Run(std::future<void> futureObj);
    void RunInternal(std::future<void> futureObj);
};

class Client {
private:
    std::unordered_map<std::string, std::shared_ptr<basket::queue<Parcel>>> sources;
    std::shared_ptr<basket::unordered_map<uint32_t, size_t>> sizes;
public:
    Client();
    ~Client();

    // Initialization
    bool SubmitJob(uint32_t jid);
    bool TerminateJob(uint32_t jid);

    // Admin API

    // Source API
    bool CreateSource(std::string name);
    bool DeleteSource(std::string name);
    bool Publish(std::string srcid, CharStruct message, uint32_t jid);

    // Sink API
    bool CreateSink(CharStruct name);
    bool DeleteSink(CharStruct name);
    bool SubscribeSink(CharStruct srcid);
    bool UnsubscribeSink(CharStruct srcid);
};
}


#endif //RHEA_RHEA_H
