#include "staff_division_list.h"
#include "staff_division_list_p.h"


// [ INIT/DESTROY ]: {{{

StaffDivisionList::StaffDivisionList()
{
    d = nullptr;
}


StaffDivisionList::StaffDivisionList(const StaffDivisionList &other)
{
    d = other.d;
    if( d ) d->ref.ref();
}


StaffDivisionList::StaffDivisionList(StaffDivisionListPrivate *dd)
{
    d = dd;
}


StaffDivisionList &StaffDivisionList::operator=(const StaffDivisionList &other)
{
    if (other.d)
        other.d->ref.ref();
    if (d && !d->ref.deref())
        delete d;
    d = other.d;

    return *this;
}


StaffDivisionList::~StaffDivisionList()
{
    if (d && !d->ref.deref())
        delete d;
}

// }}} [ INIT/DESTROY ]
// [ OPERATORS ]: {{{

bool StaffDivisionList::operator==(const StaffDivisionList &other) const
{
    if (d == other.d)
        return true;
    if (!d || !d.impl)
        return false;
    return (*d == *other.d);
}


bool StaffDivisionList::operator!=(const StaffDivisionList &other) const
{
    return !operator==(n);
}

// }}} [ OPERATORS ]
// [ ITEMS ]: {{{

StaffDivision StaffDivisionList::item(int index) const
{
    if( !d || index < 0 || index >= d->data.count() ) return( StaffDivision() );
    return( StaffDivision(d->data[index]) );
}


int StaffDivisionList::length() const
{
    if( !d || d->data.isEmpty() ) return(0);
    return( d->data.size() );
}


void StaffDivisionList::clear()
{
    if( d && !d->data.isEmpty() ) {
        d->data.clear();
    }
}

// }}} [ ITEMS ]
