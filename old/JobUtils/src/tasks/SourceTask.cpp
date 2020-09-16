//.h.
// Created by jaime on 4/1/20.
//

#include <basket/common/singleton.h>
#include "tasks/SourceTask.h"

template<class input, class output>
output Rhea::SourceTask<input, output>::run(int origin, int destination) {
    init(origin, destination);
    exec();
    finish();
}


template<class input, class output>
void Rhea::SourceTask<input, output>::terminate() {
    running = false;
}

template<class input, class output>
void Rhea::SourceTask<input, output>::init(int origin, int destination) {
    this->origin = origin;
    this->destination = destination;
    running = true;
}

template<class input, class output>
void Rhea::SourceTask<input, output>::exec() {
    auto client = basket::Singleton<Rhea::Client<std::vector<Event>>>::GetInstance(false);
    while (running) {
        if (client.get()->GetSize(origin) > 0) {
            auto event = client.get()->GetTask(origin);
            if (event.first) {
                client.get()->PutTask(event.second);
            }
        }
        std::this_thread::sleep_for(10ns);
    }
}

template<class input, class output>
void Rhea::SourceTask<input, output>::finish() {

}

