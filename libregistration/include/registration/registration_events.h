#ifndef ESU_REGISTRATION_EVENTS
#define ESU_REGISTRATION_EVENTS

#ifdef ESU_APPLICATION
#include "core_event.h"
#else
#include <QtCore/qcoreevent.h>
#endif
#include "registration_data.h"


#ifndef ESU_APPLICATION

// [ GENERIC EVENT ]: {{{

class ESUEvent : public QEvent
{
public:
    enum BaseType {
        EmptyEvent              = User + 1,
        RegistrationEvent       = User + 2,
        ProfileEvent            = User + 3
    };

public:
    explicit ESUEvent(BaseType type = EmptyEvent):
        QEvent((Type)type)
    { }

    virtual ~ESUEvent()
    { }
};

// }}}

#endif

class ESURegistrationEvent: public ESUEvent
{
public:
    enum EventType {
        NoEvent, RegistrationRequestEvent, RegistrationRequestState,
        StopCurrentOperationEvent
    };

    enum State { NoState, Successfull, Rejected, Error };

public:
    explicit ESURegistrationEvent(EventType event = NoEvent):
        ESUEvent(ESUEvent::RegistrationEvent)
      , m_event(event)
    { }

    ~ESURegistrationEvent() { }

public:
    inline EventType eventType() const { return(m_event); }

    // [ User Data ]:
    inline void setUserData(const RegistrationUserInfo& data);
    inline RegistrationUserInfo& userData() { return(*p_userData); }
    inline bool hasUserData() const { return( !p_userData.isNull() ); }

    // [ Registration State ]:
    inline void setState(State state) { m_state = state; }
    inline State state() const { return m_state; }

protected:
    EventType m_event;
    State m_state;
    QSharedPointer<RegistrationUserInfo> p_userData;
};

// [ IMPLEMENTATION ]:

void ESURegistrationEvent::setUserData(const RegistrationUserInfo &data)
{
    if( p_userData.isNull() )
        p_userData = QSharedPointer<RegistrationUserInfo>(new RegistrationUserInfo(data));
    else (*p_userData) = data;
}

#endif // ESU_REGISTRATION_EVENTS
