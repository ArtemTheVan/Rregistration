#ifndef STAFF_DIVISION_LIST_H
#define STAFF_DIVISION_LIST_H

#include "staff_division.h"


class StaffDivisionListPrivate;

class StaffDivisionList
{
public:
    StaffDivisionList();
    StaffDivisionList(const StaffDivisionList& other);
    ~StaffDivisionList();

    StaffDivisionList& operator= (const StaffDivisionList& other);
    bool operator== (const StaffDivisionList& other) const;
    bool operator!= (const StaffDivisionList& other) const;

    StaffDivision item(int index) const;
    inline StaffDivision operator[](int index) { return item(index); }
    inline StaffDivision at(int index) const { return item(index); }

    int length() const;
    inline int count() const { return length(); }
    inline int size() const { return length(); }
    inline bool isEmpty() const { return length() == 0; }

    void clear();

private:
    StaffDivisionListPrivate* d;
    StaffDivisionList(StaffDivisionListPrivate* dd);
};

#endif // STAFF_DIVISION_LIST_H
