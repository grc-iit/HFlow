//
// Created by jaime on 4/14/20.
//

#ifndef RHEA_DAEMON_H
#define RHEA_DAEMON_H

#include <fstream>
#include <csignal>
#include <execinfo.h>
#include <unistd.h>
#include <future>

#include <mpi.h>
#include <thread>

namespace Rhea {
    template<class T>
    class Daemon {
    public:
        std::string main_log_file;
        std::promise<void> exitSignal;
        std::thread worker;
        T jobManager;

        Daemon(int argc, char *argv[], std::string main_log_file = "Rhea_JobManager_log.log");

        virtual ~Daemon();

    private:
        void signalHandler(int sig);

        void logMessage(const std::string &filename, const std::string &message);

        void catchSignals();

    };
}

#endif //RHEA_DAEMON_H
