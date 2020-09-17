//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_JOB_MANAGER_H
#define RHEA_JOB_MANAGER_H

#include <cstdint>
#include <time.h>
#include <chrono>
#include <thread>
#include <future>

#include <basket.h>
#include <sentinel/job_manager/client.h>

#define DEFAULT_INTERVAL 300
#define VARIATION 100
#define STEP 5

namespace rhea{

    enum  struct Alter_Type: int8_t {
        GROW = 1,
        SHRINK = -1,
    };

    class server {
    private:
        /**
         * FIXME: all methods use capitalize case e.g., alter_collector -> AlterCollector
         * Do the actual calls through the Lib and integrate it.
         */
        time_t interval;
        uint_fast16_t variation;
        uint_fast64_t step;
        bool AlterCollector(uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterTransformers(uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterWriters(uint_fast64_t out_rate, uint_fast64_t in_rate);
        uint_fast64_t get_in_rate();
        void RunInternal(std::future<void> futureObj);
        uint_fast64_t get_out_rate();
        // sentinel::job_manager::client job_manager;
        // auto rpc;
    public:
        server(){
            interval = DEFAULT_INTERVAL;
            variation = VARIATION;
            step = STEP;
            // rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
        }
        server(server &other){
            this->interval = other.interval;
            this->variation = other.variation;
            this->step = other.variation;
        }
        server &operator=(const server &other){
            this->interval = other.interval;
            this->variation = other.variation;
            this->step = other.step;
            return *this;
        }
        void Run();
    };
}

#endif //RHEA_JOB_MANAGER_H
