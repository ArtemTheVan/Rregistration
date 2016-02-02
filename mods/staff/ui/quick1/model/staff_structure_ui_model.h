#ifndef STAFF_STRUCTURE_UI_MODEL_H
#define STAFF_STRUCTURE_UI_MODEL_H

#include <QtCore>


class StaffObject;
class StaffDivision;

class StaffUIPathModel;
class StaffStructureUIModelPrivate;

class StaffStructureUIModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum ObjectRoles {
        NameFullRole        = Qt::DisplayRole,
        DescrFirstRole      = Qt::UserRole + 1,
        DescrSecondRole     = Qt::UserRole + 2,
        NameRole            = Qt::UserRole + 3,
        TypeRole            = Qt::UserRole + 4,
        ImageRole           = Qt::UserRole + 5
    };

public:
    explicit StaffStructureUIModel(QObject *parent = 0);
    ~StaffStructureUIModel();

    void initData();

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearData();
    Q_INVOKABLE void updateData();

public:
    // [ SWITCH MODEL METHODS ]:
    Q_INVOKABLE bool toChildDivision(const QString& name);
    Q_INVOKABLE void parentDivision();
    Q_INVOKABLE bool toParentDivision(const QString& name);

    // [OTHER METHODS]:
    Q_INVOKABLE QString rootElementName() const;
    Q_INVOKABLE QString rootElementNameFull() const;

public:
    // [ PROPERTIES ]:
    int count() const;

    // [ MODEL DATA ]:
    StaffDivision modelRootElement() const;
    StaffObject modelDataFromIndex(int index) const;

Q_SIGNALS:
    void countChanged();

protected:
    QScopedPointer<StaffStructureUIModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE_D(StaffStructureUIModel::d_ptr.data(), StaffStructureUIModel)
    Q_DISABLE_COPY(StaffStructureUIModel)
};

#endif // STAFF_STRUCTURE_UI_MODEL_H
