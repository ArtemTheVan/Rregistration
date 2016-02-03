#include "staff_object_p.h"

#include "esu_staff_engine.h"


// [ INIT/DESTROY ]: {{{

StaffObjectPrivate::StaffObjectPrivate():
    ref(1)
  , engine(nullptr)
  , nameBase(""), name(""), nameFull("")
  , nameFullPart(""), nameDescription(""), description("")
  , number(0)
{
    //
}


StaffObjectPrivate::StaffObjectPrivate(StaffObjectPrivate *other, bool deep, bool copyLinks):
    ref(1)
  , engine(other->engine)
{
    Q_UNUSED(deep)
    Q_UNUSED(copyLinks)

    nameBase = other->nameBase;
    name = other->name;
    nameFull = other->nameFull;
    nameFullPart = other->nameFullPart;
    nameDescription = other->nameDescription;
    number = other->number;
    note = other->note;
}


StaffObjectPrivate::~StaffObjectPrivate()
{
    //
}


StaffObjectPrivate *StaffObjectPrivate::clone(bool deep)
{
    StaffObjectPrivate* p = new StaffObjectPrivate(this, deep);
    p->ref.deref();
    return p;
}


StaffObjectPrivate *StaffObjectPrivate::copy()
{
    StaffObjectPrivate* p = new StaffObjectPrivate(this, false, false);
    p->ref.deref();
    return p;
}

// }}} [ INIT/DESTROY ]

void StaffObjectPrivate::clear()
{
    name.clear();
    nameBase.clear();
    nameDescription.clear();
    nameFull.clear();
    nameFullPart.clear();
    number = 0;
}
