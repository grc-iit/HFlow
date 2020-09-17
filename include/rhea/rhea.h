//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_RHEA_H
#define RHEA_RHEA_H

#include <sentinel/job_manager/client.h>
#include <sentinel/worker_manager/client.h>
#include <rhea/common/data_structures.h>
#include <sentinel/common/daemon.h>
#include <basket/common/singleton.h>

namespace rhea{

class Client {
private:
public:
    Client();
    ~Client();

    // Initialization
    bool SubmitJob(uint32_t jid);
    bool TerminateJob(uint32_t jid);

    // Admin API

    // Source API
    bool CreateSource(CharStruct name);
    bool DeleteSource(CharStruct name);
    bool Publish(CharStruct src, CharStruct message);

    // Sink API
    bool CreateSink(CharStruct name);
    bool DeleteSink(CharStruct name);
    bool SubscribeSink(CharStruct src);
    bool UnsubscribeSink(CharStruct src);
};
}


#endif //RHEA_RHEA_H
