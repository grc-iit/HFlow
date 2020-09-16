//
// Created by hariharan on 8/16/19.
//

#ifndef HLOG_CONFIGURATION_MANAGER_H
#define HLOG_CONFIGURATION_MANAGER_H

#include <basket/common/singleton.h>
#include <basket/common/typedefs.h>
#include <basket/common/data_structures.h>
#include <basket/common/macros.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>

#define RHEA_CONF basket::Singleton<Rhea::ConfigurationManager>::GetInstance()
namespace Rhea {
    class ConfigurationManager {

    private:
        void config(rapidjson::Document &doc, const char *member, uint16_t &variable) {
            assert(doc.HasMember(member));
            assert(doc[member].IsInt());
            variable = doc[member].GetInt();
        }

        void config(rapidjson::Document &doc, const char *member, really_long &variable) {
            assert(doc.HasMember(member));
            assert(doc[member].IsUint64());
            variable = doc[member].GetUint64();
        }

        void config(rapidjson::Document &doc, const char *member, CharStruct &variable) {
            assert(doc.HasMember(member));
            assert(doc[member].IsString());
            variable = doc[member].GetString();
        }

        int CountServers(CharStruct server_list_path) {
            fstream file;
            int total = 0;
            file.open(server_list_path.c_str(), ios::in);
            if (file.is_open()) {
                std::string file_line;
                std::string server_node_name;
                int count;
                while (getline(file, file_line)) {
                    if (!file_line.empty()) {
                        int split_loc = file_line.find(':');  // split to node and net
                        if (split_loc != std::string::npos) {
                            server_node_name = file_line.substr(0, split_loc);
                            count = atoi(file_line.substr(split_loc + 1, std::string::npos).c_str());
                        } else {
                            // no special network
                            server_node_name = file_line;
                            count = 1;
                        }
                        // server list is list of network interfaces
                        for (int i = 0; i < count; ++i) {
                            total++;
                        }
                    }
                }
            } else {
                printf("Error: Can't open server list file %s\n", server_list_path.c_str());
                exit(EXIT_FAILURE);
            }
            file.close();
            return total;
        }

    public:
        CharStruct QUEUE_CLIENT_LISTS;
        uint16_t QUEUE_SERVER_PORT, QUEUE_CLIENT_PORT;
        uint16_t JOB_MANAGER_SERVER_PORT, TASK_MANAGER_SERVER_PORT;
        uint16_t QUEUE_SERVER_RPC_THREADS, QUEUE_CLIENT_RPC_THREADS;
        CharStruct QUEUE_DIR;
        CharStruct CONFIGURATION_FILE;
        really_long NVME_CAPACITY, SSD_CAPACITY;
        int QUEUE_SERVERS_COUNT;


        ConfigurationManager() : CONFIGURATION_FILE("/home/jaime/Documents/Rhea/Rhea/conf/config/Rhea.json"),
                                 QUEUE_CLIENT_LISTS(
                                         "/home/jaime/Documents/Rhea/Rhea/conf/server_lists/chronoplayer_client"),
                                 QUEUE_DIR("/dev/shm/hari/Rhea"),
                                 QUEUE_SERVER_PORT(13000),
                                 QUEUE_CLIENT_PORT(14000),
                                 QUEUE_SERVER_RPC_THREADS(4),
                                 QUEUE_CLIENT_RPC_THREADS(1),
                                 JOB_MANAGER_SERVER_PORT(12000),
                                 TASK_MANAGER_SERVER_PORT(11000),
                //NVME_CAPACITY(128ULL*1024ULL),
                                 NVME_CAPACITY(200ULL * 1024ULL * 1024ULL * 1024ULL),
                                 SSD_CAPACITY(1024ULL * 1024ULL * 1024ULL * 1024ULL),
                                 QUEUE_SERVERS_COUNT(1) {}

        void LoadConfiguration() {
            using namespace rapidjson;

            FILE *outfile = fopen(CONFIGURATION_FILE.c_str(), "r");
            if (outfile == NULL) {
                std::cout << "HLog configuration not found" << std::endl;
                exit(EXIT_FAILURE);
            }
            char buf[65536];
            FileReadStream instream(outfile, buf, sizeof(buf));
            Document doc;
            doc.ParseStream<kParseStopWhenDoneFlag>(instream);
            if (!doc.IsObject()) {
                std::cout << "HLog - Canfiguration JSON is invalid" << std::endl;
                fclose(outfile);
                exit(EXIT_FAILURE);
            }
            config(doc, "QUEUE_CLIENT_LISTS", QUEUE_CLIENT_LISTS);
            config(doc, "JOB_MANAGER_SERVER_PORT", JOB_MANAGER_SERVER_PORT);
            config(doc, "TASK_MANAGER_SERVER_PORT", TASK_MANAGER_SERVER_PORT);
            config(doc, "QUEUE_DIR", QUEUE_DIR);
            config(doc, "QUEUE_SERVER_PORT", QUEUE_SERVER_PORT);
            config(doc, "QUEUE_CLIENT_PORT", QUEUE_CLIENT_PORT);
            config(doc, "QUEUE_CLIENT_RPC_THREADS", QUEUE_CLIENT_RPC_THREADS);
            config(doc, "QUEUE_SERVER_RPC_THREADS", QUEUE_SERVER_RPC_THREADS);
            config(doc, "NVME_CAPACITY", NVME_CAPACITY);
            config(doc, "SSD_CAPACITY", SSD_CAPACITY);

        }

        void ConfigureQueueClient() {
            LoadConfiguration();
            BASKET_CONF->RPC_THREADS = QUEUE_CLIENT_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1024ULL * 1ULL;
            if (QUEUE_CLIENT_LISTS.c_str() != "") BASKET_CONF->SERVER_LIST_PATH = QUEUE_CLIENT_LISTS.c_str();
            BASKET_CONF->LoadServers();
            BASKET_CONF->IS_SERVER = false;
            BASKET_CONF->MY_SERVER = BASKET_CONF->MPI_RANK % BASKET_CONF->NUM_SERVERS;
            BASKET_CONF->SERVER_ON_NODE = true;
            BASKET_CONF->RPC_PORT = QUEUE_CLIENT_PORT;
        }

        void ConfigureQueueServer() {
            LoadConfiguration();
            BASKET_CONF->RPC_THREADS = QUEUE_CLIENT_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1024ULL * 1ULL;
            if (QUEUE_CLIENT_LISTS.c_str() != "") BASKET_CONF->SERVER_LIST_PATH = QUEUE_CLIENT_LISTS.c_str();
            BASKET_CONF->LoadServers();
            BASKET_CONF->IS_SERVER = true;
            BASKET_CONF->MY_SERVER = BASKET_CONF->MPI_RANK % BASKET_CONF->NUM_SERVERS;
            BASKET_CONF->SERVER_ON_NODE = true;
            BASKET_CONF->RPC_PORT = QUEUE_CLIENT_PORT;
        }
    };
}


#endif //HLOG_CONFIGURATION_MANAGER_H
