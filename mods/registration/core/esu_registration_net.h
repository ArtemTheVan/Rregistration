#ifndef ESU_REGISTRATION_NET
#define ESU_REGISTRATION_NET

#ifdef ESU_NET_SYSTEM
    #include "protomessageserver.h"

    #ifdef esuNet
        #undef esuNet
    #endif
    #define esuNet ProtoMessageServer::inst().registrationInterface()
#endif

#endif // ESU_REGISTRATION_NET

