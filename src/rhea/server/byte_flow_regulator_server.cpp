//
// Created by mani on 9/14/2020.
//

#include <rhea/server/byte_flow_regulator_server.h>

void rhea::ByteFlowRegulatorServer::Run(std::future<void> futureObj,common::Daemon<ByteFlowRegulatorServer>* obj) {
    daemon = obj;
    RunInternal(std::move(futureObj));
}

void rhea::ByteFlowRegulatorServer::SetInRate(uint16_t job_id, uint_fast32_t in_rate){
    in_rate_map[job_id] = in_rate;
}

void rhea::ByteFlowRegulatorServer::SetOutRate(uint16_t job_id, uint_fast32_t out_rate) {
    out_rate_map[job_id] = out_rate;
}

bool rhea::ByteFlowRegulatorServer::AlterCollector(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	// Left here as place holder for further expansion
	return 1;
}
bool rhea::ByteFlowRegulatorServer::AlterTransformers(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
    // Left here as place holder for further expansion
	return 1;
}
bool rhea::ByteFlowRegulatorServer::AlterNodes(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate){
	//doOp
    int8_t multiplier = static_cast<int8_t>(out_rate > in_rate ? Alter_Type::SHRINK : Alter_Type::GROW);
	auto difference = abs((int)(out_rate - in_rate)) - (variation/2);
	uint16_t node_var = difference*multiplier/step;
	// TODO: add more alterations based
	ResourceAllocation resources(job_id, node_var ,0, 0);
	auto jobmanager_client = basket::Singleton<sentinel::job_manager::client>::GetInstance();
    return jobmanager_client->ChangeResourceAllocation(resources);
}

void rhea::ByteFlowRegulatorServer::RunInternal(std::future<void> futureObj) {
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




