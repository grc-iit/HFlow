//
// Created by mani on 9/14/2020.
//

#include "server.h"

void rhea::server::Run() {
	std::promise<void> exit_signal;
	std::future<void> future_obj = exit_signal.get_future();
	std::thread t_thread(&rhea::server::RunInternal, this, std::move(future_obj));
	// promises.push_back(std::move(exit_signal));
	// pool.push_back(std::move(t_thread));

	t_thread.join();
}

void rhea::server::set_in_rate(uint16_t job_id, uint_fast32_t in_rate){
    in_rate_map[job_id] = in_rate;
}

void rhea::server::set_out_rate(uint16_t job_id, uint_fast32_t out_rate){
    out_rate_map[job_id] = out_rate;
}

bool rhea::server::AlterCollector(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	// doOp
	return 1;
}
bool rhea::server::AlterTransformers(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
	return 1;
}
bool rhea::server::AlterWriters(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
    int8_t multiplier = static_cast<int8_t>(out_rate > in_rate ? Alter_Type::SHRINK : Alter_Type::GROW);
	auto difference = abs((int)(out_rate - in_rate)) - (variation/2);
	uint16_t node_var = difference*multiplier/step;
	// TODO: add more alterations based
	ResourceAllocation resources(job_id, node_var ,0, 0);
//	auto ret = basket::Singleton<sentinel::job_manager::client>::GetInstance()->ChangeResourceAllocation(resources);
//	return ret;
    return true;
}

/**
 * TODO: follow other server conventions. we call this method RunInternal.
 * This will be a private function called by Run method.
 */
void rhea::server::RunInternal(std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::microseconds(interval)) == std::future_status::timeout) {
        // iterate over map and get the in and out flows and do fun shit!

        for (auto &i : in_rate_map ) {
            auto out_map_find = out_rate_map.find(i.first);
            if (out_map_find != out_rate_map.end()){
                auto in_rate = i.second;
                auto out_rate = out_map_find->second;
                if (abs((int) (out_rate - in_rate)) >= variation) {
                    /**
                     *
                     * TODO: create a object of request allocation. where u will say how many more or less.
                     * Get Current Allocation Status of Sentinel (n nodes)
                     * if( in_rate < out_rate) increase node proportionally
                     * grow or shink
                     *
                     */
                    auto ret = AlterWriters(i.first, out_rate, in_rate);
                }
            }
        }
    }
}

void rhea::server::RPCInit() {

    std::shared_ptr<RPC> client_rpc_;
    std::function<void(uint16_t, uint_fast32_t)> functionSetInRate(std::bind(&rhea::server::set_in_rate, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc_->bind("set_in_rate", functionSetInRate);

    std:function<void(uint16_t, uint_fast32_t)> functionSetOutRate(std::bind(&rhea::server::set_out_rate, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc_->bind("set_out_rate", functionSetOutRate);

}


