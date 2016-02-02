#ifndef STAFFUIMODEL_H
#define STAFFUIMODEL_H

#include <QtCore>


class StaffDivision;
class StaffUIModelPrivate;

class StaffUIModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        NameFullRole        = Qt::DisplayRole,
        DescrFirstRole      = Qt::UserRole + 1,
        DescrSecondRole     = Qt::UserRole + 2,
        NameRole            = Qt::UserRole + 3,
        TypeRole            = Qt::UserRole + 4,
        ImageRole           = Qt::UserRole + 5,
        AddressRole         = Qt::UserRole + 6
    };

public:
    explicit StaffUIModel(QObject *parent = 0);
    ~StaffUIModel();

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearData();
    Q_INVOKABLE void updateData();

public:
    // [ SWITCH MODEL METHODS ]:
    Q_INVOKABLE bool toChildDivision(const QString& name);
    Q_INVOKABLE bool parentDivision();
    Q_INVOKABLE void toParentDivision(const QString& name);

    // [MODEL INFO METHODS]:
    Q_INVOKABLE QString rootElementName() const;
    Q_INVOKABLE QString rootElementNameFull() const;

public:
    // [ PROPERTIES ]:
    int count() const;

    // [ MODEL DATA ]:
    void setModelData(const StaffDivision& data);
    void updateModel(const StaffDivision& data);

Q_SIGNALS:
    void countChanged();

protected:
    const QScopedPointer<StaffUIModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(StaffUIModel)
    Q_DISABLE_COPY(StaffUIModel)
};

#endif // STAFFUIMODEL_H
