#ifndef ESU_STAFF_H
#define ESU_STAFF_H

#include "staff_globals.h"
#include "staff_division.h"

#include <QtNetwork/qhostaddress.h>


class ESUStaffStructurePrivate;

class ESUStaffStructure : public QObject
{
    Q_OBJECT

public:
    explicit ESUStaffStructure(QObject *parent = nullptr);
    ~ESUStaffStructure();

public:
    void create();
    bool load(const QString& fileName);
    void save(const QString& fileName = QString());
    void clear();

public:
    StaffDivision rootDivision();
    StaffDivision findDivisionByName(const QString& name);

    StaffBaseUnit findUnitByName(const QString& name);

public:
    bool isEmpty() const;
    bool contains(const QString& name);
    bool containsDivision(const QString& name);
    bool containsUnit(const QString& name);

protected:
    ESUStaffStructurePrivate* d;

private:
    Q_DISABLE_COPY(ESUStaffStructure)
    Q_DECLARE_PRIVATE_D(ESUStaffStructure::d, ESUStaffStructure)

    Q_FRIEND(ESUStaffEngine)
};

#endif // ESU_STAFF_H
