//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_BYTEFLOW_REGULATOR_SERVER_H
#define RHEA_BYTEFLOW_REGULATOR_SERVER_H 

#include <cstdint>
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
// datastructures is commented out due to compilation issues
#include <sentinel/job_manager/client.h>
#include <basket/communication/rpc_factory.h>
#include <rpc/client.h>

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

        // make all these maps against job_id
        time_t interval;
        uint_fast16_t variation;
        uint_fast64_t step;
        std::shared_ptr<RPC> client_rpc_;

        std::unordered_map<uint16_t, uint_fast32_t> in_rate_map;
        std::unordered_map<uint16_t, uint_fast32_t> out_rate_map;
        bool AlterCollector(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterTransformers(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterNodes(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        void SetInRate(uint16_t job_id, uint_fast32_t in_rate);
        void SetOutRate(uint16_t job_id, uint_fast32_t out_rate);
        void RunInternal(std::future<void> futureObj);
        void RPCInit();
        void ConfigInit();
    public:
        server(){
            interval = DEFAULT_INTERVAL;
            variation = VARIATION;
            step = STEP;
            ConfigInit();
            RPCInit();
        }
        server(server &other){
            this->interval = other.interval;
            this->variation = other.variation;
            this->step = other.variation;
            ConfigInit();
            RPCInit();
        }
        server &operator=(const server &other){
            this->interval = other.interval;
            this->variation = other.variation;
            this->step = other.step;
            ConfigInit();
            RPCInit();
            return *this;
        }
        void Run();
    };
}

#endif //RHEA_JOB_MANAGER_H
