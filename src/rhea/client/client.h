//
// Created by neeraj on 9/17/20.
//

#ifndef RHEA_BYTEFLOW_REGULATOR_CLIENT_H
#define RHEA_BYTEFLOW_REGULATOR_CLIENT_H 

#include <basket.h>
#include <basket/communication/rpc_factory.h>
#include <memory>
#include <sentinel/job_manager/client.h>
#include <rpc/client.h>
#include <basket.h>
#include <string>

namespace rhea{
        class Client {
        private:
            std::shared_ptr<RPC> server_rpc;
            void ConfigInit();
            void RPCInit();
        public:
            Client();
            bool SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate);
            bool setOutRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate);
            void Finalize();

            Client(Client &other){
                server_rpc = other.server_rpc;
                ConfigInit();
                RPCInit();
            }
            Client &operator=(const Client &other){
                server_rpc = other.server_rpc;
                ConfigInit();
                RPCInit();
                return *this;
            }



        };
};

#endif