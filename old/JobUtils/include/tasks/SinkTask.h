//
// Created by jaime on 4/1/20.
//

#ifndef RHEA_SINKTASK_H
#define RHEA_SINKTASK_H


#import "Task.h"
#include "basket/common/singleton.h"
#include "basket/data_structure.h"
#include "client/client.h"

namespace Rhea {
    template<class input, class output>
    class SinkTask : public Rhea::Task<input, output> {
    protected:
        void init(int origin, int destination) override;

        void exec() override;

        void finish() override;

        std::atomic<bool> running;
    public:
        int origin;
        int destination;

        output run() override;

        void terminate() override;

        SinkTask() = default;

        SinkTask(const SinkTask &other) = default; /* copy constructor*/
        SinkTask(SinkTask &&other) = default;   /* move constructor*/
        SinkTask &operator=(const SinkTask &other) = default;
    };
}

#endif //RHEA_SINKTASK_H


