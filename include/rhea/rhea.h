//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_RHEA_H
#define RHEA_RHEA_H

#include <sentinel/job_manager/client.h>
#include <rhea/common/data_structures.h>
#include <common/daemon.h>
#include <basket/common/singleton.h>
#include <basket/queue/queue.h>
#include <basket/unordered_map/unordered_map.h>
#include <rhea/byte_flow_regulator/byte_flow_regulator_client.h>
#include <rhea/common/configuration_manager.h>
namespace bip=boost::interprocess;

namespace rhea {

    class Client {
    private:
        std::shared_ptr<basket::unordered_map<Parcel, std::string>> warehouse;
        std::shared_ptr<basket::queue<Parcel>> write_queue, read_queue;
        std::shared_ptr<basket::unordered_map<Parcel,ParcelState>> parsel_state;
        uint32_t job_id_;
        bool is_application_;
    public:
        Client(uint32_t job_id_, bool is_application_=true);

        void FinalizeClient();

        bool Publish(Parcel &parcel, char* data);

        bool Subscribe(Parcel &parcel, char* data);

        std::vector<Parcel> GetWriteParsel(uint16_t server_id);

        std::vector<Parcel> GetReadParsel(uint16_t server_id);

        std::string GetData(Parcel &parcel);
        bool PutData(Parcel &parcel,char* data);
        bool DeleteData(Parcel &parcel);
        bool UpdateParcelStatus(Parcel &parcel, TaskStatus status);
        std::vector<ParcelState> WaitAll(std::vector<Parcel> &parcels);
        ParcelState Wait(Parcel &parcel);

    };
}


#endif //RHEA_RHEA_H
