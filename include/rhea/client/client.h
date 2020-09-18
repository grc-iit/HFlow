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
        class ByteFlow_Regulator_Client {
        private:
            std::shared_ptr<RPC> server_rpc;
            void ConfigInit();
            void RPCInit();
        public:
            ByteFlow_Regulator_Client();
            bool SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate);
            bool SetOutRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate);
            void Finalize();

            ByteFlow_Regulator_Client(ByteFlow_Regulator_Client &other){
                server_rpc = other.server_rpc;
                ConfigInit();
                RPCInit();
            }
            ByteFlow_Regulator_Client &operator=(const ByteFlow_Regulator_Client &other){
                server_rpc = other.server_rpc;
                ConfigInit();
                RPCInit();
                return *this;
            }



        };
};

#endif