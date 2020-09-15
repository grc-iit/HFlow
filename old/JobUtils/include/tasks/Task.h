//
// Created by jaime on 4/1/20.
//

#ifndef RHEA_EXECUTABLE_H
#define RHEA_EXECUTABLE_H

#include <future>

namespace Rhea {
    template<class input, class output>
    class Task {
    public:
        virtual void init() = 0;

        virtual Event *exec(Event event) = 0;

        virtual Event *finish() = 0;

        Task() = default;
        Task(const Task &other) = default; /* copy constructor*/
        Task(Task &&other) = default;   /* move constructor*/
        Task &operator=(const Task &other) = default;
    };
}


#endif //RHEA_EXECUTABLE_H
