#include "staff_globals.h"

#include <QtCore>
#include <QtXml>
#include <vector>

using namespace std;
using namespace staff;

// [ namespace ]:[ staff ]: {{{

// [ GLOBALS ]: {{{

namespace staff {

static QHash<QString, UnitDuty> unitDutyData;
static QHash<QString, UnitRank> unitRankData;
static QHash<QString, DivisionType> divisionTypeData;
static QHash<QString, QString> dutyNameDescrData;
static QHash<QString, QString> divisionNameDescrData;

RankCategory getRankCategoryByRank(UnitRank rank)
{
    if     ((rank >= SoldierRank)                && (rank < JuniorSergeantRank))         return SoldierCategory;
    else if((rank >= JuniorSergeantRank)         && (rank < JuniorLieutenantGroundRank)) return SergeantCategory;
    else if((rank >= JuniorLieutenantGroundRank) && (rank < RankCount))                  return OfficerCategory;
}

static vector<int> divSizeVector()
{
    vector<int> v(DivisionTypeCount, 0);
    v[BathDivision     ] =    5;
    v[CommandDivision  ] =   40;
    v[SquadDivision    ] =   10;
    v[PlatoonDivision  ] =   30;
    v[CompanyDivision  ] =  100;
    v[BattalionDivision] =  700;
    v[BrigadeDivision  ] = 3000;
    v[BatteryDivision  ] = 6000;

    return v;
}

int getDivisionSize(DivisionType divType)
{
    static vector<int> v = divSizeVector();
    return(v[divType]);
}

}

// }}} [ GLOBALS ]
// [ STATIC METHODS ]: {{{

static void init(QHash<QString, staff::UnitDuty>& data)
{
    /* [ Командиры ] */
    data["Старшина"]                = SergeantMajorRank;
    data["Командир бани"]           = BathCommanderRank;
    data["Командир отделения"]      = SquadCommanderRank;
    data["Командир взвода"]         = PlatoonCommanderRank;
    data["Командир роты"]           = CompanyCommanderRank;
    data["Командир батальона"]      = BattalionCommanderRank;
    data["Командир бригады"]        = BrigadeCommanderRank;
    data["Командир батареи"]        = BatteryCommanderRank;

    /* [ Замком-ы ] */
    data["Зам командира отделения"] = SquadAssistantRank;
    data["Зам командира взвода"]    = PlatoonAssistantRank;
    data["Зам командира роты"]      = CompanyAssistantRank;
    data["Зам командира батальона"] = BattalionAssistantRank;

    /* [ Стрелки/Пулемтичики ] */
    data["Пулеметчик"]      = MachineGunnerRank;
    data["Стрелок"]         = ShooterRank;
    data["Старший стрелок"] = SeniorShooterRank;
    data["Стрелок-санитар"] = MedicShooterRank;
    data["Снайпер"]         = SniperRank;

    /* [ Артиллерийская часть ] */
    data["Гранатометчик"]           = GrenadeThrowerRank;
    data["Помощник гранатометчика"] = GrenadeThrowerHelperRank;
    data["Наводчик"]                = DialManRank;

     /* [ Водители/Машинисты ] */
    data["Водитель"]    = DriverRank;
    data["Водитель-66"] = Driver66Rank;

    /* [ Медицинский штат ] */
    data["Санитар"] = AidmanRank;

    /* [ Технический штат ] */
    data["Техник"]                      = TechnicianRank;
    data["Радио-телефонист"]            = RadioTelephoneOperatorRank;
    data["Оператор ближней разведки"]   = NearScoutingOperatorRank;
}

