//
// Created by hariharan on 11/6/19.
//

#include "client/client.h"


template<typename T>
Rhea::Client<T>::Client(bool requester): server_request_queue("SERVER_REQUEST_QUEUE"),
                                         num_servers(BASKET_CONF->NUM_SERVERS) {
    RHEA_CONF->ConfigureQueueClient();
//        uint16_t port_=RHEA_CONF->QUEUE_CLIENT_PORT+BASKET_CONF->MPI_RANK;
//        client_rpc=basket::Singleton<RPCFactory>::GetInstance()->GetRPC(port_);
//        std::function<int(PlaybackEvent&,std::vector<std::pair<Event, std::string>>&)> functionGetRangeCallBack(std::bind(&Rhea::Client::GetRangeCallBack,this,
//                std::placeholders::_1,std::placeholders::_2));
//        client_rpc->bind("GET_RANGE_CALLBACK",functionGetRangeCallBack);
}

template<typename T>
std::pair<bool, T> Rhea::Client<T>::GetTask(uint16_t server) {
    return server_request_queue.Pop(server);
}

template<typename T>
bool Rhea::Client<T>::PutTask(T task) {
    return server_request_queue.Push(task, task.task_origin);
}

template<typename T>
bool Rhea::Client<T>::GetSize(uint16_t server) {
    return server_request_queue.Size(server);
}

