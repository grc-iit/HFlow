//
// Created by hdevarajan on 9/20/20.
//

#include <rhea_job/common/rhea_job.h>

std::shared_ptr<Job<Parcel>> create_job_0()  {
    return std::make_shared<RheaJob>(0);
}

void free_job_0(RheaJob *p) {
    { delete p; }
}
