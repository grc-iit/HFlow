//
// Created by mani on 9/14/2020.
//

#include <rhea/rhea.h>



rhea::Client::Client(uint32_t jid):jid(jid) {
    RHEA_CONF->ConfigureRheaClient();
    auto warehouse = std::shared_ptr<basket::unordered_map<uint32_t, size_t>>();
    auto queue = std::shared_ptr<basket::queue<Parcel>>();
    if(BASKET_CONF->MPI_RANK == 0)
        basket::Singleton<sentinel::job_manager::client>::GetInstance()->SubmitJob(jid);
    MPI_Barrier(MPI_COMM_WORLD);
}

rhea::Client::~Client() {
    MPI_Barrier(MPI_COMM_WORLD);
    if(BASKET_CONF->MPI_RANK == 0)
        basket::Singleton<sentinel::job_manager::client>::GetInstance()->TerminateJob(jid);
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
