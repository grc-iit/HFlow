//
// Created by hdevarajan on 9/20/20.
//

#include <rhea_job/common/rhea_job.h>

std::shared_ptr<Job<Parcel>> create_job_0()  {
    return std::make_shared<RheaWriteJob>(0);
}

void free_job_0(RheaWriteJob *p) {
    { delete p; }
}

std::shared_ptr<Job<Parcel>> create_job_1() {
    return std::make_shared<RheaReadJob>(1);
}

void free_job_1(RheaReadJob *p) {

}
