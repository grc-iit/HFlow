//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>



rhea::Client::Client(uint32_t jid, bool is_application): job_id_(jid) {
    RHEA_CONF->ConfigureRheaClient();
    warehouse = std::make_shared<basket::unordered_map<CharStruct, bip::string>>();
    queue = std::make_shared<basket::queue<Parcel>>();
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
    auto data_str =  bip::string(data);
    auto status = true;
    status = status && warehouse->Put(parcel.id_, data_str);
    status = status && queue->Push(parcel,BASKET_CONF->MY_SERVER);
    return status;
}

bool rhea::Client::Subscribe(Parcel &parcel, char *data) {
    auto status = true;
    status = status && queue->Push(parcel,BASKET_CONF->MY_SERVER);
    std::pair<bool, std::string> result;
    do{
        result = warehouse->Get(parcel.id_);
        if(!result.first) usleep(100);
        else memcpy(data, result.second.data(), result.second.size());
    }while(!result.first);
    return result.first;
}

std::vector<Parcel> rhea::Client::GetParsel(uint16_t server_id) {
    float size = queue->Size(server_id);
    auto amount = size * .1 == 0? size : size * .1;
    auto parcels = std::vector<Parcel>();
    while(amount > 0){
        auto value = queue->Pop(server_id);
        if(value.first) parcels.push_back(value.second);
        else break;
    }
    return parcels;
}

std::string rhea::Client::GetData(Parcel &parcel) {
    auto result = warehouse->Get(parcel.id_);
    return result.second.c_str();
}

bool rhea::Client::DeleteData(Parcel &parcel) {
    auto result = warehouse->Erase(parcel.id_);
    return result.first;
}
