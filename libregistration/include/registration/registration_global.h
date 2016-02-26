#ifndef ESU_REGISTRATION_GLOBAL
#define ESU_REGISTRATION_GLOBAL

#ifndef ESU_APPLICATION

#define ESU_REGISTRATION_ID                         0x00000030
#define ESU_REGISTRATION_NAME_ID                    "ESURegistration"
#define ESU_REGISTRATION_UI_ID                      0x00000031
#define ESU_REGISTRATION_UI_NAME_ID                 "ESURegistrationUI"

#ifndef Q_FRIEND
#define Q_FRIEND(Class) friend class Class;
#endif

#define ESU_APP_CONFIG_PATH "/usr/local/etc/nic/"

#else

#include "esu_global.h"
#include "esu_globals_id.h"

#endif

#endif // ESU_REGISTRATION_GLOBAL

