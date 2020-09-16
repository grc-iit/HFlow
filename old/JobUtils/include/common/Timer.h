//
// Created by jaime on 4/2/20.
//

#ifndef RHEA_TIMER_H
#define RHEA_TIMER_H

#include <chrono>

namespace Rhea {
    class Timer {
    private:
        std::chrono::high_resolution_clock::time_point t1;
        double time_elapsed;
    public:
        Timer() : time_elapsed(0) {
            t1 = std::chrono::high_resolution_clock::now();
        }

        /**
         *
         * @return miliseconds as a double
         */
        double getTimeElapsed() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - t1).count() / 1000000.0;
        }

        void reset() {
            t1 = std::chrono::high_resolution_clock::now();
        }
    };
}

#endif //RHEA_TIMER_H