static void init(QHash<QString, staff::UnitRank>& data)
{
    /* [ Солдаты и матросы ] */
    // войсковые:
    data["Рядовой"]         = SoldierRank;
    data["Ефрейтор"]        = LanceCorporalRank;
    // корабельные:
    data["Матрос"]          = SeamanRank;
    data["Старший матрос"]  = SeniorSeamanRank;

    /* [ Сержанты и старшины ] */
    // войсковые:
    data["Младший сержант"] = JuniorSergeantRank;
    data["Сержант"]         = SergeantRank;
    data["Старший сержант"] = SeniorSergeantRank;
    data["Старшина"]        = MasterSergeantRank;
    // корабельные:
    data["Старшина 1-ой статьи"]            = PettyOfficerFirstClassRank;
    data["Старшина 2-ой статьи"]            = PettyOfficerSecondClassRank;
    data["Главный старшина"]                = ChiefMateRank;
    data["Главный корабельный старшина"]    = MasterChiefPettyOfficerRank;

    /* [ Прапорщики и мичманы ] */
    // войсковые:
    data["Прапорщик"]           = WarrantOfficerGroundRank;
    data["Старший прапорщик"]   = SeniorWarrantOfficerGroundRank;
    // корабельные:
    data["Мичман"]              = WarrantOfficerMarineRank;
    data["Старший мичман"]      = SeniorWarrantOfficerMarineRank;

    /* [ Младшие офицеры ] */
    // войсковые:
    data["Младший лейтенант (войск)"]   = JuniorLieutenantGroundRank;
    data["Лейтенант (войск)"]           = LieutenantGroundRank;
    data["Старший лейтенант (войск)"]   = SeniorLieutenantGroundRank;
    data["Капитан (войск)"]             = CaptainRank;
    // корабельные:
    data["Младший лейтенант (морск)"]   = JuniorLieutenantMarineRank;
    data["Лейтенант (морск)"]           = LieutenantMarineRank;
    data["Старший лейтенант (морск)"]   = SeniorLieutenantMarineRank;
    data["Капитан (морск)"]             = CaptainLieutenantRank;

    /* [ Старшие офицеры ] */
    // войсковые:
    data["Майор"]                   = MajorRank;
    data["Подполковник"]            = HalfColonelRank;
    data["Полковник"]               = ColonelRank;
    // корабельные:
    data["Капитан первого ранга"]   = CaptainFirstRank;
    data["Капитан второго ранга"]   = CaptainSecondRank;
    data["Капитан третьего ранга"]  = CaptainThirdRank;

     /* [ Высшие офицеры ] */
    // войсковые:
    data["Генерал-майор"]       = BrigadierGeneralRank;
    data["Генерал-лейтенант"]   = LieutenantGeneralRank;
    data["Генерал-полковник"]   = ColonelGeneralRank;
    data["Генерал армии"]       = ArmyGeneralRank;
    data["Маршал"]              = MarshallRank;
    // корабельные:
    data["Контр-адмирал"]       = RearAdmiralLowerHalfRank;
    data["Вице-адмирал"]        = ViceAdmiralRank;
    data["Адмирал флота"]       = FleetAdmiralRank;
}


static void init(QHash<QString, staff::DivisionType>& data)
{
    data["баня"]        = BathDivision;
    data["отделение"]   = SquadDivision;
    data["взвод"]       = PlatoonDivision;
    data["рота"]        = CompanyDivision;
    data["батальон"]    = BattalionDivision;
    data["дивизион"]    = BattalionDivision;
    data["бригада"]     = BrigadeDivision;    
    data["батарея"]     = BatteryDivision;
}


static bool staff::loadDescription()
{
    QHash<QString, QString>& dutyNameDescr = dutyNameDescrData;
    QHash<QString, QString>& divNameDescr = divisionNameDescrData;
    QString key;
    QString value;

    QDomDocument docXml("description");
    QFile fileXml("./../../conf/staff_info.xml");

    if( !fileXml.exists() || !fileXml.open(QIODevice::ReadOnly) )
        return(false);

    if( !docXml.setContent(&fileXml) ) {
        fileXml.close();
        return(false);
    }

    QDomElement rootElem = docXml.documentElement();
    if( rootElem.isNull() ) return(false);
    QDomNode node = rootElem.firstChild();

    while( !node.isNull()  )
    {
        QDomElement descrGroupElement = node.toElement();
        if( descrGroupElement.isNull() ) {            
            node = node.nextSibling();
            continue;
        }
        //qDebug() << descrGroupElement.tagName();

        if( descrGroupElement.tagName() == QString("DivisionList") )
        {
            QDomNode divNode = descrGroupElement.firstChild();
            while( !divNode.isNull() ) {
                QDomElement divElem = divNode.toElement();
                if( !divElem.isNull() ) {
                    key = divElem.attribute("key");
                    value = divElem.text();
                    if( !key.isEmpty() && !value.isEmpty() )
                        divNameDescr.insert(key, value);
                }
                divNode = divNode.nextSibling();
            }
        }
        else if( descrGroupElement.tagName() == QString("DutyList") )
        {
            QDomNode dutyNode = descrGroupElement.firstChild();
            while( !dutyNode.isNull() ) {
                QDomElement dutyElem = dutyNode.toElement();
                if( !dutyElem.isNull() ) {
                    key = dutyElem.attribute("key");
                    value = dutyElem.text();
                    if( !key.isEmpty() && !value.isEmpty() ) {
                        dutyNameDescr.insert(key, value);                        
                    }
                }
                dutyNode = dutyNode.nextSibling();
            }
        }

        node = node.nextSibling();
    }

    return(true);
}

// }}} [ STATIC METHODS ]

// [1]

bool staff::isSecondInCommand(QString duty_name)
{
    if(duty_name == "ЗК") return(true);
    return(false);
}

UnitDuty staff::getSecondInCommandDuty(DivisionType div_type)
{
    switch (div_type) {
    case BathDivision:
    case SquadDivision:     return(SquadAssistantRank);
    case CommandDivision:
    case PlatoonDivision:   return(PlatoonAssistantRank);
    case CompanyDivision:   return(CompanyAssistantRank);
    case BattalionDivision: return(BattalionAssistantRank);
    default:                return(UnknownDuty);
    }
}

staff::UnitDuty staff::getUnitDutyByDescr(const QString &descr)
{
    UnitDuty result = UnknownDuty;

    if( unitDutyData.isEmpty() ) init(unitDutyData);
    if( unitDutyData.contains(descr) ) result = unitDutyData[descr];

    return( result );
}

