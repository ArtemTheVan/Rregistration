#ifndef ESU_REGISTRATION_TABLE_MODEL_H
#define ESU_REGISTRATION_TABLE_MODEL_H

#include <QAbstractListModel>

#include "esu_registration_models.h"
#include "esu_registration_engine_types.h"


class ESURegistrationTableManager;

class ESURegistrationTableModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)

public:
    enum Roles {
        ProfileRole         = Qt::DisplayRole,
        NameRole            = Qt::UserRole + 1,
        AddressRole         = Qt::UserRole + 2,
        IndexRole           = Qt::UserRole + 3,
        TimeRole            = Qt::UserRole + 4,
        CheckedRole         = Qt::UserRole + 5
    };

public:
    ESURegistrationTableModel(QObject *parent = 0);
    ~ESURegistrationTableModel();

    void setDataSource(ESURegistrationTableManager* source);

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void updateData();
    Q_INVOKABLE void clearData();

public:
    // [ PROPERTIES ]:
    int count() const;

    bool checkable() const;
    void setCheckable(bool checkable);

protected Q_SLOTS:
    void onTableUpdated();

Q_SIGNALS:
    void countChanged();
    void checkableChanged();

private:
    bool m_checkable;
    ESURegistrationTableManager* m_dataSource;

    QHash<int, QByteArray> m_roleNames;
    RegistrationTableData m_data;
};

#endif // ESU_REGISTRATION_TABLE_MODEL_H
