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

    uint16_t server_id;
    RheaQueueSourceTask() : SourceTask(),server_id(0) {}

protected:
    bool Initialize(Parcel &event) override {
        server_id = atoi(event.id_.data());
        return true;
    }

    Parcel Run(Parcel &event) override {
        while(true){
            auto client = basket::Singleton<rhea::Client>::GetInstance(job_id_,false);
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
                basket::Singleton<FileIOClient>::GetInstance(0)->Write(source,destination); //TODO: FIX ME: getInstance should use redis
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
        auto redis_client = basket::Singleton<FileIOClient>::GetInstance(0); //TODO: FIX ME: getInstance should use redis
        redis_client->Read(source,destination);
        redis_client->Remove(source);
        basket::Singleton<IOFactory>::GetInstance()->GetIOClient(event.storage_index_)->Write(destination,source);
    }

    bool Finalize(Parcel &event) override {
        return false;
    }
} RheaSinkTask;

struct RheaJob : public Job<Parcel> {
    RheaJob(uint32_t job_id):Job<Parcel>(job_id){
        CreateDAG();
    }
    RheaJob(const RheaJob &other) : Job<Parcel>(other){}
    RheaJob(RheaJob &other) : Job<Parcel>(other) {}
    /*Define Assignment Operator*/
    RheaJob &operator=(const RheaJob &other) {
        Job<Parcel>::operator=(other);
        return *this;
    }

    void CreateDAG() override {
        source_ = std::make_shared<RheaQueueSourceTask>();
        source_->job_id_=job_id_;
        source_->id_=0;
        auto key_by = std::make_shared<RheaKeyByTask>();
        key_by->job_id_=job_id_;
        key_by->id_=1;
        source_->links.push_back(key_by);
        auto sink = std::make_shared<RheaSinkTask>();
        sink->job_id_=job_id_;
        sink->id_=2;
        key_by->links.push_back(sink);
    }
};

extern "C" {
    std::shared_ptr<Job<Parcel>> create_job_0();
    void free_job_0(RheaJob *p);
}

namespace clmdep_msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace mv1 = clmdep_msgpack::v1;

        namespace adaptor {

            template<>
            struct convert<Task<Parcel>> {
                mv1::object const &operator()(mv1::object const &o, Task<Parcel> &input) const {
                    input.job_id_=o.via.array.ptr[0].as<uint32_t>();
                    input.id_=o.via.array.ptr[1].as<uint32_t>();
                    input.type_=o.via.array.ptr[2].as<TaskType>();
                    input.links=o.via.array.ptr[3].as<std::vector<std::shared_ptr<Task<Parcel>>>>();
                    return o;
                }
            };

            template<>
            struct pack<Task<Parcel>> {
                template<typename Stream>
                packer<Stream> &operator()(mv1::packer<Stream> &o, Task<Parcel> const &input) const {
                    o.pack_array(4);
                    o.pack(input.job_id_);
                    o.pack(input.id_);
                    o.pack(input.type_);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<Task<Parcel>> {
                void operator()(mv1::object::with_zone &o, Task<Parcel> const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 4;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.job_id_, o.zone);
                    o.via.array.ptr[1] = mv1::object(input.id_, o.zone);
                    o.via.array.ptr[2] = mv1::object(input.type_, o.zone);
                    o.via.array.ptr[3] = mv1::object(input.links, o.zone);
                }
            };

            template<>
            struct convert<RheaQueueSourceTask> {
                mv1::object const &operator()(mv1::object const &o, RheaQueueSourceTask &input) const {
                    input.job_id_=o.via.array.ptr[0].as<uint32_t>();
                    input.id_=o.via.array.ptr[1].as<uint32_t>();
                    input.type_=o.via.array.ptr[2].as<TaskType>();
                    input.links=o.via.array.ptr[3].as<std::vector<std::shared_ptr<Task<Parcel>>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaQueueSourceTask> {
                template<typename Stream>
                packer<Stream> &operator()(mv1::packer<Stream> &o, RheaQueueSourceTask const &input) const {
                    o.pack_array(4);
                    o.pack(input.job_id_);
                    o.pack(input.id_);
                    o.pack(input.type_);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaQueueSourceTask> {
                void operator()(mv1::object::with_zone &o, RheaQueueSourceTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 4;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.job_id_, o.zone);
                    o.via.array.ptr[1] = mv1::object(input.id_, o.zone);
                    o.via.array.ptr[2] = mv1::object(input.type_, o.zone);
                    o.via.array.ptr[3] = mv1::object(input.links, o.zone);
                }
            };

            template<>
            struct convert<RheaKeyByTask> {
                mv1::object const &operator()(mv1::object const &o, RheaKeyByTask &input) const {
                    input.job_id_=o.via.array.ptr[0].as<uint32_t>();
                    input.id_=o.via.array.ptr[1].as<uint32_t>();
                    input.type_=o.via.array.ptr[2].as<TaskType>();
                    input.links=o.via.array.ptr[3].as<std::vector<std::shared_ptr<Task<Parcel>>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaKeyByTask> {
                template<typename Stream>
                packer<Stream> &operator()(mv1::packer<Stream> &o, RheaKeyByTask const &input) const {
                    o.pack_array(4);
                    o.pack(input.job_id_);
                    o.pack(input.id_);
                    o.pack(input.type_);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaKeyByTask> {
                void operator()(mv1::object::with_zone &o, RheaKeyByTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 4;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.job_id_, o.zone);
                    o.via.array.ptr[1] = mv1::object(input.id_, o.zone);
                    o.via.array.ptr[2] = mv1::object(input.type_, o.zone);
                    o.via.array.ptr[3] = mv1::object(input.links, o.zone);
                }
            };
            template<>
            struct convert<RheaSinkTask> {
                mv1::object const &operator()(mv1::object const &o, RheaSinkTask &input) const {
                    input.job_id_=o.via.array.ptr[0].as<uint32_t>();
                    input.id_=o.via.array.ptr[1].as<uint32_t>();
                    input.type_=o.via.array.ptr[2].as<TaskType>();
                    input.links=o.via.array.ptr[3].as<std::vector<std::shared_ptr<Task<Parcel>>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaSinkTask> {
                template<typename Stream>
                packer<Stream> &operator()(mv1::packer<Stream> &o, RheaSinkTask const &input) const {
                    o.pack_array(4);
                    o.pack(input.job_id_);
                    o.pack(input.id_);
                    o.pack(input.type_);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaSinkTask> {
                void operator()(mv1::object::with_zone &o, RheaSinkTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 4;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.job_id_, o.zone);
                    o.via.array.ptr[1] = mv1::object(input.id_, o.zone);
                    o.via.array.ptr[2] = mv1::object(input.type_, o.zone);
                    o.via.array.ptr[3] = mv1::object(input.links, o.zone);
                }
            };

        }  // namespace adaptor
    }
}  // namespace clmdep_msgpack
#endif //RHEA_RHEA_JOB_H