//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>



rhea::Client::Client(uint32_t jid, bool is_application): job_id_(jid) {
    RHEA_CONF->ConfigureRheaClient();
    warehouse = std::make_shared<basket::unordered_map<CharStruct, bip::string>>();
    write_queue = std::make_shared<basket::queue<Parcel>>("WRITE_QUEUE");
    read_queue = std::make_shared<basket::queue<Parcel>>("READ_QUEUE");
    if(is_application){
        if(BASKET_CONF->MPI_RANK == 0)
            basket::Singleton<sentinel::job_manager::client>::GetInstance()->SubmitJob(jid, RHEA_CONF->RHEA_CLIENT_SERVICE_COUNT);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void rhea::Client::FinalizeClient() {
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
//        basket::Singleton<sentinel::job_manager::client>::GetInstance()->TerminateJob(job_id_);
        int i =0;
}

bool rhea::Client::Publish(Parcel &parcel, char* data) {
    auto data_str =  bip::string(data);
    auto status = true;
    auto key = CharStruct(parcel.id_.data() + std::to_string(parcel.position_));
    AUTO_TRACER("rhea::Client::Publish", key);
    status = status && warehouse->Put(key, data_str);
    status = status && write_queue->Push(parcel, BASKET_CONF->MY_SERVER);
    return status;
}

bool rhea::Client::Subscribe(Parcel &parcel, char *data) {
    auto status = true;
    status = status && read_queue->Push(parcel, BASKET_CONF->MY_SERVER);
    std::pair<bool, std::string> result;
    do{
        auto key = CharStruct(parcel.id_.data());
        AUTO_TRACER("rhea::Client::Subscribe", key);
        result = warehouse->Get(key);
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
    auto key = CharStruct(parcel.id_.data() + std::to_string(parcel.position_));
    AUTO_TRACER("rhea::Client::GetData", key);
    auto result = warehouse->Get(key);
    return result.second.c_str();
}

bool rhea::Client::DeleteData(Parcel &parcel) {
    auto key = CharStruct(parcel.id_.data() + std::to_string(parcel.position_));
    AUTO_TRACER("rhea::Client::DeleteData", key);
    auto result = warehouse->Erase(key);
    return result.first;
}

bool rhea::Client::PutData(Parcel &parcel, char *data) {
    auto key = CharStruct(parcel.id_.data() + std::to_string(parcel.position_));
    AUTO_TRACER("rhea::Client::PutData", key);
    auto data_str = bip::string(data, parcel.data_size_);
    return warehouse->Put(key,data_str);
}
