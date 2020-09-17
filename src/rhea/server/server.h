//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_JOB_MANAGER_H
#define RHEA_SERVER_H

#include <cstdint>
#include <time.h>
#include <chrono>
#include <thread>
#include <future>

#define DEFAULT_INTERVAL 300
#define VARIATION 100

namespace rhea{

    enum  struct Alter_Type{
        GROW,
        SHRINK
    };

    class server {
    private:
        /**
         * FIXME: all methods use capitalize case e.g., alter_collector -> AlterCollector
         * Do the actual calls through the Lib and integrate it.
         */
        time_t interval;
        uint_fast16_t variation;
        int alter_collector(Alter_Type);
        int alter_transformers(Alter_Type);
        int alter_writers(Alter_Type);
        uint_fast64_t get_in_rate();
        uint_fast64_t get_out_rate();
    public:
        server(){
            interval = DEFAULT_INTERVAL;
            variation = VARIATION;
        }
        server(server &other){
            this->interval = other.interval;
            this->variation = other.variation;
        }
        server &operator=(const server &other){
            this->interval = other.interval;
            this->variation = other.variation;
            return *this;
        }
        void run();
        void single_loop(std::future<void> futureObj);
    };
}



#endif //RHEA_JOB_MANAGER_H
