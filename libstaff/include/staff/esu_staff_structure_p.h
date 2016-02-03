#ifndef ESU_STAFF_P
#define ESU_STAFF_P

#include "esu_staff_structure.h"
#include "esu_staff_engine.h"


class ESUStaffStructurePrivate
{
    Q_DECLARE_PUBLIC(ESUStaffStructure)

public:
    ESUStaffStructurePrivate(ESUStaffStructure* parent = nullptr):
        q_ptr(parent)
      , defaultStructFile("qrc:/mods/staff/conf/staff_structure_msb.xml")
      , defaultStaffFile("/media/card/esutz/staff/staff.xml")
      , currentFile("")
    { }

    ~ESUStaffStructurePrivate() { }

public:
    ESUStaffStructure* q_ptr;
    QString defaultStructFile;
    QString defaultStaffFile;

    QString currentFile;
    ESUStaffEngine engine;
};


#endif // ESU_STAFF_P

