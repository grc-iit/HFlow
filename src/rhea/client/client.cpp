//
// Created by neeraj on 9/16/20.
//

#include "rhea/client/client.h"

rhea::Client::Client() {
    ConfigInit();
    RPCInit();
}

void rhea::Client::RPCInit() {
    this->server_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
}

void rhea::Client::Finalize() {}

bool rhea::Client::SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetInRate", job_id, in_rate).as<bool>();
    return check;
}

bool rhea::Client::setOutRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "setOutRate", job_id, out_rate).as<bool>();
    return check;
}

void rhea::Client::ConfigInit() {
    SENTINEL_CONF->ConfigureByteflowRegulatorClient();
    auto basket=BASKET_CONF;
}

