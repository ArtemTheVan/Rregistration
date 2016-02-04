#ifndef ESU_STAFF_P
#define ESU_STAFF_P

#include "staff_structure.h"
#include "staff_engine.h"


class ESUStaffStructurePrivate
{
    Q_DECLARE_PUBLIC(ESUStaffStructure)

public:
    ESUStaffStructurePrivate(ESUStaffStructure* parent = nullptr):
        q_ptr(parent)
      , defaultStructFile("qrc:./../../conf/staff_structure_msb.xml")
      , defaultStaffFile("./../../conf/staff.xml")
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

