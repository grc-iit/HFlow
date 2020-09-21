//
// Created by hdevarajan on 9/18/20.
//

#include <rhea/client_service/rhea_client.h>
#include <rhea/common/configuration_manager.h>

void rhea::RheaClient::Run(std::future<void> futureObj,common::Daemon<RheaClient> * obj) {
    daemon = obj;
    RunInternal(std::move(futureObj));
}

void rhea::RheaClient::RunInternal(std::future<void> futureObj) {
    while(futureObj.wait_for(std::chrono::milliseconds(RHEA_CONF->BYTEFLOW_STAT_PUSH_INTERVAL)) == std::future_status::timeout){}
}
