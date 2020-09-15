//
// Created by hariharan on 11/6/19.
//


#include "server/server.h"

template<class T>
Rhea::Server<T>::Server():server_request_queue("SERVER_REQUEST_QUEUE", RHEA_CONF->QUEUE_SERVER_PORT) {

//    server_rpc=server_request_queue.rpc;
//    std::function<TaskEvent(void)> functionGetTask(std::bind(&Rhea::Server::GetTask,this));
//    std::function<bool(TaskEvent)> functionPutTask(std::bind(&Rhea::Server::PutTask,this, std::placeholders::_1));
//    server_rpc->bind("GetTaskServerLocal", functionGetTask);
//    server_rpc->bind("PutTaskServerRemote", functionGetTask);
}

template<class T>
void Rhea::Server<T>::RunInternal(std::future<void> futureObj) {
    bool count=true;
    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
        usleep(10000);
        if(count){
            if(BASKET_CONF->MPI_RANK == 0) printf("Started the Chronoplayer server\n");
            count=false;
        }
    }
}

//TaskEvent Rhea::Server::GetTask() {
//    auto num_elements =  server_request_queue.LocalSize();
//    if(num_elements > 0) {
//        auto ele = server_request_queue.LocalPop();
//        if (ele.first) return ele.second;
//        std::cerr << "Local pop queue didnt work";
//    }
//}

