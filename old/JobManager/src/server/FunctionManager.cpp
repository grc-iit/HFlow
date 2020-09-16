//
// Created by jaime on 4/15/20.
//

#include "server/FunctionManager.h"

std::pair<bool, Rhea::Job> Rhea::FunctionManager::generateJob(const std::string &jobPath) {
    auto parsedJob = parseJob(jobPath + "job.json");

    if (!parsedJob.first) return std::pair<bool, Rhea::Job>(false, Rhea::Job());
    if (!validateJob(parsedJob.second, jobPath)) return std::pair<bool, Rhea::Job>(false, Rhea::Job());

    return parsedJob;
}

std::pair<bool, Rhea::Job> Rhea::FunctionManager::parseJob(const std::string &jobPath) {

    std::unordered_map<std::string, std::vector<std::string>> adjacency;
    std::unordered_map<std::string, JobNode> taskData;

    auto doc = openJob(jobPath + "job.json");

    const rapidjson::Value &adj = doc["adjancency"];
    assert(adj.IsArray());
    const rapidjson::Value &nodes = doc["nodes"];
    assert(nodes.IsArray());

    for (rapidjson::Value::ConstMemberIterator iter = adj.MemberBegin(); iter != adj.MemberEnd(); ++iter) {
        std::string c = std::string(iter->name.GetString());
        std::vector<std::string> parents;
        for (auto &v : iter->value.GetArray())
            parents.push_back(std::move(std::string(v.GetString())));
        adjacency.insert(std::pair<std::string, std::vector<std::string>>(c, parents));
    }

    for (auto &iter : adjacency) {
        std::string key = iter.first;
        nodes[key.c_str()];
        std::string s = std::string(nodes["name"].GetString());
        int n = nodes["parallelism"].GetInt();
        taskData.insert(std::pair<std::string, JobNode>(key, JobNode(s, n)));
    }

    return std::pair<bool, Rhea::Job>(true, Rhea::Job(jobPath + "job.so", std::move(adjacency), std::move(taskData)));
}

bool Rhea::FunctionManager::validateJob(const Rhea::Job &job, const std::string &jobPath) {
    using std::cout;
    using std::cerr;
    cout << "C++ dlopen demo\n\n";
    // open the library
    cout << "Opening hello.so...\n";;
    void *handle = dlopen((jobPath + "job.so").c_str(), RTLD_LAZY);
    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        exit(EXIT_FAILURE);
    }
    for (auto &iter : job.taskData) {
        if (!parseSymbol(handle, iter.second.TaskName.c_str())) return false;
    }
    return true;
}

rapidjson::Document Rhea::FunctionManager::openJob(const std::string &jobPath) {
    FILE *outfile = fopen(jobPath.c_str(), "r");
    if (outfile == nullptr) {
        std::cout << "HLog configuration not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    char buf[65536];
    rapidjson::FileReadStream instream(outfile, buf, sizeof(buf));
    rapidjson::Document doc;
    if (doc.ParseStream<rapidjson::kParseStopWhenDoneFlag>(instream).HasParseError()) {
        std::cout << "Job's JSON is invalid" << std::endl;
        fclose(outfile);
        exit(EXIT_FAILURE);
    }
    fclose(outfile);
    return doc;
}

bool Rhea::FunctionManager::parseSymbol(void *handle, const char *symbol) {
    dlerror();
    typedef void (*task_ptr)(); //TODO: add Task in final form and change void to Task* (https://stackoverflow.com/questions/16495909/c-dynamically-load-classes)
    auto constructor = (task_ptr *) dlsym(handle, symbol);
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Constructor not Found for " << symbol << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }
    return true;
}
