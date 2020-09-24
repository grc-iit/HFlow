//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>



rhea::Client::Client(uint32_t jid, bool is_application): job_id_(jid) {
    RHEA_CONF->ConfigureRheaClient();
    warehouse = std::make_shared<basket::unordered_map<Parcel, bip::string>>();
    write_queue = std::make_shared<basket::queue<Parcel>>("WRITE_QUEUE");
    read_queue = std::make_shared<basket::queue<Parcel>>("READ_QUEUE");
    parsel_state = std::make_shared<basket::unordered_map<Parcel,ParcelState>>("PARSEL_STATE");
    if(is_application){
        if(BASKET_CONF->MPI_RANK == 0)
            basket::Singleton<sentinel::job_manager::client>::GetInstance()->SubmitJob(jid, RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void rhea::Client::FinalizeClient() {
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
        basket::Singleton<sentinel::job_manager::client>::GetInstance()->TerminateJob(job_id_);
}

bool rhea::Client::Publish(Parcel &parcel, char* data) {
    AUTO_TRACER("rhea::Client::Publish", parcel.id_);
    auto data_str =  bip::string(data);
    auto status = true;
    status = status && warehouse->Put(parcel, data_str);
    status = status && write_queue->Push(parcel, BASKET_CONF->MY_SERVER);
    auto parsel_state_val = ParcelState(TaskStatus::QUEUED);
    status = status && parsel_state->Put(parcel, parsel_state_val);
    return status;
}

bool rhea::Client::Subscribe(Parcel &parcel, char *data) {
    AUTO_TRACER("rhea::Client::Subscribe", parcel.id_);
    auto status = true;
    status = status && read_queue->Push(parcel, BASKET_CONF->MY_SERVER);
    std::pair<bool, std::string> result;
    do{
        result = warehouse->Get(parcel);
        if(!result.first) usleep(100);
        else
            memcpy(data, result.second.data(), result.second.size());
    }while(!result.first);
    return result.first;
}

std::vector<Parcel> rhea::Client::GetWriteParsel(uint16_t server_id) {
    float size = write_queue->Size(server_id);
    auto amount = size * .1 == 0? size : size * .1;
    auto parcels = std::vector<Parcel>();
    while(amount > 0){
        auto value = write_queue->Pop(server_id);
        if(value.first) parcels.push_back(value.second);
        else break;
    }
    return parcels;
}

std::vector<Parcel> rhea::Client::GetReadParsel(uint16_t server_id) {
    float size = read_queue->Size(server_id);
    auto amount = size * .1 == 0? size : size * .1;
    auto parcels = std::vector<Parcel>();
    while(amount > 0){
        auto value = read_queue->Pop(server_id);
        if(value.first) parcels.push_back(value.second);
        else break;
    }
    return parcels;
}

std::string rhea::Client::GetData(Parcel &parcel) {
    AUTO_TRACER("rhea::Client::GetData", parcel);
    auto result = warehouse->Get(parcel);
    return result.second.c_str();
}

bool rhea::Client::DeleteData(Parcel &parcel) {
    AUTO_TRACER("rhea::Client::DeleteData", parcel);
    auto result = warehouse->Erase(parcel);
    return result.first;
}

bool rhea::Client::PutData(Parcel &parcel, char *data) {
    AUTO_TRACER("rhea::Client::PutData", parcel);
    auto data_str = bip::string(data, parcel.data_size_);
    return warehouse->Put(parcel,data_str);
}

bool rhea::Client::UpdateParcelStatus(Parcel &parcel, TaskStatus status) {
    auto parsel_state_val = ParcelState(status);
    return parsel_state->Put(parcel, parsel_state_val);
}

std::vector<ParcelState> rhea::Client::WaitAll(vector<Parcel> &parcels) {
    auto parcel_states = std::vector<ParcelState>();
    for(auto& parcel:parcels) parcel_states.push_back(Wait(parcel));
    return parcel_states;
}

ParcelState rhea::Client::Wait(Parcel &parcel) {
    bool is_completed=false;
    std::pair<bool, ParcelState> result;
    do{
        result = parsel_state->Get(parcel);
        is_completed = result.first && result.second.status_ == TaskStatus::DONE;
        if(!is_completed) usleep(100);
        else
            return result.second;
    }while(!is_completed);
    return result.second;
}
