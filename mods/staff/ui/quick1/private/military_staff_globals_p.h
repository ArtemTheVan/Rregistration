#ifndef MILITARY_STAFF_GLOBALS_P
#define MILITARY_STAFF_GLOBALS_P

#include <QtGlobal>
#include <QtCore/QObject>

#ifdef ESU_UI_DECLARATIVE
    #include <QtDeclarative> // QtQuick 1.1
#else    
    #include <QtQml> // QtQuick 2.0 or higher
#endif


class MilitaryStaffGlobals: public QObject
{
    Q_OBJECT

public:
    enum StaffObjectType {
        UnknownType = 0, CommanderType = 1, SoldierType = 2, DivisionType = 3
    };

    enum ViewMode {
        UnknownUIMode, StaffUIMode, StructureUIMode
    };

    enum UserAction {
        UnknownAction, DefaultAction, CreateStaffAction
    };

    Q_ENUMS(StaffObjectType)
    Q_ENUMS(ViewMode)
    Q_ENUMS(UserAction)

public:
    static void declareQML() {
        static bool declared = false;

        if( declared ) return;
//        qmlRegisterUncreatableType<MilitaryStaffGlobals>(
//                    "MKBKompas.CSR.ESUTZ.StaffModule", 1, 0, "StaffUI", "Can\'t create MilitaryStaff object instance!");
        declared = true;
    }

protected:
    MilitaryStaffGlobals() { }
    ~MilitaryStaffGlobals() { }
};


#endif // MILITARY_STAFF_GLOBALS_P