staff::UnitRank staff::getUnitRankByDescr(const QString &descr)
{
    UnitRank result = UnknownRank;

    if( unitRankData.isEmpty() ) init(unitRankData);
    if( unitRankData.contains(descr) ) result = unitRankData[descr];

    return( result );
}


staff::DivisionType staff::getDivisionTypeByTypename(const QString &typeName)
{
    DivisionType result = UnknownDivision;

    if( divisionTypeData.isEmpty() ) init(divisionTypeData);
    if( typeName.isEmpty() || typeName.isNull() ) return( result );
    if( divisionTypeData.contains(typeName) ) result = divisionTypeData[typeName];

    return( result );
}


staff::UnitDuty staff::getCommanderDutyByDivType(DivisionType type)
{
    UnitDuty result = UnknownDuty;
    static QHash<DivisionType, UnitDuty> data;

    if( data.isEmpty() ) {
        data[BathDivision]      = BathCommanderRank;
        data[CommandDivision]   = UnknownDuty;
        data[SquadDivision]     = SquadCommanderRank;
        data[PlatoonDivision]   = PlatoonCommanderRank;
        data[CompanyDivision]   = CompanyCommanderRank;
        data[BattalionDivision] = BattalionCommanderRank;
        data[BrigadeDivision]   = BrigadeCommanderRank;
    }
    if( data.contains(type) ) result = data[type];

    return( result );
}


staff::UnitDuty staff::getUnitDutyByName(const QString &name)
{
    QString descr = getDutyDescrByName(name);
    if( descr.isEmpty() ) return( UnknownDuty );

    return( getUnitDutyByDescr(descr) );
}


staff::DivisionType staff::getDivisionTypeByName(const QString& name)
{
    QString descr = getDivisionDescrByName(name);
    if( descr.isEmpty() ) return( UnknownDivision );

    return( getDivisionTypeByTypename(descr) );
}

// [/1]
// [2]

QString staff::getDutyDescription(staff::UnitDuty duty)
{
    QString result;

    if( unitDutyData.isEmpty() ) init(unitDutyData);
    if( duty == UnknownDuty ) return( result );

    static QList<staff::UnitDuty> values = unitDutyData.values();
    if( values.contains(duty) ) result = unitDutyData.key(duty);

    return( result );
}


QString staff::getRankDescription(UnitRank rank)
{
    QString result;

    if( unitRankData.isEmpty() ) init(unitRankData);
    if( rank == UnknownRank ) return( result );

    static QList<staff::UnitRank> values = unitRankData.values();
    if( values.contains(rank) ) result = unitRankData.key(rank);

    return( result );
}


QString staff::getDivisionTypename(staff::DivisionType type)
{
    QString result;

    if( divisionTypeData.isEmpty() ) init(divisionTypeData);
    if( type == UnknownDivision ) return( result );

    static QList<staff::DivisionType> values = divisionTypeData.values();
    if( values.contains(type) )
        result = divisionTypeData.key(type);

    return( result );
}

// [/2]
// [3]

QString staff::getDutyDescrByName(const QString &name)
{
    QString result;

    if( dutyNameDescrData.isEmpty() )
        loadDescription();
    if( name.isEmpty() || name.isNull() )
        return( result );
    if( dutyNameDescrData.contains(name) )
        result = dutyNameDescrData[name];

    return( result );
}


QString staff::getDutyNameByDescr(const QString &descr)
{
    QString result;

    if( dutyNameDescrData.isEmpty() ) loadDescription();
    if( descr.isEmpty() || descr.isNull() ) return( result );

    static QStringList values = dutyNameDescrData.values();
    if( values.contains(descr) ) result = dutyNameDescrData.key(descr);

    return( result );
}


QString staff::getDivisionDescrByName(const QString &name)
{
    QString result;

    if( divisionNameDescrData.isEmpty() ) loadDescription();
    if( name.isEmpty() || name.isNull() ) return( result );
    if( divisionNameDescrData.contains(name) ) result = divisionNameDescrData[name];

    return( result );
}

// [/3]
// }}} [ namespace ]:[ staff ]

// [ namespace ]:[ staff_private ]: {{{

QString staff_private::formatName(const QString &name)
{
    QString result = name.trimmed();
#if QT_VERSION >= 0x050000
    result.remove(QRegularExpression("^[\\s.,-]+|[\\s.,-]+$"));
#else
    result.remove(QRegExp("^[\\s.,-]+|[\\s.,-]+$"));
#endif
    return(result);
}


QString staff_private::formatNameLeft(const QString &leftPart)
{
    QString result = leftPart.trimmed();
#if QT_VERSION >= 0x050000
    result.remove(QRegularExpression("^[\\s.,-0-9]+"));
#else
    result.remove(QRegExp("^[\\s.,-0-9]+"));
#endif
    return(result);
}


QString staff_private::formatNameRight(const QString &rightPart)
{
    QString result = rightPart.trimmed();
#if QT_VERSION >= 0x050000
    result.remove(QRegularExpression("[\\s.,-0-9]+$"));
#else
    result.remove(QRegExp("[\\s.,-0-9]+$"));
#endif
    return(result);
}

// }}} [ namespace ]:[ staff_private ]
