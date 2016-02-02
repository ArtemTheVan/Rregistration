#ifndef STAFF_GLOBALS_H
#define STAFF_GLOBALS_H

#include <QtGlobal>
#include <QtCore/qstring.h>
#include <QtCore/qhash.h>


#ifndef ESU_APPLICATION
    #define ESU_STAFF_ID                                0x00000040
    #define ESU_STAFF_NAME_ID                           "ESUMilitaryStaff"
    #define ESU_STAFF_UI_ID                             0x00000041
    #define ESU_STAFF_UI_NAME_ID                        "ESUMilitaryStaffUI"

    #ifndef Q_FRIEND
    #define Q_FRIEND(Class) friend class Class;
    #endif
#else
    #include "esu_global.h"
    #include "esu_globals_id.h"
#endif


namespace staff
{
    /* Должность */
    enum UnitDuty
    {
        /* [ Командиры ] */
        SergeantMajorRank,                  // Старшина
        BathCommanderRank,                  // Командир бани
        SquadCommanderRank,                 // Командир отделения
        PlatoonCommanderRank,               // Командир взвода
        CompanyCommanderRank,               // Командир роты
        BattalionCommanderRank,             // Командир батальона
        BrigadeCommanderRank,               // Командир бригады
        BatteryCommanderRank,               // Командир батареи
        /* [ Замком-ы ] */
        SquadAssistantRank,                 // Зам командира отделения
        PlatoonAssistantRank,               // Зам командира взвода
        CompanyAssistantRank,               // Зам командира роты
        BattalionAssistantRank,             // Зам командира батальона
        /* [ Стрелки/Пулемтичики/Пехота ] */
        MachineGunnerRank,                  // Пулеметчик
        ShooterRank,                        // Стрелок
        SeniorShooterRank,                  // Старший стрелок
        MedicShooterRank,                   // Стрелок-санитар
        SniperRank,                         // Снайпер
        /* [ Артиллерийская часть ] */
        GrenadeThrowerRank,                 // Гранатометчик
        GrenadeThrowerHelperRank,           // Помощник гранатометчика
        DialManRank,                        // Наводчик
        /* [ Водители/Машинисты ] */
        DriverRank,                         // Водитель
        Driver66Rank,                       // Водитель-66
        /* [ Медицинский штат ] */
        AidmanRank,                         // Санитар
        /* [ Технический штат ] */
        TechnicianRank,                     // Техник
        RadioTelephoneOperatorRank,         // Радио-телефонист
        NearScoutingOperatorRank,           // Оператор ближней разведки        
        /* [ Кол-во должностей ] */
        DutyCount,
        UnknownDuty
    };

    /* Звание */
    enum UnitRank
    {
        UnknownRank,
        /* [ Солдаты и матросы ] */
        // войсковые:
        SoldierRank,                    // Рядовой (курсант)
        LanceCorporalRank,              // Ефрейтор
        // корабельные:
        SeamanRank,                     // Матрос (курсант)
        SeniorSeamanRank,               // Старший матрос
        /* [ Сержанты и старшины ] */
        // войсковые:
        JuniorSergeantRank,             // Младший сержант
        SergeantRank,                   // Сержант
        SeniorSergeantRank,             // Старший сержант
        MasterSergeantRank,             // Старшина
        // корабельные:
        PettyOfficerFirstClassRank,     // Старшина 1-ой статьи
        PettyOfficerSecondClassRank,    // Старшина 2-ой статьи
        ChiefMateRank,                  // Главный старшина
        MasterChiefPettyOfficerRank,    // Главный корабельный старшина
        /* [ Прапорщики и мичманы ] */
        // войсковые:
        WarrantOfficerGroundRank,       // Прапорщик
        SeniorWarrantOfficerGroundRank, // Старший прапорщик
        // корабельные:
        WarrantOfficerMarineRank,       // Мичман
        SeniorWarrantOfficerMarineRank, // Старший мичман
        /* [ Младшие офицеры ] */
        // войсковые:
        JuniorLieutenantGroundRank,     // Младший лейтенант
        LieutenantGroundRank,           // Лейтенант
        SeniorLieutenantGroundRank,     // Старший лейтенант
        CaptainRank,                    // Капитан
        // корабельные:
        JuniorLieutenantMarineRank,     // Младший лейтенант
        LieutenantMarineRank,           // Лейтенант
        SeniorLieutenantMarineRank,     // Старший лейтенант
        CaptainLieutenantRank,          // Капитан-лейтенант
        /* [ Старшие офицеры ] */
        // войсковые
        OfficerRank,                    // Офицер
        MajorRank,                      // Майор
        HalfColonelRank,                // Подполковник
        ColonelRank,                    // Полковник
        // корабельные
        CaptainFirstRank,               // Капитан первого ранга
        CaptainSecondRank,              // Капитан второго ранга
        CaptainThirdRank,               // Капитан третьего ранга
        /* [ Высшие офицеры ] */
        // войсковые
        BrigadierGeneralRank,           // Генерал-майор
        LieutenantGeneralRank,          // Генерал-лейтенант
        ColonelGeneralRank,             // Генерал-полковник
        ArmyGeneralRank,                // Генерал армии
        MarshallRank,                   // Маршал
        // корабельные
        RearAdmiralLowerHalfRank,       // Контр-адмирал
        ViceAdmiralRank,                // Вице-адмирал
        FleetAdmiralRank,               // Адмирал флота
        /* [ Кол-во званий ] */
        RankCount
    };

