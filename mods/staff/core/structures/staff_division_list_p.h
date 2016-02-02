#ifndef STAFF_DIVISION_LIST_P
#define STAFF_DIVISION_LIST_P

#include "staff_division_list.h"


class StaffDivisionListPrivate
{
public:
    StaffDivisionListPrivate(): ref(1)
    { }

public:
    QAtomicInt ref;
    QList<StaffDivisionPrivate*> data;
}

#endif // STAFF_DIVISION_LIST_P

