//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_CONFIGURATION_MANAGER_H
#define RHEA_CONFIGURATION_MANAGER_H

#include <basket/common/singleton.h>
#include <basket/common/typedefs.h>
#include <sentinel/common/enumerations.h>
#include <sentinel/common/data_structures.h>
#include <basket/common/data_structures.h>
#include <basket/common/macros.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <regex>
#include <boost/filesystem/operations.hpp>
#include <sentinel/common/error_codes.h>
#include <common/configuration_manager.h>

#define RHEA_CONF basket::Singleton<rhea::ConfigurationManager>::GetInstance()
namespace rhea {
    class ConfigurationManager : public common::ConfigurationManager {

    protected:
        void LoadChildConfigurations(void *doc_) override {
            rapidjson::Document* doc = (rapidjson::Document*)doc_;
            config(doc, "BYTEFLOW_SIZE_MAP_NAME", BYTEFLOW_SIZE_MAP_NAME);
            config(doc, "BYTEFLOW_STAT_PUSH_INTERVAL", BYTEFLOW_STAT_PUSH_INTERVAL);
            config(doc, "RHEA_CLIENT_SERVICE_DIR", RHEA_CLIENT_SERVICE_DIR);
            config(doc, "BYTEFLOW_REGULATOR_DIR", BYTEFLOW_REGULATOR_DIR);
            config(doc, "BYTEFLOW_REGULATOR_SERVER_PORT", BYTEFLOW_REGULATOR_SERVER_PORT);
            config(doc, "BYTEFLOW_REGULATOR_RPC_THREADS", BYTEFLOW_REGULATOR_RPC_THREADS);
            config(doc, "BYTEFLOW_REGULATOR_HOST", BYTEFLOW_REGULATOR_HOST);
            config(doc, "RHEA_CLIENT_SERVICE_PORT", RHEA_CLIENT_SERVICE_PORT);
            config(doc, "RHEA_CLIENT_SERVICE_RPC_THREADS", RHEA_CLIENT_SERVICE_RPC_THREADS);
            config(doc, "RHEA_CLIENT_SERVICE_HOST", RHEA_CLIENT_SERVICE_HOST);
            boost::filesystem::create_directories(RHEA_CLIENT_SERVICE_DIR.c_str());
            boost::filesystem::create_directories(BYTEFLOW_REGULATOR_DIR.c_str());
        }

    public:
        CharStruct BYTEFLOW_REGULATOR_DIR;
        uint16_t BYTEFLOW_REGULATOR_SERVER_PORT;
        uint16_t BYTEFLOW_REGULATOR_RPC_THREADS;
        CharStruct BYTEFLOW_REGULATOR_HOST;
        uint16_t BYTEFLOW_REGULATOR_COUNT;

        CharStruct RHEA_CLIENT_SERVICE_DIR;
        uint16_t RHEA_CLIENT_SERVICE_PORT;
        uint16_t RHEA_CLIENT_SERVICE_RPC_THREADS;
        CharStruct RHEA_CLIENT_SERVICE_HOST;
        uint16_t RHEA_CLIENT_SERVICE_COUNT;

        CharStruct BYTEFLOW_SIZE_MAP_NAME;
        uint64_t BYTEFLOW_STAT_PUSH_INTERVAL;

        ConfigurationManager() : //common::ConfigurationManager("/home/user/sentinel/conf/base_rhea.conf"),
                                 BYTEFLOW_SIZE_MAP_NAME("test_byteflow_size_map"),
                                 BYTEFLOW_STAT_PUSH_INTERVAL(100),
                                 RHEA_CLIENT_SERVICE_DIR("/dev/shm/rhea/rhea_client_service"),
                                 BYTEFLOW_REGULATOR_DIR("/dev/shm/rhea/rhea_byteflow_regulator"),
                                 BYTEFLOW_REGULATOR_SERVER_PORT(9000),
                                 BYTEFLOW_REGULATOR_RPC_THREADS(4),
                                 BYTEFLOW_REGULATOR_HOST("${HOME}/projects/rhea/scripts/local/byteflow_regulator"),
                                 RHEA_CLIENT_SERVICE_PORT(12000),
                                 RHEA_CLIENT_SERVICE_RPC_THREADS(4),
                                 RHEA_CLIENT_SERVICE_HOST("${HOME}/projects/rhea/scripts/local/client_service") {

        }

        void ConfigureByteflowRegulatorClient() {
            LoadConfiguration();
            //port, SERVER_LIST
            BASKET_CONF->ConfigureDefaultClient(BYTEFLOW_REGULATOR_HOST.c_str());
            BASKET_CONF->RPC_PORT = BYTEFLOW_REGULATOR_SERVER_PORT;
            BYTEFLOW_REGULATOR_COUNT = BASKET_CONF->NUM_SERVERS;
        }

        void ConfigureByteflowRegulatorServer() {
            LoadConfiguration();
            //port, list of servers, server dir,
            BASKET_CONF->RPC_THREADS = BYTEFLOW_REGULATOR_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1024ULL;
            BASKET_CONF->BACKED_FILE_DIR = BYTEFLOW_REGULATOR_DIR;
            BASKET_CONF->ConfigureDefaultServer(BYTEFLOW_REGULATOR_HOST.c_str());
            BYTEFLOW_REGULATOR_COUNT = BASKET_CONF->NUM_SERVERS;
            BASKET_CONF->RPC_PORT = BYTEFLOW_REGULATOR_SERVER_PORT;
        }

        void ConfigureRheaClient() {
            LoadConfiguration();
            //port, SERVER_LIST
            BASKET_CONF->ConfigureDefaultClient(RHEA_CLIENT_SERVICE_HOST.c_str());
            BASKET_CONF->RPC_PORT = RHEA_CLIENT_SERVICE_PORT;
            RHEA_CLIENT_SERVICE_COUNT = BASKET_CONF->NUM_SERVERS;
        }

        void ConfigureRheaClientService() {
            LoadConfiguration();
            //port, list of servers, server dir,
            BASKET_CONF->RPC_THREADS = RHEA_CLIENT_SERVICE_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1024ULL;
            BASKET_CONF->BACKED_FILE_DIR = RHEA_CLIENT_SERVICE_DIR;
            BASKET_CONF->ConfigureDefaultServer(RHEA_CLIENT_SERVICE_HOST.c_str());
            RHEA_CLIENT_SERVICE_COUNT = BASKET_CONF->NUM_SERVERS;
            BASKET_CONF->RPC_PORT = RHEA_CLIENT_SERVICE_PORT;
        }
    };
}


#endif //RHEA_CONFIGURATION_MANAGER_H
