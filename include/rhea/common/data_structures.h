//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_COMMON_DATA_STRUCTURES_H
#define RHEA_COMMON_DATA_STRUCTURES_H

#include <sentinel/common/data_structures.h>

typedef struct Parcel : public Event {
    uint16_t storage_index_;
    /*Define the default, copy and move constructor*/
    Parcel() : Event(),storage_index_() {}

    Parcel(const Parcel &other) : Event(other),storage_index_(other.storage_index_) {}

    Parcel(Parcel &other) : Event(other),storage_index_(other.storage_index_) {}

    /*Define Assignment Operator*/
    Parcel &operator=(const Parcel &other) {
        Event::operator=(other);
        storage_index_=other.storage_index_;
        return *this;
    }
} Parcel;




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
std::ostream &operator<<(std::ostream &os, Parcel &parcel);

#endif //RHEA_COMMON_DATA_STRUCTURES_H