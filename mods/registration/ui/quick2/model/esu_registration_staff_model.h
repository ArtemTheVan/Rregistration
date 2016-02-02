#ifndef ESU_REGISTRATION_STAFF_MODEL_H
#define ESU_REGISTRATION_STAFF_MODEL_H

#include <QAbstractListModel>


class StaffDivision;
class ESURegistrationStaffModelPrivate;
class ESURegistrationTableManager;
class ESURegistrationPathModel;

class ESURegistrationStaffModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)

public:
    enum Roles {
        RoleNameFullRole    = Qt::DisplayRole,
        DescrFirstRole      = Qt::UserRole + 1,
        DescrSecondRole     = Qt::UserRole + 2,
        RoleBaseRole        = Qt::UserRole + 3,
        NameRole            = Qt::UserRole + 4,
        TypeRole            = Qt::UserRole + 5,
        TypeString          = Qt::UserRole + 6,
        ImageRole           = Qt::UserRole + 7,
        AddressRole         = Qt::UserRole + 8,
        RegisteredRole      = Qt::UserRole + 9,
        IndexRole           = Qt::UserRole + 10
    };

public:
    explicit ESURegistrationStaffModel(QObject *parent = 0);
    ~ESURegistrationStaffModel();

    void setDataSource(ESURegistrationTableManager* source);

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearData();
    Q_INVOKABLE void updateData();

public:
    // [ SWITCH MODEL METHODS ]:
    Q_INVOKABLE bool toChildDivision(const QString& name);
    Q_INVOKABLE bool setParentDivision();
    Q_INVOKABLE void toParentDivision(const QString& name);

    // [MODEL INFO METHODS]:
    Q_INVOKABLE QString rootElementName() const;
    Q_INVOKABLE QString rootElementNameFull() const;

public:
    // [ PROPERTIES ]:
    int count() const;

    bool checkable() const;
    void setCheckable(bool checkable);

    // [ MODEL DATA ]:
    void setModelData(const StaffDivision& data);

    ESURegistrationPathModel *pathModel();

public Q_SLOTS:
    void onContentUpdated();

Q_SIGNALS:
    void countChanged();
    void checkableChanged();

protected:
    const QScopedPointer<ESURegistrationStaffModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(ESURegistrationStaffModel)
    Q_DISABLE_COPY(ESURegistrationStaffModel)
};

#endif // ESU_REGISTRATION_STAFF_MODEL_H
