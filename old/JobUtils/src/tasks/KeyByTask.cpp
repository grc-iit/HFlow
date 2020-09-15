//
// Created by jaime on 4/1/20.
//

#include "tasks/KeyByTask.h"

template<class input, class output>
output Rhea::KeyByTask<input, output>::run() {
    init(origin, destination);
    exec();
    finish();
}

template<class input, class output>
void Rhea::KeyByTask<input, output>::terminate() {
    running = false;
}

template<class input, class output>
void Rhea::KeyByTask<input, output>::init(int origin, int destination) {
    this->origin = origin;
    this->destination = destination;
    running = true;
}

template<class input, class output>
void Rhea::KeyByTask<input, output>::exec() {
    auto client = basket::Singleton<Rhea::Client<std::vector<Event>>>::GetInstance(false);

    while (running) {
        if (client.get()->GetSize(origin) > 0) {
            auto event = client.get()->GetTask(origin);
            if (event.first) {
                event.second.at(0).keyby = std::hash<Event>{}(event.second.at(0));

                client.get()->PutTask(event.second,
                                      origin); //TODO: how to indicate to basket to put it back into origin
            }
        }
        std::this_thread::sleep_for(10ns);
    }
}

/**
 * Jab Manager
 *      - map, int(thread_count)
 *      - callback exposed
 *          - lock
 *          - find if present return else atomic add and return
 *          - unlock
 * Task1
 *      - call callback with hash(filename1)
 * Task2
 *      - call callback with hash(filename2)
 */
* /

template<class input, class output>
void Rhea::KeyByTask<input, output>::finish() {

}