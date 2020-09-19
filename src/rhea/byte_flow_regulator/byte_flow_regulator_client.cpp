//
// Created by neeraj on 9/16/20.
//

#include <rhea/byte_flow_regulator/byte_flow_regulator_client.h>

void rhea::ByteFlowRegulatorClient::Finalize() {}

bool rhea::ByteFlowRegulatorClient::SetInRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetInRate", job_id, in_rate).as<bool>();
    return check;
}

bool rhea::ByteFlowRegulatorClient::SetOutRate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetOutRate", job_id, out_rate).as<bool>();
    return check;
}

