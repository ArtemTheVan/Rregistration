#ifndef ESU_REGISTRATION_TABLE_MANAGER_H
#define ESU_REGISTRATION_TABLE_MANAGER_H

#include "esu_registration_engine_types.h"
#ifdef ESU_NET_PROTO_SERVER
#include "esu_net_system_types.h"
#else
#include "esu_registration_types_net.h"
#endif
#include <memory>


class VariantTable;

class ESURegistrationTableManager : public QObject
{
    Q_OBJECT

public:
    enum FormatType { Compact, Indented };

public:
    explicit ESURegistrationTableManager(QObject *parent = 0);
    ~ESURegistrationTableManager();

public:
    static QVariantMap __createDBRecord(const RegistrationUserInfo& r);
    static void __fillDataFromDBRecord(RegistrationUserInfo& out, const QVariantMap& record);

public:
    // [ Content functions/properties ]:
    void addRecord(const RegistrationUserInfo& userInfo);
    void updateRecord(const QHostAddress& address, const RegistrationUserInfo& data);
    void updateRecord(const QString& role, const RegistrationUserInfo& data);

    bool containsRole(const QString& role);
    bool containsAddress(const QHostAddress& address);
    bool containsAddress(const QString& address);
    bool containsName(const QString& name);

    // [ Data/Content properties ]:
    inline RegistrationUserInfo &getRecord(int index);
    inline RegistrationUserInfo &operator[](int index);

    RegistrationUserInfo getRecordByRole(const QString& role) const;
    RegistrationUserInfo getRecordByAddress(const QHostAddress& address) const;
    RegistrationUserInfo getRecordByAddress(const QString& address) const;
    RegistrationUserInfo getRecordByName(const QString& name) const;

    bool removeRecordByAddress(const QHostAddress& address);
    bool removeRecordByAddress(const QString& address);
    bool removeRecordByRole(const QString& role);

    inline RegistrationTableData &contentData();

public:
    // [ Sync functions ]
    void syncData();
    void syncRecord(const QHostAddress& address);
    void syncAllData();

    void updateData(const RegistrationTableData& updateData);
    void updateData(const RegistrationPackageNET& p);

public:
    // [ Load functions ]:
    bool loadFromFile(const QString& filePath = QString());
    bool loadFromJSON(const QByteArray& dataJSON);
    bool loadFromXML(const QByteArray& dataXML);
    bool loadFromLocalDB();

    // [ Save functions ]:
    bool saveToFile(const QString& filePath = QString());
    bool saveToLocalDB();

    // [ Clear functions ]:
    void clearData();

    // [ Format functions ]:
    QByteArray convertToXML(FormatType format = Compact);
    QByteArray convertToJSON(FormatType format = Compact);

    static RegistrationTableData convertFromXML(const QByteArray& dataXML);
    static RegistrationTableData convertFromJSON(const QByteArray& dataJSON);

protected Q_SLOTS:
    void onUpdateDB();

Q_SIGNALS:
    void contentUpdated();

protected:
    bool m_ready;
    QString m_filePath;
    int m_lastSyncTime;
    RegistrationTableData m_tableData;

    // Local DB
    QVariantMap m_tableSceme;
#ifdef QT4_ETU
    VariantTable* m_dbTable;
#else
    std::shared_ptr<VariantTable> m_dbTable;
#endif
    bool m_dbReady;
    QTimer m_updateTimer;
};


inline RegistrationUserInfo &ESURegistrationTableManager::getRecord(int index)
{
    //if( m_tableData.isEmpty() || index < 0 || index >= m_tableData.count() )
    //    return(RegistrationRecordSpec());

    return( m_tableData[index] );
}


inline RegistrationUserInfo &ESURegistrationTableManager::operator[](int index)
{
    //if( m_tableData.isEmpty() || index < 0 || index >= m_tableData.count() )
    //    return(RegistrationRecordSpec());

    return( m_tableData[index] );
}


inline RegistrationTableData &ESURegistrationTableManager::contentData()
{
    return( m_tableData );
}

#endif // ESU_REGISTRATION_TABLE_MANAGER_H
