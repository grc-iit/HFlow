//
// Created by jaime on 4/1/20.
//

#ifndef RHEA_KEYBYTASK_H
#define RHEA_KEYBYTASK_H

#include "atomic"
#import "Task.h"
#include "basket/common/singleton.h"
#include "basket/data_structure.h"
#include "client/client.h"

namespace Rhea {
    template<class input, class output>
    class KeyByTask : public Rhea::Task<input, output> {
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

        KeyByTask() = default;

        KeyByTask(const KeyByTask &other) = default; /* copy constructor*/
        KeyByTask(KeyByTask &&other) = default;   /* move constructor*/
        KeyByTask &operator=(const KeyByTask &other) = default;
    };
}

#endif //RHEA_KEYBYTASK_H
