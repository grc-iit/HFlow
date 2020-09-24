//
// Created by hdevarajan on 9/18/20.
//

#ifndef RHEA_RHEA_CLIENT_H
#define RHEA_RHEA_CLIENT_H

#include <basket/unordered_map/unordered_map.h>
#include <basket/queue/queue.h>
#include <rhea/common/data_structures.h>
#include <rhea/common/configuration_manager.h>
#include <common/daemon.h>

namespace bip=boost::interprocess;

namespace rhea{
    class RheaClient {
    private:
        basket::unordered_map<CharStruct,bip::string> warehouse;
        basket::unordered_map<Parcel,ParcelState> parsel_state;
        basket::queue<Parcel> write_queue,read_queue;
        common::Daemon<RheaClient> * daemon;
    public:
        RheaClient():warehouse(),write_queue("WRITE_QUEUE"),read_queue("READ_QUEUE"),parsel_state("PARSEL_STATE"){}
        void Run(std::future<void> futureObj,common::Daemon<RheaClient> * obj);
        void RunInternal(std::future<void> futureObj);
    };
}



#endif //RHEA_RHEA_CLIENT_H
