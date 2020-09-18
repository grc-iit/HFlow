//
// Created by neeraj on 9/17/20.
//

#ifndef RHEA_BYTEFLOW_REGULATOR_CLIENT_H
#define RHEA_BYTEFLOW_REGULATOR_CLIENT_H 

#include <basket.h>
#include <memory>
#include <basket/communication/rpc_factory.h>
#include <rpc/client.h>
#include <basket.h>
#include <string>

namespace rhea{
        class Client {
        private:
            std::shared_ptr<RPC> server_rpc;
            void RPCInit();
        public:
            Client();
            bool set_in_rate(uint8_t server_index_, uint16_t ,uint_fast64_t);
            bool set_out_rate(uint8_t server_index_, uint16_t ,uint_fast64_t);
            void Finalize();
        };
};

#endif