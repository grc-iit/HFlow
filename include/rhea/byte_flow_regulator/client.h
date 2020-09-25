//
// Created by neeraj on 9/17/20.
//

#ifndef RHEA_CLIENT_H
#define RHEA_CLIENT_H

#include <basket/communication/rpc_factory.h>
#include <sentinel/common/configuration_manager.h>
#include <rpc/client.h>
#include <rhea/common/configuration_manager.h>

namespace rhea::byteflow_regulator{
        class Client {
        private:
            std::shared_ptr<RPC> server_rpc;
        public:
            Client(){
                RHEA_CONF->ConfigureByteflowRegulatorClient();
                this->server_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
            }
            bool SetInRate(JobId job_id, uint32_t in_rate);
            bool SetOutRate(JobId job_id, uint32_t out_rate);
            bool RegisterJob(JobId job_id);
            bool Finalize(JobId job_id);
        };
};

#endif