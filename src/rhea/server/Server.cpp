//
// Created by mani on 9/14/2020.
//

#include "server.h"

void rhea::server::run() {
	std::promise<void> exit_signal;
	std::future<void> future_obj = exit_signal.get_future();
	std::thread t_thread(&rhea::server::single_loop, this, std::move(future_obj));
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

int rhea::server::alter_collector(rhea::Alter_Type){
	// doOp
	return 1;
}
int rhea::server::alter_transformers(rhea::Alter_Type){
	//doOp
	return 1;
}
int rhea::server::alter_writers(rhea::Alter_Type){
	//doOp
	return 1;
}
/**
 * TODO: follow other server conventions. we call this method RunInternal.
 * This will be a private function called by Run method.
 */
void rhea::server::single_loop(std::future<void> futureObj) {
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
			if (in_rate > out_rate){
				auto op = Alter_Type::GROW;
				auto ret = alter_writers(op);
			}
			else if (out_rate >  in_rate){
				auto op = Alter_Type::SHRINK;
				auto ret = alter_writers(op);
			}
		}
	}
}

