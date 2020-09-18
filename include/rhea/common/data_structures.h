//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_DATA_STRUCTURES_H
#define RHEA_DATA_STRUCTURES_H

#include <sentinel/common/data_structures.h>

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

#endif //RHEA_DATA_STRUCTURES_H
