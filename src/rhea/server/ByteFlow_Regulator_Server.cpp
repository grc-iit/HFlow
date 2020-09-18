//
// Created by mani on 9/14/2020.
//

#include "rhea/server/ByteFlow_Regulator_Server.h"

void rhea::ByteFlow_Regulator_Server::Run() {
	std::promise<void> exit_signal;
	std::future<void> future_obj = exit_signal.get_future();
	std::thread t_thread(&rhea::ByteFlow_Regulator_Server::RunInternal, this, std::move(future_obj));
	t_thread.join();
}

void rhea::ByteFlow_Regulator_Server::SetInRate(uint16_t job_id, uint_fast32_t in_rate){
    in_rate_map[job_id] = in_rate;
}

void rhea::ByteFlow_Regulator_Server::SetOutRate(uint16_t job_id, uint_fast32_t out_rate) {
    out_rate_map[job_id] = out_rate;
}

bool rhea::ByteFlow_Regulator_Server::AlterCollector(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	// Left here as place holder for further expansion
	return 1;
}
bool rhea::ByteFlow_Regulator_Server::AlterTransformers(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
    // Left here as place holder for further expansion
	return 1;
}
bool rhea::ByteFlow_Regulator_Server::AlterNodes(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
    int8_t multiplier = static_cast<int8_t>(out_rate > in_rate ? Alter_Type::SHRINK : Alter_Type::GROW);
	auto difference = abs((int)(out_rate - in_rate)) - (variation/2);
	uint16_t node_var = difference*multiplier/step;
	// TODO: add more alterations based
	ResourceAllocation resources(job_id, node_var ,0, 0);
	auto ret = basket::Singleton<sentinel::job_manager::client>::GetInstance()->ChangeResourceAllocation(resources);
//	return ret;
    return true;
}

void rhea::ByteFlow_Regulator_Server::RunInternal(std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::microseconds(interval)) == std::future_status::timeout) {
        // iterate over map and get the in and out flows and do fun shit!

        for (auto &i : in_rate_map ) {
            auto out_map_find = out_rate_map.find(i.first);
            if (out_map_find != out_rate_map.end()){
                auto in_rate = i.second;
                auto out_rate = out_map_find->second;
                if (abs((int) (out_rate - in_rate)) >= variation) {
                    auto ret = AlterNodes(i.first, out_rate, in_rate);
                }
            }
        }
    }
}

void rhea::ByteFlow_Regulator_Server::RPCInit() {

    std::shared_ptr<RPC> client_rpc_;
    std::function<void(uint16_t, uint_fast32_t)> functionSetInRate(std::bind(&rhea::ByteFlow_Regulator_Server::SetInRate, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc_->bind("SetInRate", functionSetInRate);

    std:function<void(uint16_t, uint_fast32_t)> functionSetOutRate(std::bind(&rhea::ByteFlow_Regulator_Server::SetOutRate, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc_->bind("setOutRate", functionSetOutRate);

}

void rhea::ByteFlow_Regulator_Server::ConfigInit() {
    SENTINEL_CONF->ConfigureByteflowRegulatorServer();
    auto basket=BASKET_CONF;
}


