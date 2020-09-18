//
// Created by neeraj on 9/16/20.
//

#include "rhea/client/client.h"

rhea::ByteFlow_Regulator_Client::ByteFlow_Regulator_Client() {
    ConfigInit();
    RPCInit();
}

void rhea::ByteFlow_Regulator_Client::RPCInit() {
    this->server_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
}

void rhea::ByteFlow_Regulator_Client::Finalize() {}

bool rhea::ByteFlow_Regulator_Client::SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetInRate", job_id, in_rate).as<bool>();
    return check;
}

bool rhea::ByteFlow_Regulator_Client::SetOutRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetOutRate", job_id, out_rate).as<bool>();
    return check;
}

void rhea::ByteFlow_Regulator_Client::ConfigInit() {
    SENTINEL_CONF->ConfigureByteflowRegulatorClient();
    auto basket=BASKET_CONF;
}

