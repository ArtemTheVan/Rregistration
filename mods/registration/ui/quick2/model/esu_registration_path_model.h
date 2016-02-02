#ifndef ESU_REGISTRATION_PATH_MODEL_H
#define ESU_REGISTRATION_PATH_MODEL_H

#include <QAbstractListModel>


class ESURegistrationPathModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum ModelRoles {
        NameRole            = Qt::DisplayRole,
        NameFullRole        = Qt::UserRole + 1,
        IndexRole           = Qt::UserRole + 2,
        ArrowRole           = Qt::UserRole + 3
    };

public:
    explicit ESURegistrationPathModel(QObject *parent = nullptr);
    ~ESURegistrationPathModel();

public:
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void clearModel();

    void setData(const QStringList& data);

public:
    // [ PROPERTIES ]:
    int count() const;

public Q_SLOTS:
    // [ MODEL METHODS ]:
    void add(const QString& name);
    void remove(int index = 0, int cnt = 1);
    void removeFrom(int index = 0);
    void removeByName(const QString& name);
    void removeLast();
    void removeFirst();
    int itemIndex(const QString& name);

Q_SIGNALS:
    void countChanged();

private:
    QHash<int, QByteArray> m_roleNames;
    QStringList m_data;
};

#endif // ESU_REGISTRATION_PATH_MODEL_H
