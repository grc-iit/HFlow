//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>

rhea::Client::Client() {

}

rhea::Client::~Client() {

}

bool rhea::Client::SubmitJob(uint32_t jid) {
    return basket::Singleton<sentinel::job_manager::client>::GetInstance()->SubmitJob(jid);
}

bool rhea::Client::TerminateJob(uint32_t jid) {
    return basket::Singleton<sentinel::job_manager::client>::GetInstance()->TerminateJob(jid);
}

bool rhea::Client::CreateSource(CharStruct name) {
    basket::Singleton<sentinel::worker_manager::Client>::GetInstance()->Init();
    return false;
}

bool rhea::Client::DeleteSource(CharStruct name) {
    basket::Singleton<sentinel::worker_manager::Client>::GetInstance()->Finalize();
    return false;
}

bool rhea::Client::Publish(CharStruct src, CharStruct message) {
    return false;
}

bool rhea::Client::CreateSink(CharStruct name) {
    return false;
}

bool rhea::Client::DeleteSink(CharStruct name) {
    return false;
}

bool rhea::Client::SubscribeSink(CharStruct src) {
    return false;
}

bool rhea::Client::UnsubscribeSink(CharStruct src) {
    return false;
}