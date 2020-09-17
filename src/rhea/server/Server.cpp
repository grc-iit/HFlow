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

uint_fast64_t rhea::server::get_in_rate(){
	return 10;
}

uint_fast64_t rhea::server::get_out_rate(){
	return 10;
}

bool rhea::server::AlterCollector(uint_fast64_t out_rate, uint_fast64_t in_rate){
	// doOp
	return 1;
}
bool rhea::server::AlterTransformers(uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
	return 1;
}
bool rhea::server::AlterWriters(uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
	int multiplier = out_rate > in_rate ? -1 : 1;
	auto difference = abs((int)(out_rate - in_rate)) - (variation/2);
	uint16_t node_var = difference*multiplier/step;
	ResourceAllocation resources(node_var ,0, 0);
	auto ret = basket::Singleton<sentinel::job_manager::client>::GetInstance()->ChangeResourceAllocation(resources);
	return ret;
}

/**
 * TODO: follow other server conventions. we call this method RunInternal.
 * This will be a private function called by Run method.
 */
void rhea::server::RunInternal(std::future<void> futureObj) {
	while (futureObj.wait_for(std::chrono::microseconds(interval)) == std::future_status::timeout) {
		auto in_rate = get_in_rate();
		auto out_rate = get_out_rate();
		if (abs((int)(out_rate - in_rate)) >= variation){
		    /**
		     *
		     * TODO: create a object of request allocation. where u will say how many more or less.
		     * Get Current Allocation Status of Sentinel (n nodes)
		     * if( in_rate < out_rate) increase node proportionally
		     * grow or shink
		     *
		     */
                auto ret = AlterWriters(out_rate, in_rate);
			}
		}
	}
}

