//
// Created by jaime on 4/20/20.
//

#ifndef RHEA_EVENT_H
#define RHEA_EVENT_H

#include <common/Action.h>

namespace Rhea {
    class Event {
    public:
        int id;
        int type;
        std::string data;
        Action action;
    };

}

namespace Rhea {
    class EventLocation : public Event {
    public:
        int TaskManagerId;
        std::string TaskID;
        Action action;
    };

}


#endif //RHEA_EVENT_H
