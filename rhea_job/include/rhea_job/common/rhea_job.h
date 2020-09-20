//
// Created by hdevarajan on 9/18/20.
//

#ifndef RHEA_RHEA_JOB_H
#define RHEA_RHEA_JOB_H


#include <rpc/msgpack.hpp>
#include <sentinel/common/data_structures.h>
#include <rhea/common/configuration_manager.h>
#include <rhea/rhea.h>
#include <common/io_clients/io_factory.h>

typedef struct RheaQueueSourceTask : public SourceTask<Parcel> {
    std::shared_ptr<rhea::Client> client;
    uint16_t server_id;
    RheaQueueSourceTask() : SourceTask(),server_id(0) {}

protected:
    bool Initialize(Parcel &event) override {
        client = basket::Singleton<rhea::Client>::GetInstance(job_id_,false);
        server_id = atoi(event.id_.data());
        return true;
    }

    Parcel Run(Parcel &event) override {
        while(true){
            auto parsels = client->GetParsel(server_id);
            if(parsels.size() == 0) {
                usleep(100);
                continue;
            }
            for(auto parsel:parsels){
                std::string data = client->GetData(parsel);
                Parcel destination = parsel;
                Parcel source = parsel;
                source.buffer_=data.data();
                basket::Singleton<RedisIOClient>::GetInstance(RHEA_CONF->REDIS_INDEX)->Write(source,destination);
                client->DeleteData(parsel);
                emit(job_id_, id_, parsel);
            }
        }
        return event;
    }

    bool Finalize(Parcel &event) override {
        return true;
    }
} RheaQueueSourceTask;

typedef struct RheaKeyByTask : public KeyByTask<Parcel> {
    RheaKeyByTask() : KeyByTask<Parcel>() {}

protected:
    bool Initialize(Parcel &event) override {
        return true;
    }

    size_t Run(Parcel &event) override {
        auto hash = std::hash<std::string>();
        return hash(std::string(event.id_.c_str()));
    }

    bool Finalize(Parcel &event) override {
        return true;
    }
} RheaKeyByTask;

typedef struct RheaSinkTask : public SinkTask<Parcel> {
    RheaSinkTask() : SinkTask<Parcel>() {}

protected:
    bool Initialize(Parcel &event) override {
        return true;
    }

    void Run(Parcel &event) override {
        Parcel destination = event;
        Parcel source = event;
        basket::Singleton<RedisIOClient>::GetInstance(RHEA_CONF->REDIS_INDEX)->Read(source,destination);
        basket::Singleton<IOFactory>::GetInstance()->GetIOClient(event.storage_index_)->Write(destination,source);
    }

    bool Finalize(Parcel &event) override {
        return false;
    }
} RheaSinkTask;

struct RheaJob : public Job<Parcel> {
    RheaJob(uint32_t job_id):Job<Parcel>(job_id){}
    RheaJob(const RheaJob &other) : Job<Parcel>(other){}
    RheaJob(RheaJob &other) : Job<Parcel>(other) {}
    /*Define Assignment Operator*/
    RheaJob &operator=(const RheaJob &other) {
        Job<Parcel>::operator=(other);
        return *this;
    }

    void CreateDAG() override {
        source_ = std::make_shared<RheaQueueSourceTask>();
        source_->id_=0;
        auto key_by = std::make_shared<RheaKeyByTask>();
        source_->links.push_back(key_by);
        auto sink = std::make_shared<RheaSinkTask>();
        key_by->links.push_back(sink);
    }
};

extern "C" {
    std::shared_ptr<RheaJob> create_job_2() {
        return std::make_shared<RheaJob>(2);
    }
    void free_job_2(RheaJob *p) { delete p; }
}

#endif //RHEA_RHEA_JOB_H