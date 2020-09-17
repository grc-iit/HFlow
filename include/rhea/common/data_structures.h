//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_DATA_STRUCTURES_H
#define RHEA_DATA_STRUCTURES_H

#include <sentinel/common/data_structures.h>

typedef struct Parcel: public Data{

    /*Define the default, copy and move constructor*/
    Parcel(CharStruct id, size_t position, char *buffer, uint16_t storage_index, size_t data_size): Data(id_(id), position_(position), buffer_(buffer), storage_index_(storage_index),data_size_(data_size)){}

    Parcel(const Parcel &other): Data(other){}
    Parcel(Parcel &other): Data(other){}
    /*Define Assignment Operator*/
    Parcel &operator=(const Parcel &other){
        Data::operator=(other);
        return *this;
    }
}Parcel;

typedef struct Flow {
    std::vector<Parcel> parcels_;

    /*Define the default, copy and move constructor*/
    Flow(): parcels_(){}
    Flow(const Flow &other): parcels_(std::vector(other.parcels_)){}
    Flow(Flow &other): parcels_(std::vector(other.parcels_)){}
} Flow;

#endif //RHEA_DATA_STRUCTURES_H
