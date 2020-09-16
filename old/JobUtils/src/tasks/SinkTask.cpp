//
// Created by jaime on 4/1/20.
//

#include "tasks/SinkTask.h"

template<class input, class output>
output Rhea::SinkTask<input, output>::run() {
    init(origin, destination);
    exec();
    finish();
}

template<class input, class output>
void Rhea::SinkTask<input, output>::terminate() {
    running = false;
}

template<class input, class output>
void Rhea::SinkTask<input, output>::init(int origin, int destination) {
    this->origin = origin;
    this->destination = destination;
    running = true;
}

template<class input, class output>
void Rhea::SinkTask<input, output>::exec() {
    auto client = basket::Singleton<Rhea::Client<std::vector<Event>>>::GetInstance(false);
    while (running) {
        if (client.get()->GetSize(origin) > 0) {
            auto event = client.get()->GetTask(origin);

            if (event.first) {
                std::ofstream out(event.second.at(0).file, ofstream::binary);
                for (Event ev : event.second) {
                    out.seekp(ev.offset);
                    out << ev.data.string();
                }
                out.close();
            }
        }
        std::this_thread::sleep_for(10ns);
    }
}

template<class input, class output>
void Rhea::SinkTask<input, output>::finish() {

}