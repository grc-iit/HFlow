//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_COMMON_DATA_STRUCTURES_H
#define RHEA_COMMON_DATA_STRUCTURES_H

#include <sentinel/common/data_structures.h>

typedef struct Parcel : public Event {


    /*Define the default, copy and move constructor*/
    Parcel() : Event() {}

    Parcel(const Parcel &other) : Event(other) {}

    Parcel(Parcel &other) : Event(other) {}

    /*Define Assignment Operator*/
    Parcel &operator=(const Parcel &other) {
        Event::operator=(other);
        return *this;
    }
} Parcel;

typedef struct RheaSourceTask: public SourceTask{
    RheaSourceTask():SourceTask(){}

    void Execute(){
        printf("Test RheaSourceTask's execute function....\n");
    }
}RheaSourceTask;

typedef struct RheaKeyByTask: public KeyByTask{
    RheaKeyByTask():KeyByTask(){}

    void Execute(){
        printf("Test RheaKeyByTask's execute function....\n");
    }
}RheaKeyByTask;

typedef struct RheaSinkTask: public SinkTask{
    RheaSinkTask():SinkTask(){}

    void Execute(){
        printf("Test RheaSinkTask's execute function....\n");
    }
}RheaSinkTask;

namespace clmdep_msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace mv1 = clmdep_msgpack::v1;

        namespace adaptor {
            template<>
            struct convert<Parcel> {
                mv1::object const &operator()(mv1::object const &o, Parcel &input) const {
                    input.id_ = o.via.array.ptr[0].as<CharStruct>();
                    input.position_ = o.via.array.ptr[1].as<size_t>();
                    auto data = o.via.array.ptr[2].as<std::string>();
                    input.data_size_ = o.via.array.ptr[3].as<size_t>();
                    if (!data.empty()) {
                        input.buffer_ = static_cast<char *>(malloc(input.data_size_));
                        memcpy(input.buffer_, data.data(), input.data_size_);
                    }
                    input.storage_index_ = o.via.array.ptr[4].as<uint16_t>();
                    return o;
                }
            };

            template<>
            struct pack<Parcel> {
                template<typename Stream>
                packer <Stream> &operator()(mv1::packer<Stream> &o, Parcel const &input) const {
                    o.pack_array(5);
                    o.pack(input.id_);
                    o.pack(input.position_);
                    if (input.buffer_ == NULL) o.pack(std::string());
                    else {
                        o.pack(std::string(input.buffer_, input.data_size_));
                    }
                    o.pack(input.data_size_);
                    o.pack(input.storage_index_);
                    return o;
                }
            };

            template<>
            struct object_with_zone<Parcel> {
                void operator()(mv1::object::with_zone &o, Parcel const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 5;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.id_, o.zone);
                    o.via.array.ptr[1] = mv1::object(input.position_, o.zone);
                    if (input.buffer_ == NULL) o.via.array.ptr[2] = mv1::object(std::string(), o.zone);
                    else {
                        o.via.array.ptr[2] = mv1::object(std::string(input.buffer_, input.data_size_), o.zone);
                        free(input.buffer_);
                    }
                    o.via.array.ptr[3] = mv1::object(input.data_size_, o.zone);
                    o.via.array.ptr[4] = mv1::object(input.storage_index_, o.zone);
                }
            };
            template<>
            struct convert<RheaSourceTask> {
                mv1::object const &operator()(mv1::object const &o, RheaSourceTask &input) const {
                    input.links = o.via.array.ptr[0].as<std::vector<std::shared_ptr<Task>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaSourceTask>{
                template<typename Stream>
                packer <Stream> &operator()(mv1::packer <Stream> &o, RheaSourceTask const &input) const {
                    o.pack_array(1);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaSourceTask> {
                void operator()(mv1::object::with_zone &o, RheaSourceTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 1;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.links, o.zone);
                }
            };

            template<>
            struct convert<RheaKeyByTask> {
                mv1::object const &operator()(mv1::object const &o, RheaKeyByTask &input) const {
                    input.links = o.via.array.ptr[0].as<std::vector<std::shared_ptr<Task>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaKeyByTask>{
                template<typename Stream>
                packer <Stream> &operator()(mv1::packer <Stream> &o, RheaKeyByTask const &input) const {
                    o.pack_array(1);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaKeyByTask> {
                void operator()(mv1::object::with_zone &o, SourceTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 1;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.links, o.zone);
                }
            };

            template<>
            struct convert<RheaSinkTask> {
                mv1::object const &operator()(mv1::object const &o, RheaSinkTask &input) const {
                    input.links = o.via.array.ptr[0].as<std::vector<std::shared_ptr<Task>>>();
                    return o;
                }
            };

            template<>
            struct pack<RheaSinkTask>{
                template<typename Stream>
                packer <Stream> &operator()(mv1::packer <Stream> &o, RheaSinkTask const &input) const {
                    o.pack_array(1);
                    o.pack(input.links);
                    return o;
                }
            };

            template<>
            struct object_with_zone<RheaSinkTask> {
                void operator()(mv1::object::with_zone &o, RheaSinkTask const &input) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 1;
                    o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                            sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                    o.via.array.ptr[0] = mv1::object(input.links, o.zone);
                }
            };
        }  // namespace adaptor
    }
}  // namespace clmdep_msgpack

std::ostream &operator<<(std::ostream &os, Parcel &parcel);

#endif //RHEA_COMMON_DATA_STRUCTURES_H