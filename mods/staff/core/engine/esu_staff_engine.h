#ifndef ESU_STAFF_ENGINE_H
#define ESU_STAFF_ENGINE_H

#include <QtCore>
#include <QtXml>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#endif

#include <QtNetwork/qhostaddress.h>

#ifdef ESU_APPLICATION
#include <esu_global.h>
#endif

#include "staff_object_p.h"
#include "staff_division.h"
#include "staff_soldier.h"
#include "staff_commander.h"



class ESUStaffEngine : public QObject
{
    Q_OBJECT

public:
    enum DocumentType { UnknownDocument, StaffDocument, StructureDocument };

public:
    explicit ESUStaffEngine(QObject *parent = 0);
    ~ESUStaffEngine();

public:
    bool readDocument();
    void readStructure(QDomElement &element, StaffDivision &division, int contNum = 0);
    void readStaff(QDomElement& element, StaffDivision& division);

public:
    bool nodesAddressContains(const QHostAddress& address);
    bool nodesAddressContains(StaffObjectPrivate* node);

public:
    QDomDocument currentDocument;
    StaffDivision rootDivision;
    QHash<QString, StaffObjectPrivate*> nodes;
    QHash<QHostAddress, StaffObjectPrivate*> nodesNetAddress;
    QHash<QString, StaffObjectPrivate*> nodesRegistered;

private:
    Q_FRIEND(StaffDivision)
    Q_FRIEND(StaffSoldier)
    Q_FRIEND(StaffCommander)
    Q_FRIEND(StaffBaseUnit)
    Q_FRIEND(StaffObject)
};

#endif // ESU_STAFF_ENGINE_H
