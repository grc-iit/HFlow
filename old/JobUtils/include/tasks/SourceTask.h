//
// Created by jaime on 4/1/20.
//

#ifndef RHEA_SOURCE_TASK_H
#define RHEA_SOURCE_TASK_H

#include "atomic"
#import "Task.h"
#include "basket/common/singleton.h"
#include "basket/data_structure.h"
#include "client/client.h"

namespace Rhea {
    template<class input, class output>
    class SourceTask : public Rhea::Task<input, output> {
    protected:
        void init(int origin, int destination) override;

        void exec() override;

        void finish() override;

        std::atomic<bool> running;
    public:
        int origin;
        int destination;

        output run(int origin, int destination) override;

        void terminate() override;

        SourceTask() = default;

        SourceTask(const SourceTask &other) = default; /* copy constructor*/
        SourceTask(SourceTask &&other) = default;   /* move constructor*/
        SourceTask &operator=(const SourceTask &other) = default;
    };
}


#endif //RHEA_SOURCE_TASK_H
