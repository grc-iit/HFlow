//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_DATA_STRUCTURES_H
#define RHEA_DATA_STRUCTURES_H

#include <sentinel/common/data_structures.h>

typedef struct Flow {
    std::vector<Data> parcels_;

    /*Define the default, copy and move constructor*/
    Flow(): parcels_(){}
    Flow(const Flow &other): parcels_(other.parcels_.copy()){}
    Flow(Flow &other): parcels_(other.parcels_.copy()){}
} Flow;

#endif //RHEA_DATA_STRUCTURES_H
