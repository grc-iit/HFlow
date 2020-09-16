//
// Created by jaime on 4/1/20.
//

#ifndef RHEA_AGGREGATETASK_H
#define RHEA_AGGREGATETASK_H

#include "atomic"
#import "Task.h"
#import "common/Timer.h"
#import "basket/data_structure.h"
#import "client/client.h"

namespace Rhea {
    template<class input, class output>
    class AggregateTask : public Rhea::Task<input, output> {
    protected:
        void init(int origin, int destination) override;

        void exec() override;

        void finish() override;

        std::atomic<bool> running;
        std::unordered_map<std::string, Timer> timers;
        std::unordered_map<std::string, std::vector<Event>> aggregators;
        int max_size;
        int max_time;

    public:
        int origin;
        int destination;

        output run(int origin, int destination) override;

        void terminate() override;

        AggregateTask(int max_size, int max_time);

        AggregateTask(const AggregateTask &other) = default; /* copy constructor*/
        AggregateTask(AggregateTask &&other) = default;   /* move constructor*/
        AggregateTask &operator=(const AggregateTask &other) = default;


    };
}

#endif //RHEA_AGGREGATETASK_H
