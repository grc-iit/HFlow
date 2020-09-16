//
// Created by jaime on 4/22/20.
//

#include <stdio.h>
#include <server/Server.h>
#include "../Daemon/include/Daemon.h"

int main(int argc, const char *argv[]) {
    //TODO: INit MPI and config basket
    auto daemon = new Rhea::Daemon<Server::TaskManager>();
}