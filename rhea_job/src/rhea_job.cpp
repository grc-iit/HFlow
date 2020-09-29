//
// Created by hdevarajan on 9/20/20.
//

#include <rhea_job/common/rhea_job.h>

std::shared_ptr<Job<Parcel>> create_job_0()  {
    return std::make_shared<RheaWriteJob>(0);
}

void free_job_0(RheaWriteJob *p) {
    { }
}

std::shared_ptr<Job<Parcel>> create_job_1() {
    return std::make_shared<RheaReadJob>(1);
}

void free_job_1(RheaReadJob *p) {

}

std::shared_ptr<Job<Parcel>> create_job_2() {
    return std::make_shared<IOFWriteJob>(2);
}

void free_job_2(IOFWriteJob *p) {

}

std::shared_ptr<Job<Parcel>> create_job_3() {
    return std::make_shared<IOFWriteJob>(3);
}

void free_job_3(IOFWriteJob *p) {

}

std::shared_ptr<Job<Parcel>> create_job_4() {
    return std::make_shared<IOFWriteJob>(4);
}

void free_job_4(IOFWriteJob *p) {

}
