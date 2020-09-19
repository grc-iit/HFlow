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
        void LoadChildConfigurations(rapidjson::Document &doc) override {
            config(doc, "CONFIGURATION_FILE", CONFIGURATION_FILE);
            config(doc, "BYTEFLOW_SIZE_MAP_NAME", BYTEFLOW_SIZE_MAP_NAME);
            config(doc, "BYTEFLOW_STAT_PUSH_INTERVAL", BYTEFLOW_STAT_PUSH_INTERVAL);
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

        ConfigurationManager() : common::ConfigurationManager(),
                                 BYTEFLOW_SIZE_MAP_NAME("test_byteflow_size_map"),
                                 BYTEFLOW_STAT_PUSH_INTERVAL(100),
                                 RHEA_CLIENT_SERVICE_DIR("/dev/shm/rhea/rhea_client_service"),
                                 BYTEFLOW_REGULATOR_DIR("/dev/shm/rhea/rhea_byteflow_regulator"),
                                 BYTEFLOW_REGULATOR_SERVER_PORT(9000),
                                 BYTEFLOW_REGULATOR_RPC_THREADS(4),
                                 BYTEFLOW_REGULATOR_HOST("./hostfile_bfr"),
                                 RHEA_CLIENT_SERVICE_PORT(12000),
                                 RHEA_CLIENT_SERVICE_RPC_THREADS(4),
                                 RHEA_CLIENT_SERVICE_HOST("./hostfile_rcs"){
            this->CONFIGURATION_FILE = CharStruct("/home/user/sentinel/conf/base_rhea.conf");
        }

        void ConfigureByteflowRegulatorClient() {
            LoadConfiguration();
            //port, SERVER_LIST
            BASKET_CONF->ConfigureDefaultClient(BYTEFLOW_REGULATOR_HOST.c_str());
            BASKET_CONF->RPC_PORT = BYTEFLOW_REGULATOR_SERVER_PORT;
        }

        void ConfigureByteflowRegulatorServer() {
            LoadConfiguration();
            //port, list of servers, server dir,
            BASKET_CONF->RPC_THREADS = BYTEFLOW_REGULATOR_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1ULL;
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
        }

        void ConfigureRheaClientService() {
            LoadConfiguration();
            //port, list of servers, server dir,
            BASKET_CONF->RPC_THREADS = RHEA_CLIENT_SERVICE_RPC_THREADS;
            BASKET_CONF->MEMORY_ALLOCATED = 1024ULL * 1024ULL * 1ULL;
            BASKET_CONF->BACKED_FILE_DIR = RHEA_CLIENT_SERVICE_DIR;
            BASKET_CONF->ConfigureDefaultServer(RHEA_CLIENT_SERVICE_HOST.c_str());
            RHEA_CLIENT_SERVICE_COUNT = BASKET_CONF->NUM_SERVERS;
            BASKET_CONF->RPC_PORT = RHEA_CLIENT_SERVICE_PORT;
        }
    };
}

namespace clmdep_msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {
            namespace mv1=clmdep_msgpack::v1;
            template<>
            struct convert<bip::string> {
                clmdep_msgpack::object const &operator()(clmdep_msgpack::object const &o, bip::string &v) const {
                    switch (o.type) {
                        case clmdep_msgpack::type::BIN:
                            v.assign(o.via.bin.ptr, o.via.bin.size);
                            break;
                        case clmdep_msgpack::type::STR:
                            v.assign(o.via.str.ptr, o.via.str.size);
                            break;
                        default:
                            throw clmdep_msgpack::type_error();
                            break;
                    }
                    return o;
                }
            };

            template<>
            struct pack<bip::string> {
                template<typename Stream>
                clmdep_msgpack::packer<Stream> &
                operator()(clmdep_msgpack::packer<Stream> &o, const bip::string &v) const {
                    uint32_t size = checked_get_container_size(v.size());
                    o.pack_str(size);
                    o.pack_str_body(v.data(), size);
                    return o;
                }
            };

            template<>
            struct object<bip::string> {
                void operator()(clmdep_msgpack::object &o, const bip::string &v) const {
                    uint32_t size = checked_get_container_size(v.size());
                    o.type = clmdep_msgpack::type::STR;
                    o.via.str.ptr = v.data();
                    o.via.str.size = size;
                }
            };

            template<>
            struct object_with_zone<bip::string> {
                void operator()(clmdep_msgpack::object::with_zone &o, const bip::string &v) const {
                    uint32_t size = checked_get_container_size(v.size());
                    o.type = clmdep_msgpack::type::STR;
                    char *ptr = static_cast<char *>(o.zone.allocate_align(size, MSGPACK_ZONE_ALIGNOF(char)));
                    o.via.str.ptr = ptr;
                    o.via.str.size = size;
                    std::memcpy(ptr, v.data(), v.size());
                }
            };
        }
    }
}

#endif //RHEA_CONFIGURATION_MANAGER_H