    /* Категории */
    enum RankCategory {
        SoldierCategory,    // Рядовой (курсант)
        SergeantCategory,
        OfficerCategory,
        /* */
        RankCategoryCount,
        UnknownCategory
    };

    /* Учетные категории(для данных подразделения) */
    enum CountCategory {
        InState = 0,    // По штату
        InList,         // По списку
        IsPresent,      // Налицо
        IsDamaged,      // Раненные
        IsKilled,       // Убитые
        CountCategoryCount,
        UnknownCountCategory
    };

    /* Тип подразделения */
    enum DivisionType
    {
        BathDivision = 0,       // Баня
        CommandDivision = 1,    // Управление
        SquadDivision = 2,      // Отделение
        PlatoonDivision = 3,    // Взвод
        CompanyDivision = 4,    // Рота
        BattalionDivision = 5,  // Батальон/Дивизион
        BrigadeDivision = 6,    // Бригада
        BatteryDivision = 7,    // Батарея
        /* [ Кол-во типов подразделений ] */
        DivisionTypeCount,
        UnknownDivision
    };

    enum Health{ Healthy, Damaged, Killed, HealthStateCount, UnknownHealth };

    int             getDivisionSize(DivisionType divType);
    RankCategory    getRankCategoryByRank(UnitRank rank);
    //-----------------------------------------------------------------------------------------------
    /* Является ли сокращенное название должности зам. командира? Является = true, иначе - false. */
    bool            isSecondInCommand(QString duty_name);
    /* Определение должности зам. кома в зависимости от принадлежности к подразделению. */
    UnitDuty        getSecondInCommandDuty(DivisionType div_type);
    UnitDuty        getUnitDutyByDescr(const QString& descr);    
    UnitRank        getUnitRankByDescr(const QString& descr);    

    /*!
     * \brief getDivTypeByTypename -
     *  Возвращает перечисление <enum DivisionType> из его строкового представления.
     * \param typeName - Строковое представление типа подразделения.
     * \return - перечисление <enum DivisionType> - тип подразделения.
     */
    DivisionType    getDivisionTypeByTypename(const QString& typeName);
    //-----------------------------------------------------------------------------------------------
    UnitDuty        getCommanderDutyByDivType(DivisionType type);
    //-----------------------------------------------------------------------------------------------
    UnitDuty        getUnitDutyByName(const QString& name);    
    DivisionType    getDivisionTypeByName(const QString& name);
    //-----------------------------------------------------------------------------------------------
    QString         getDutyDescription(UnitDuty duty);
    QString         getRankDescription(UnitRank rank);

    /*!
     * \brief getDivisionTypename -
     *  Возращает строковое представление типа подразделения (отделение, взвод, рота и т.д.).
     *  Пример: staff::SquadDivision --> "Отделение".
     * \param type - перечисление <enum DivisionType> - тип подразделения.
     * \return - Строковое представление типа подразделения.
     */
    QString         getDivisionTypename(DivisionType type);
    //-----------------------------------------------------------------------------------------------
    QString         getDutyDescrByName(const QString& name);
    QString         getDutyNameByDescr(const QString& descr);

    /*!
     * \brief getDivisionDescrByName -
     *  Конвертирует сокращенное название подразделения в полное.
     *  Пример: "МСО"--> "Мотострелковое отделение"
     * \param name - сокращенное наименование подразделение (аббревиатура).
     * \return - Полное наименование подрразделения.
     */
    QString         getDivisionDescrByName(const QString& name);
    //-----------------------------------------------------------------------------------------------
} // namespace staff

namespace staff_private
{

QString formatName(const QString& name);

QString formatNameLeft(const QString& leftPart);
QString formatNameRight(const QString& rightPart);

} // namespace staff_private


#if !defined(CHECK_D) && !defined(CHECK_D_RET)
    #define CHECK_D(d) if( d == nullptr ) return
    #define CHECK_D_RET(d, r) if( d == nullptr ) return(r)
#endif

#endif // STAFF_GLOBALS_H
