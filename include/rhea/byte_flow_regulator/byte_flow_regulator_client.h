//
// Created by neeraj on 9/17/20.
//

#ifndef RHEA_BYTE_FLOW_REGULATOR_CLIENT_H
#define RHEA_BYTE_FLOW_REGULATOR_CLIENT_H

#include <basket/communication/rpc_factory.h>
#include <sentinel/common/configuration_manager.h>
#include <rpc/client.h>
#include <rhea/common/configuration_manager.h>

namespace rhea{
        class ByteFlowRegulatorClient {
        private:
            std::shared_ptr<RPC> server_rpc;
        public:
            ByteFlowRegulatorClient(){
                // TODO: first initial configuration
                RHEA_CONF->ConfigureByteflowRegulatorClient();
                this->server_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
                //RHEA_CONF->ConfigureByteflowRegulatorClient();
                //auto basket=BASKET_CONF;
            }
            //TODO: For the second parameter type, client use uint_fast64_t, server-side use uint_fast32_t. Which one do we use?
            bool SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast32_t in_rate);
            bool SetOutRate(uint8_t server_index_, uint16_t job_id, uint_fast32_t out_rate);
            void Finalize();
        };
};

#endif