#ifndef STAFF_UI_PATH_MODEL_H
#define STAFF_UI_PATH_MODEL_H

#include <QtCore>


class StaffUIPathModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    struct ModelItem {
        ModelItem(const QString& name, const QString& fullName):
            name(name), nameFull(fullName) { }
        QString name;
        QString nameFull;
    };

    enum ModelRoles {
        NameRole            = Qt::DisplayRole,
        NameFullRole        = Qt::UserRole + 1,
        IndexRole           = Qt::UserRole + 2,
        ArrowRole           = Qt::UserRole + 3
    };

public:
    explicit StaffUIPathModel(QObject *parent = 0);
    ~StaffUIPathModel();

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearModel();

public:
    // [ PROPERTIES ]:
    int count() const;

    // [ MODEL METHODS ]:
    Q_INVOKABLE void add(const QString& name, const QString& nameFull);
    Q_INVOKABLE void remove(int index = 0, int cnt = 1);
    Q_INVOKABLE void removeFrom(int index = 0);
    Q_INVOKABLE void removeByName(const QString& name);
    Q_INVOKABLE void removeLast();
    Q_INVOKABLE void removeFirst();
    Q_INVOKABLE int itemIndex(const QString& name);

Q_SIGNALS:
    void countChanged();

private:
    QHash<int, QByteArray> m_roleNames;
    QList<ModelItem> m_data;
};

#endif // STAFF_UI_PATH_MODEL_H
