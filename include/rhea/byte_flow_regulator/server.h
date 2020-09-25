//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_SERVER_H
#define RHEA_SERVER_H

#include <cstdint>
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <sentinel/job_manager/client.h>
#include <basket/communication/rpc_factory.h>
#include <rpc/client.h>
#include <rhea/common/configuration_manager.h>
#include <common/daemon.h>
#include <rhea/common/enumerations.h>


namespace rhea::byteflow_regulator{

    class Server {
    private:
        // make all these maps against job_id
        std::shared_ptr<RPC> client_rpc_;
        mutable std::shared_mutex in_mutex, out_mutex;
        std::unordered_map<uint16_t, uint_fast32_t> in_rate_map;
        std::unordered_map<uint16_t, uint_fast32_t> out_rate_map;
        bool AlterCollector(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate);
        bool AlterTransformers(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate);
        bool AlterNodes(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate);
        void RunInternal(std::future<void> futureObj);
        common::Daemon<Server> * daemon;
    public:
        void Run(std::future<void> futureObj, common::Daemon<Server>* obj);
        bool SetInRate(JobId job_id, uint32_t in_rate);
        bool SetOutRate(JobId job_id, uint32_t out_rate);
        bool RegisterJob(JobId job_id);
        bool FinalizeJob(JobId job_id);
        Server(){
            RHEA_CONF->ConfigureByteflowRegulatorServer();
            auto basket=BASKET_CONF;
            client_rpc_=basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
            std::function<bool(JobId, uint32_t)> functionSetInRate(std::bind(&rhea::byteflow_regulator::Server::SetInRate, this, std::placeholders::_1, std::placeholders::_2));
            std:function<bool(JobId, uint32_t)> functionSetOutRate(std::bind(&rhea::byteflow_regulator::Server::SetOutRate, this, std::placeholders::_1, std::placeholders::_2));
            function<bool(JobId)> functionRegisterJob(std::bind(&rhea::byteflow_regulator::Server::RegisterJob, this, std::placeholders::_1));
            function<bool(JobId)> functionFinalizeJob(std::bind(&rhea::byteflow_regulator::Server::FinalizeJob, this, std::placeholders::_1));
            client_rpc_->bind("SetInRate", functionSetInRate);
            client_rpc_->bind("SetOutRate", functionSetOutRate);
            client_rpc_->bind("RegisterJob", functionRegisterJob);
            client_rpc_->bind("FinalizeJob", functionFinalizeJob);
        }
    };
}

#endif //RHEA_JOB_MANAGER_H
