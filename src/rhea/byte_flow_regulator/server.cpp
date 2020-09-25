//
// Created by mani on 9/14/2020.
//

#include <rhea/byte_flow_regulator/server.h>


void rhea::byteflow_regulator::Server::Run(std::future<void> futureObj, common::Daemon<Server>* obj) {
    daemon = obj;
    RunInternal(std::move(futureObj));
}

bool rhea::byteflow_regulator::Server::RegisterJob(JobId job_id) {
    std::unique_lock in_lock(in_mutex);
    in_rate_map[job_id] = 0;
    in_lock.unlock();
    std::unique_lock out_lock(out_mutex);
    out_rate_map[job_id] = 0;
    out_lock.unlock();
    return true;
}


bool rhea::byteflow_regulator::Server::FinalizeJob(JobId job_id) {
    std::unique_lock in_lock(in_mutex);
    in_rate_map[job_id] = 0;
    in_lock.unlock();
    std::unique_lock out_lock(out_mutex);
    out_rate_map[job_id] = 0;
    out_lock.unlock();
    return true;
}

bool rhea::byteflow_regulator::Server::SetInRate(JobId job_id, uint32_t in_rate){
    AUTO_TRACER("rhea::ByteFlowRegulatorServer::SetInRate");

    std::unique_lock in_lock(in_mutex);
    in_rate_map[job_id] = in_rate;
    in_lock.unlock();
    return true;
}

bool rhea::byteflow_regulator::Server::SetOutRate(JobId job_id, uint32_t out_rate) {
    AUTO_TRACER("rhea::ByteFlowRegulatorServer::SetOutRate");
    std::unique_lock out_lock(out_mutex);
    out_rate_map[job_id] = out_rate;
    out_lock.unlock();
    return true;
}

bool rhea::byteflow_regulator::Server::AlterCollector(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate){
	// Left here as place holder for further expansion
	return true;
}
bool rhea::byteflow_regulator::Server::AlterTransformers(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate){
    // Left here as place holder for further expansion
	return true;
}
bool rhea::byteflow_regulator::Server::AlterNodes(uint16_t job_id, uint_fast32_t out_rate, uint_fast32_t in_rate){
	//doOp
    int8_t multiplier = static_cast<int8_t>(out_rate > in_rate ? Alter_Type::SHRINK : Alter_Type::GROW);
	auto difference = abs((int)(out_rate - in_rate)) - (RHEA_CONF->BYTEFLOW_UPDATE_VARIATION/2);
	int16_t node_var = difference*multiplier/RHEA_CONF->BYTEFLOW_UPDATE_STEP;
	// TODO: add more alterations based
	ResourceAllocation resources(job_id, node_var ,0, 0);
	auto jobmanager_client = basket::Singleton<sentinel::job_manager::client>::GetInstance();
    return jobmanager_client->ChangeResourceAllocation(resources);
}

void rhea::byteflow_regulator::Server::RunInternal(std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::microseconds(RHEA_CONF->BYTEFLOW_UPDATE_INTERVAL)) == std::future_status::timeout) {
        CalculateRate();
    }
}

bool rhea::byteflow_regulator::Server::CalculateRate() {
    auto in_jobs = std::vector<JobId>();
    std::shared_lock in_lock(in_mutex);
    in_jobs.reserve(in_rate_map.size());
    for(const auto& kv : in_rate_map) {
        in_jobs.push_back(kv.first);
    }
    in_lock.unlock();
    for(const auto& job:in_jobs){
        std::shared_lock in_lock(in_mutex);
        std::shared_lock out_lock(in_mutex);
        auto in_rate = in_rate_map[job];
        auto out_rate = out_rate_map[job];
        out_lock.unlock();
        in_lock.unlock();
        if(in_rate == 0 || out_rate == 0) continue;
        if (abs((int) (out_rate - in_rate)) >= RHEA_CONF->BYTEFLOW_UPDATE_VARIATION) {
            auto ret = AlterNodes(job, out_rate, in_rate);
        }
    }
    return true;
}







