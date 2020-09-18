//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>



void rhea::ByteflowPusher::Run(std::future<void> futureObj) {
    client = std::make_shared<rhea::ByteFlow_Regulator_Client>();
    RunInternal(std::move(futureObj));
}

void rhea::ByteflowPusher::RunInternal(std::future<void> futureObj) {
    std::shared_ptr<basket::unordered_map<uint32_t, size_t>> rhea_size_map = std::make_shared<basket::unordered_map<uint32_t, size_t>>("test_client"); // TODO make configurable
    while(futureObj.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout){
        for (auto jobinfo : rhea_size_map->GetAllData()) {
            client->SetInRate(0, jobinfo.first, jobinfo.second/(100*0.01);
            // client->SetOutRate(0, );
        }
    }
}

rhea::Client::Client() {
    sizes = std::make_shared<basket::unordered_map<uint32_t, size_t>>("test_client"); // TODO make configurable
    CharStruct log = "./single_node_rhea_queue.log";
    auto daemon = basket::Singleton<common::Daemon<rhea::ByteflowPusher>>::GetInstance(log);
    daemon->Run();
}

rhea::Client::~Client() {

}

bool rhea::Client::SubmitJob(uint32_t jid) {
    size_t new_size = 0;
    sizes->Put(jid, new_size);
    return basket::Singleton<sentinel::job_manager::client>::GetInstance()->SubmitJob(jid);
}

bool rhea::Client::TerminateJob(uint32_t jid) {
    size_t new_size = 0;
    sizes->Put(jid, new_size);
    return basket::Singleton<sentinel::job_manager::client>::GetInstance()->TerminateJob(jid);
}

bool rhea::Client::CreateSource(std::string name) {
    // HCL queues
    sources[name] = std::make_shared<basket::queue<Parcel>>(name);
    return true;
}

bool rhea::Client::DeleteSource(std::string name) {
    sources[name] = nullptr;
    return true;
}

bool rhea::Client::Publish(std::string srcid, CharStruct message, uint32_t jid) {
    auto job_info = sizes->Get(jid);
    auto new_size = job_info.second + message.size();
    sizes->Put(jid, new_size);
    auto package = Parcel(message, 0, message.data(), 0, message.size());
    unsigned short key = 0;
    sources[srcid]->Push(package, key);
    return true;
}

bool rhea::Client::CreateSink(CharStruct name) {
    return false;
}

bool rhea::Client::DeleteSink(CharStruct name) {
    return false;
}

bool rhea::Client::SubscribeSink(CharStruct srcid) {
    return false;
}

bool rhea::Client::UnsubscribeSink(CharStruct srcid) {
    return false;
}