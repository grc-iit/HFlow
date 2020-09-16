//
// Created by jaime on 4/1/20.
//

#include "tasks/AggregateTask.h"

template<class input, class output>
Rhea::AggregateTask<input, output>::AggregateTask(int max_size, int max_time) {
    this->max_size = max_size;
    this->max_time = max_time;
}

template<class input, class output>
output Rhea::AggregateTask<input, output>::run(int origin, int destination) {
    init(origin, destination);
    exec();
    finish();
}

template<class input, class output>
void Rhea::AggregateTask<input, output>::terminate() {
    running = false;
}


template<class input, class output>
void Rhea::AggregateTask<input, output>::init(int origin, int destination) {
    this->origin = origin;
    this->destination = destination;
    running = true;
}

template<class input, class output>
void Rhea::AggregateTask<input, output>::exec() {
    auto client = basket::Singleton<Rhea::Client<std::vector<Event>>>::GetInstance(false);

    std::string active_file;
    std::vector<Event> active_aggregator;

    while (running) {
        if (client.get()->GetSize(origin) > 0) {
            auto event = client.get()->GetTask(origin);
            if (event.first) { //Succesfull get
                active_file = event.second.file.string();
                if (aggregators.find(active_file) == aggregators.end()) { //No active aggregator
                    aggregators[active_file] = std::vector<Event>();
                }
                active_aggregator = aggregators[active_file];
                active_aggregator.push_back(event.second.at(0));
            }

            if (active_aggregator.size() == 1) {
                //If it is our first element, launch the timer.
                timers[active_file] = Timer();
            }

            //TODO: we will need a final element to trigger this
            if (active_aggregator.size() > max_size or timers[active_file].getTimeElapsed() > max_time) {
                std::sort(active_aggregator.begin(), active_aggregator.end());
                client.get()->PutTask(active_aggregator);
                timers.erase(active_file); //TODO: do the objects survive after deleting the entry?. Hari?
                aggregators.erase(active_file);
            }
        }
        std::this_thread::sleep_for(10ns);
    }
}

template<class input, class output>
void Rhea::AggregateTask<input, output>::finish() {

}




