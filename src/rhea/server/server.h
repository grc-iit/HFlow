//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_JOB_MANAGER_H
#define RHEA_SERVER_H

#include <sentinel/client.h>

namespace rhea{

    enum  struct Alter_Type{
        GROW,
        SHRINK
    };

    class server {
    private:
        int alter_collector(Alter_Type);
        int alter_transformers(Alter_Type);
        int alter_writers(Alter_Type);
        float get_rate();
    public:
        void single_loop();
    };
}



#endif //RHEA_JOB_MANAGER_H
