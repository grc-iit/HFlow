//
// Created by hariharan on 11/6/19.
//

#ifndef CHRONOLOG_CLIENT_H
#define CHRONOLOG_CLIENT_H

#include <basket/queue/queue.h>
#include <boost/interprocess/containers/string.hpp>
#include <boost/thread.hpp>
#include <rpc/client.h>
#include <basket/configuration_manager.h>

namespace bip=boost::interprocess;

namespace Rhea{
    template<class T>
    class Client {
    private:
        basket::queue<T> server_request_queue;
//        std::shared_ptr<RPC> client_rpc;
//        boost::mutex mutex;
        int num_servers;

    public:
        Client(bool requester=true);

        bool PutTask(T task);

        std::pair<bool, T> GetTask(uint16_t server);

        bool GetSize(uint16_t server);
    };
}



#endif //CHRONOLOG_CLIENT_H
