//
// Created by hariharan on 8/14/19.
//

#ifndef HLOG_DATA_STRUCTURE_H
#define HLOG_DATA_STRUCTURE_H

#include <basket/common/data_structures.h>
#include <rpc/msgpack.hpp>


typedef struct TaskEvent {
    uint32_t task_node{};
    uint16_t task_origin{};
    uint32_t task_destination{};
    CharStruct source;

    TaskEvent() = default;

    TaskEvent(const TaskEvent &other) = default; /* copy constructor*/
    TaskEvent(TaskEvent &&other) = default;   /* move constructor*/
    TaskEvent &operator=(const TaskEvent &other) = default;
} Task;

typedef struct Event {
    uint32_t event_id{};
    CharStruct file;
    uint16_t offset{};
    CharStruct data;
    std::size_t keyby;

    Event() = default;

    Event(const Event &other) = default; /* copy constructor*/
    Event(Event &&other) = default;   /* move constructor*/
    Event &operator=(const Event &other) = default;

    bool operator<(const Event &event) const {
        return (offset < event.offset);
    }

} Event;

namespace std {
    template<>
    class hash<Event> {
    public:
        size_t operator()(const Event &event) const {
            return std::hash<CharStruct>()(event.file);
        }
    };
}


#ifdef BASKET_ENABLE_RPCLIB
namespace clmdep_msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
    namespace adaptor {
    namespace mv1 = clmdep_msgpack::v1;

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
        template<>
        struct convert<TaskEvent> {
            mv1::object const &operator()(mv1::object const &o, TaskEvent &input) const {
                input.source = o.via.array.ptr[0].as<CharStruct>();
                input.task_node = o.via.array.ptr[1].as<uint32_t>();
                input.task_destination = o.via.array.ptr[2].as<uint32_t>();
                input.task_origin = o.via.array.ptr[3].as<uint16_t>();
                return o;
            }
        };

        template<>
        struct pack<TaskEvent> {
            template<typename Stream>
            packer <Stream> &operator()(mv1::packer <Stream> &o, TaskEvent const &input) const {
                o.pack_array(4);
                o.pack(input.task_node);
                o.pack(input.task_destination);
                o.pack(input.task_origin);
                o.pack(input.source);
                return o;
            }
        };

        template<>
        struct object_with_zone<TaskEvent> {
            void operator()(mv1::object::with_zone &o, TaskEvent const &input) const {
                o.type = type::ARRAY;
                o.via.array.size = 4;
                o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                        sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                o.via.array.ptr[0] = mv1::object(input.task_node, o.zone);
                o.via.array.ptr[1] = mv1::object(input.task_destination, o.zone);
                o.via.array.ptr[2] = mv1::object(input.task_origin, o.zone);
                o.via.array.ptr[3] = mv1::object(input.source, o.zone);
            }
        };


        template<>
        struct convert<Event> {
            mv1::object const &operator()(mv1::object const &o, Event &input) const {
                input.event_id = o.via.array.ptr[0].as<uint32_t>();
                input.file = o.via.array.ptr[1].as<CharStruct>();
                input.offset = o.via.array.ptr[2].as<uint16_t>();
                input.data = o.via.array.ptr[3].as<CharStruct>();
                return o;
            }
        };

        template<>
        struct pack<Event> {
            template<typename Stream>
            packer <Stream> &operator()(mv1::packer <Stream> &o, Event const &input) const {
                o.pack_array(4);
                o.pack(input.event_id);
                o.pack(input.file);
                o.pack(input.offset);
                o.pack(input.data);
                return o;
            }
        };

        template<>
        struct object_with_zone<Event> {
            void operator()(mv1::object::with_zone &o, Event const &input) const {
                o.type = type::ARRAY;
                o.via.array.size = 4;
                o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
                        sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
                o.via.array.ptr[0] = mv1::object(input.event_id, o.zone);
                o.via.array.ptr[1] = mv1::object(input.file, o.zone);
                o.via.array.ptr[2] = mv1::object(input.offset, o.zone);
                o.via.array.ptr[3] = mv1::object(input.data, o.zone);
            }
        };
    }  // namespace adaptor
}
}  // namespace clmdep_msgpack
#endif

#endif //HLOG_DATA_STRUCTURE_H
