#ifndef ESUTZ_MILITARY_STAFF_P_H
#define ESUTZ_MILITARY_STAFF_P_H

#include "esu_staff_system.h"
#include "staff_division.h"

#include <QtXml>


class ESUStaffSystemPrivate
{
    Q_DECLARE_PUBLIC(ESUStaffSystem)

public:
    ESUStaffSystemPrivate(ESUStaffSystem* parent = nullptr);
    virtual ~ESUStaffSystemPrivate();

    void init();
    bool checkFiles();
    bool createStaffFile();

    bool loadStructure();
    bool loadStaff();

    bool createStaff(const StaffBaseUnit &unit, const QHostAddress &address);
    bool createStaff(const QByteArray& data);

    bool updateStaff(const QString& name, const QString& address, QByteArray* outData = nullptr);
    bool updateStaff(const QByteArray& data);

    void readDivisionStructure(QDomElement& e, StaffDivision& div, int contNum = 0);
    void readDivisionStaff(QDomElement& e, StaffDivision& division);
    void readStaffUpdate(QDomElement& newElement, QDomElement& rootElement, StaffDivision& division);
    void readStaffUnitAttributes(QDomElement& e, StaffBaseUnit& unit);

    void convertToDom(const StaffObject& object, QDomElement& parent, QDomDocument& docXML, QDomElement* updateData = nullptr);    

public:
    ESUStaffSystem* q_ptr;
    bool ready;

    ESUStaffStructure data;

    QString structureFile;
    QString staffFile;

    QDomDocument staffDocXML;
    QDomDocument structureDocXML;

    StaffDivision structureData;
    StaffDivision staffData;

    QHash<QString, StaffObjectPrivate*> structureNodes;
    QHash<QString, StaffObjectPrivate*> staffNodes;

private:
    Q_FRIEND(StaffDivision)
    Q_FRIEND(StaffSoldier)
    Q_FRIEND(StaffCommander)
    Q_FRIEND(StaffBaseUnit)
    Q_FRIEND(StaffObject)
};

#endif // ESUTZ_MILITARY_STAFF_P_H
