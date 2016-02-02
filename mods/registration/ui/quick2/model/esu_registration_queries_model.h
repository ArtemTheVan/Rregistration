#ifndef ESU_REGISTRATION_QUERIES_MODEL_H
#define ESU_REGISTRATION_QUERIES_MODEL_H

#include <QAbstractListModel>
#include <QDateTime>

#include "esu_registration_data.h"



class ESURegistrationQueriesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)

public:
    enum Roles {
        BaseRole            = Qt::DisplayRole,
        NameRole            = Qt::UserRole + 1,
        IndexRole           = Qt::UserRole + 2,
        TimeRole            = Qt::UserRole + 3,
        AddressRole         = Qt::UserRole + 4,
        RequestRole         = Qt::UserRole + 5,
        RequestStringRole   = Qt::UserRole + 6,
        MessageRole         = Qt::UserRole + 7,
        CheckedRole         = Qt::UserRole + 8
    };

public:
    explicit ESURegistrationQueriesModel(QObject* parent = nullptr);
    virtual ~ESURegistrationQueriesModel();

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearData();
    Q_INVOKABLE bool isEmpty() const;

    void setData(const QList<RegistrationUserInfo>& data);
    QList<RegistrationUserInfo> modelData() const;

public:
    // [ PROPERTIES ]:
    int count() const;

    bool checkable() const;
    void setCheckable(bool checkable);

public Q_SLOTS:
    // [ MODEL DATA METHODS ]:
    void appendData(const QString& role, const QString& name, const QString& addr, int time);
    void appendData(const RegistrationUserInfo& info);

    bool containsRole(const QString& role) const;
    bool containsAddress(const QString& address) const;

    void setChecked(bool checked, int index);
    void updateData(const RegistrationUserInfo& info, int index);

    void remove(int index = 0, int cnt = 1);
    void removeFrom(int index = 0);
    void removeByRole(const QString& role);
    void removeByAddress(const QString& address);
    void removeLast();
    void removeFirst();

    QString getAddress(int index = 0);
    QString getName(int index = 0);
    QString getRole(int index = 0);

Q_SIGNALS:
    void countChanged();
    void checkableChanged();

protected:
    QHash<int, QByteArray> m_roleNames;
    bool m_checkable;

public:
    QList<RegistrationUserInfo> m_modelData;
};

#endif // ESU_REGISTRATION_QUERIES_MODEL_H
