//
// Created by jaime on 4/15/20.
//

#include "server/Server.h"
#include "../../../JobManager/include/client/Client.h"


Server::TaskManager::TaskManager(Rhea::Job job) : exit_server_signal(),
                                                  worker(),
                                                  num_servers(BASKET_CONF->NUM_SERVERS),
                                                  pool(boost::asio::thread_pool()) {
    InitCPUMeasure();
    Bind();

    my_id =; //TODO: fill this
    jm_id =;

    LoadLibrary(job); //TODO: get this from JobManager

    std::future<void> futureObj = exit_server_signal.get_future();
    RunInternal(std::move(futureObj));
}

bool Server::TaskManager::TerminateTaskManager() {
    auto it = executorStop.begin();
    while (it != executorStop.end()) {
        DestroyTask(it->first);
    }
    exit_server_signal.set_value();
    return true;
}

/* RPC Server */
void Server::TaskManager::RunInternal(std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        usleep(10000);

        Rhea::ResourceReport newReport = generateLoadReport();
        if (newReport == previousReport) continue;
        else {
            Client::JobManager jmClient;
            jmClient.UpdateLoad(jm_id, Rhea::ClusterNode("", my_id), newReport);
            previousReport = newReport;
        }
    }
}

void Server::TaskManager::Bind() {
    client_rpc = basket::Singleton<RPCFactory>::GetInstance()->GetRPC(RHEA_CONF->JOB_MANAGER_SERVER_PORT);

    auto functionUpdateDestination(
            std::bind(&::Server::TaskManager::updateDestination, this, std::placeholders::_1, std::placeholders::_2));
    client_rpc->bind("UpdateDestination", functionUpdateDestination);

    auto functionSpawnTask(
            std::bind(&::Server::TaskManager::SpawnTask, this, std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3, std::placeholders::_4));
    client_rpc->bind("SpawnTask", functionSpawnTask);

    auto functionDestroyTask(std::bind(&::Server::TaskManager::DestroyTask, this, std::placeholders::_1));
    client_rpc->bind("DestroyTask", functionDestroyTask);

    auto functionGetDestination(std::bind(&::Server::TaskManager::GetDestination, this, std::placeholders::_1));
    client_rpc->bind("GetDestination", functionGetDestination);

    auto functionTerminateTaskManager(std::bind(&::Server::TaskManager::TerminateTaskManager, this));
    client_rpc->bind("TerminateTaskManager", functionTerminateTaskManager);

    auto functionMeasureCpu(std::bind(&Server::TaskManager::MeasureCpu, this));
    client_rpc->bind("measureCpu", functionMeasureCpu);

    auto functionMeasureMemory(std::bind(&Server::TaskManager::MeasureMemory, this));
    client_rpc->bind("measureMemory", functionMeasureMemory);

    /* Queue RPC */
    auto functionGetTask(std::bind(&Server::TaskManager::GetTask, this, std::placeholders::_1));
    client_rpc->bind("GetTask", functionGetTask);

    auto functionPutTask(std::bind(&Server::TaskManager::PutTask, this, std::placeholders::_1, std::placeholders::_2,
                                   std::placeholders::_3));
    client_rpc->bind("PutTask", functionPutTask);
}

/* Queue Server */
std::pair<bool, Rhea::Event> Server::TaskManager::GetTask(const std::string &taskId) {
    auto t = queueMap.find(taskId);
    if (t == queueMap.end()) {
        //You are running a task trying to get events for himself, we shoudl return false
        return std::pair<bool, Rhea::Event>(false, Rhea::Event());
    } else {
        auto pop = queueMap.at(taskId).LocalPop();
        return pop;
    }
}

bool Server::TaskManager::PutTask(std::string taskId, Rhea::Event event, int taskManagerId) {
    auto t = queueMap.find(taskId);
    if (t == queueMap.end()) {
        queueMap.emplace(taskId, basket::queue<Rhea::Event>(taskId, BASKET_CONF->RPC_PORT));
        SpawnTask(taskId, Rhea::TaskLocation(taskManagerId, taskId), Rhea::TaskLocation(0, "none"), taskManagerId);
    }
    auto pop = queueMap.at(taskId).LocalPush(event);
    return pop;
}

/* Spawn Task */
bool
Server::TaskManager::SpawnTask(const std::string &taskId, Rhea::TaskLocation origin, Rhea::TaskLocation destination,
                               int currentTaskManagerId) {
    std::promise<void> threadExit;
    std::future<void> futureObj = threadExit.get_future();

    if (destination.taskId != "none") {
        executorDestination.emplace(taskId, destination);
    }

    std::string taskName = taskId.substr(0, taskId.find(':'));
    boost::asio::post(pool, boost::bind(&Rhea::TaskExecutor::run, taskId, taskGenerators[taskName], origin,
                                        currentTaskManagerId, std::move(futureObj)));

    executorStop.insert(std::pair<std::string, std::promise<void>>(taskId, std::move(threadExit)));

    return true;
}

bool Server::TaskManager::DestroyTask(const std::string &taskId) {
    auto threadExit = std::move(executorStop.at(taskId));
    threadExit.set_value();
    executorStop.erase(taskId);

    queueMap.erase(taskId);
    executorDestination.erase(taskId);
    return true;
}

/* Task Managment */
bool Server::TaskManager::updateDestination(std::string taskId, Rhea::TaskLocation newDestination) {
    executorDestination[taskId] = newDestination;
    return true;
}

Rhea::TaskLocation Server::TaskManager::GetDestination(std::string taskId) {
    auto t = executorDestination.find(taskId); //TODO: this is a map
    if (t == executorDestination.end()) {
        //TODO: Request from JobManager
    }
    return executorDestination[taskId];
}

/* Library */
bool Server::TaskManager::LoadLibrary(Rhea::Job job) {
    void *handle = dlopen((job.jobPath).c_str(), RTLD_LAZY);
    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        exit(EXIT_FAILURE);
    }
    for (auto &iter : job.taskData) {
        if (!loadSymbol(handle, iter.second.TaskName)) return false;
    }
    return true;
}

bool Server::TaskManager::loadSymbol(void *handle, std::string task) {
    dlerror();
    auto constructor = (Rhea::Task<Rhea::Event, Rhea::Event> *(*)()) dlsym(handle, ("create_" + task).c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Constructor not Found for " << task << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    taskGenerators.emplace(task, constructor);
    return true;
}

/* Measures */
double Server::TaskManager::MeasureCpu() {
    struct tms timeSample;
    clock_t now;
    double percent;

    now = times(&timeSample);
    if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
        timeSample.tms_utime < lastUserCPU) {
        //Overflow detection. Just skip this value.
        percent = -1.0;
    } else {
        percent = timeSample.tms_stime - lastSysCPU +
                  (timeSample.tms_utime - lastUserCPU);
        percent /= now - lastCPU;
        percent /= numProcessors;
        percent *= 100;
    }
    lastCPU = now;
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    return percent;
}

int Server::TaskManager::MeasureMemory() {
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = ParseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void Server::TaskManager::InitCPUMeasure() {
    FILE *file;
    struct tms timeSample;
    char line[128];

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }
    fclose(file);
}

int Server::TaskManager::ParseLine(char *line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

Rhea::ResourceReport Server::TaskManager::generateLoadReport() {
    return Rhea::ResourceReport(my_id, MeasureMemory(), MeasureCpu(), 0); //Maybe later we can fill queue Usage
}
