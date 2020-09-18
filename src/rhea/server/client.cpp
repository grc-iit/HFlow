//
// Created by neeraj on 9/16/20.
//

#include "client.h"

// sentinel::worker_manager::Client::Client():server_rpc() {
    // SENTINEL_CONF->ConfigureWorkermanagerClient();
    // Init();
// }

void rhea::Client::RPCInit() {
    this->server_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
}

void rhea::Client::Finalize() {}

bool rhea::Client::set_in_rate(uint8_t server_index_, uint16_t job_id, uint_fast64_t in_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "set_in_rate", job_id, in_rate).as<bool>();
    return check;
}

bool rhea::Client::set_out_rate(uint8_t server_index_, uint16_t job_id, uint_fast64_t out_rate) {
    auto check = server_rpc->call<RPCLIB_MSGPACK::object_handle>(server_index_, "set_out_rate", job_id, out_rate).as<bool>();
    return check;
}

