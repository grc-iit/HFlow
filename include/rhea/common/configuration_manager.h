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

#define RHEA_CONF basket::Singleton<rhea::ConfigurationManager>::GetInstance()
namespace rhea {
    class ConfigurationManager {

    private:
        static std::string replaceEnvVariable(std::string temp_variable){

            std::string pattern("(\\$\\{.*?\\})");
            auto regexp = regex(pattern);
            smatch m;
            regex_search(temp_variable, m, regexp);
            auto variables=std::set<std::string>();
            for(unsigned i=0; i<m.size(); ++i) {
                auto extracted_val = m[i].str();
                //if(extracted_val.find("{") == std::string::npos) continue;
                auto val = m[i].str().substr(2, m[i].str().size() - 3);
                variables.insert(val);
            }
            for(auto variable:variables){
                auto unrolled = std::getenv(variable.c_str());
                if(unrolled==NULL) throw ErrorException(UNDEFINED_ENV_VARIABLE,variable.c_str());
                temp_variable = regex_replace(temp_variable, regexp, unrolled);
            }
            return temp_variable;
        }
        template <typename T>
        void config(T &doc, const char *member, uint16_t &variable) {
            if(!doc.HasMember(member)) return;
            assert(doc[member].IsInt());
            variable = atoi(replaceEnvVariable(std::to_string(doc[member].GetInt())).c_str());
        }
        template <typename T>
        void config(T &doc, const char *member, really_long &variable) {
            if(!doc.HasMember(member)) return;
            assert(doc[member].IsUint64());
            variable = atoll(replaceEnvVariable(std::to_string(doc[member].GetUint64())).c_str());
        }

        template <typename T>
        void config(T &doc, const char *member, std::string &variable) {
            if(!doc.HasMember(member)) return;
            assert(doc[member].IsString());
            std::string temp_variable = doc[member].GetString();
            variable = replaceEnvVariable(temp_variable);
        }
        template <typename T>
        void config(T &doc, const char *member, CharStruct &variable) {
            if(!doc.HasMember(member)) return;
            assert(doc[member].IsString());
            std::string temp_variable = doc[member].GetString();
            variable = CharStruct(replaceEnvVariable(temp_variable));
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
                            count = atoi(file_line.substr(split_loc, std::string::npos).c_str());
                        } else {
                            // no special network
                            server_node_name = file_line;
                            count = 1;
                        }
                        // server list is list of network interfaces
                        for (int i = 0; i < count; i++) {
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
        CharStruct BYTEFLOW_SIZE_MAP_NAME;
        CharStruct CONFIGURATION_FILE;
        uint64_t BYTEFLOW_STAT_PUSH_INTERVAL;

        ConfigurationManager() : BYTEFLOW_SIZE_MAP_NAME("test_byteflow_size_map"),
                                 BYTEFLOW_STAT_PUSH_INTERVAL(100),
                                 CONFIGURATION_FILE("/home/user/sentinel/conf/base_rhea.conf"){}

        void LoadConfiguration() {
            using namespace rapidjson;

            FILE *outfile = fopen(CONFIGURATION_FILE.c_str(), "r");
            if (outfile == NULL) {
                printf("Configuration not found %s \n",CONFIGURATION_FILE.c_str());
                exit(EXIT_FAILURE);
            }
            char buf[65536];
            FileReadStream instream(outfile, buf, sizeof(buf));
            Document doc;
            doc.ParseStream<kParseStopWhenDoneFlag>(instream);
            if (!doc.IsObject()) {
                std::cout << "Configuration JSON is invalid" << std::endl;
                fclose(outfile);
                exit(EXIT_FAILURE);
            }
            config(doc, "CONFIGURATION_FILE", CONFIGURATION_FILE);
            config(doc, "BYTEFLOW_SIZE_MAP_NAME", BYTEFLOW_SIZE_MAP_NAME);
            config(doc, "BYTEFLOW_STAT_PUSH_INTERVAL", BYTEFLOW_STAT_PUSH_INTERVAL);

            fclose(outfile);
        }
    };
}

#endif //RHEA_CONFIGURATION_MANAGER_H
