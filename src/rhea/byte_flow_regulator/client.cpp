//
// Created by neeraj on 9/16/20.
//

#include <rhea/byte_flow_regulator/client.h>

bool rhea::byteflow_regulator::Client::Finalize(JobId job_id) {
    uint8_t server_index_=0;
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "FinalizeJob", job_id).as<bool>();
    return check;
}

bool rhea::byteflow_regulator::Client::RegisterJob(JobId job_id) {
    uint8_t server_index_=0;
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "RegisterJob", job_id).as<bool>();
    return check;
}

bool rhea::byteflow_regulator::Client::SetInRate(JobId job_id, uint32_t in_rate) {
    uint8_t server_index_=0;
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetInRate", job_id, in_rate).as<bool>();
    return check;
}

bool rhea::byteflow_regulator::Client::SetOutRate(JobId job_id, uint32_t out_rate) {
    uint8_t server_index_=0;
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "SetOutRate", job_id, out_rate).as<bool>();
    return check;
}

