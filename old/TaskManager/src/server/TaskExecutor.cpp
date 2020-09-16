//
// Created by jaime on 4/20/20.
//

#include <common/Timer.h>

#include <utility>
#include "server/TaskExecutor.h"
#include "common/Action.h"

void Rhea::TaskExecutor::run(std::string taskId, std::function<Rhea::Task<Rhea::Event, Rhea::Event> *()> taskGenerator,
                             TaskLocation origin, int currentTaskMangerId,
                             std::future<void> futureObj) {

    init();
    exec(taskId, taskGenerator, origin, currentTaskMangerId, std::move(futureObj));
    finalize(currentTaskMangerId);
}

void Rhea::TaskExecutor::init() {
    //load config for timeout
}


void Rhea::TaskExecutor::exec(std::string taskId, std::function<Rhea::Task<Rhea::Event, Rhea::Event> *()> taskGenerator,
                              const TaskLocation &origin, int currentTaskMangerId, std::future<void> futureObj) {
    taskGenerator = std::move(taskGenerator);
    auto taskManager = Client::TaskManager();
    Task<Event, Event> *task = taskGenerator();

    task->init();
    Rhea::Timer timer;
    while (timer.getTimeElapsed() < timeout ||
           futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        auto event = taskManager.GetTask(origin.taskManagerId, origin.taskId);
        if (event.first) {
            Event *result = task->exec(event.second); //TODO: shared pointer
            send(taskId, result, event.second, currentTaskMangerId);
            delete result;
            timer.reset();
        } else {
            usleep(10000);
        }
    }
    Event *result = task->finish();
    send(taskId, result, Rhea::Event(), currentTaskMangerId);
    delete result;
}

void Rhea::TaskExecutor::finalize(int currentTasManagerId) {
    auto taskManager = Client::TaskManager();
    taskManager.DestroyTask(currentTasManagerId);
}

void Rhea::TaskExecutor::send(std::string taskId, Event *result, Event original, int currentTaskMangerId) {
    auto taskManager = Client::TaskManager();
    switch (result->action) {
        case NEW: {
            TaskLocation destination = taskManager.GetDestination(currentTaskMangerId, taskId);
            taskManager.PutTask(destination.taskManagerId, destination.taskId, *result);
            break;
        }
        case ORIGINAL: {
            TaskLocation destination = taskManager.GetDestination(currentTaskMangerId, taskId);
            taskManager.PutTask(destination.taskManagerId, destination.taskId, original);
            break;
        }
        case KEYBY: { //TODO: Redirect
            TaskLocation destination = taskManager.GetDestination(currentTaskMangerId, taskId);
            auto loc = (EventLocation *) result; //TODO: std::static_cast<>, reinterpretedCast
            taskManager.PutTask(loc->TaskManagerId, loc->TaskID, original);
            break;
        }
        case HOLD: {
            break;
        }
        default: {
            std::cout << "Default on send on taskManager: " << currentTaskMangerId << std::endl;
            break;
        }
    }

}


