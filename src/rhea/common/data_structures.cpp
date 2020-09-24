//
// Created by hdevarajan on 9/23/20.
//
#include <rhea/common/data_structures.h>

std::ostream &operator<<(std::ostream &os, Parcel &data){
    return os << "{id_:" << data.id_ << ","
              << "unique_id:" << data.unique_id << ","
              << "data_size_:" << data.data_size_ << ","
              << "position_:" << data.position_ << ","
              << "storage_index_:" << data.storage_index_ << "}";
}