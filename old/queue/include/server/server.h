#ifndef RHEA_SERVER_H
#define RHEA_SERVER_H


#include <basket/queue/queue.h>
#include <basket/configuration_manager.h>

namespace Rhea{
    template<class T>
    class Server {
    private:
        basket::queue<T> server_request_queue;
//        std::shared_ptr<RPC> server_rpc;
        /**
         * Server hosting
         */
        std::promise<void> exit_server_signal;
        std::thread worker;

        void RunInternal(std::future<void> futureObj);

    public:
        Server();
//        TaskEvent GetTask();
//        bool PutTask(TaskEvent task);
        /**
         * Server hosting
         */
        void Run(){
            std::future<void> futureObj = exit_server_signal.get_future();
            worker=std::thread (&Server::RunInternal, this, std::move(futureObj));
        }

        void Stop(){
            exit_server_signal.set_value();
            worker.join();
            if(BASKET_CONF->MPI_RANK == 0) printf("Stopped the Chronoplayer server\n");
        }
    };
}


#endif RHEA_SERVER_H
