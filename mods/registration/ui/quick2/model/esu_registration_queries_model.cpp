#include "esu_registration_queries_model.h"

#include <QtCore>


// [ INIT/DESTROY ]: {{{

ESURegistrationQueriesModel::ESURegistrationQueriesModel(QObject *parent):
    QAbstractListModel(parent)
{
    m_roleNames[BaseRole]           = "baseRole";
    m_roleNames[NameRole]           = "nameRole";
    m_roleNames[IndexRole]          = "indexRole";
    m_roleNames[TimeRole]           = "timeRole";
    m_roleNames[AddressRole]        = "addressRole";
    m_roleNames[RequestRole]        = "requestRole";
    m_roleNames[RequestStringRole]  = "requestStringRole";

    m_checkable = false;
}


ESURegistrationQueriesModel::~ESURegistrationQueriesModel()
{

}

// }}} [ INIT/DESTROY ]
// [ MODEL INTERFACE ]: {{{

QHash<int, QByteArray> ESURegistrationQueriesModel::roleNames() const
{
    return( m_roleNames );
}


int ESURegistrationQueriesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return( (m_modelData.isEmpty())?(0):(m_modelData.count()) );
}


QVariant ESURegistrationQueriesModel::data(const QModelIndex &index, int role) const
{
    do {
        int i = index.row();
        if( i < 0 || m_modelData.isEmpty() || i >= m_modelData.count() ) break;

        const RegistrationUserInfo& item = m_modelData[i];

        switch (role)
        {
        case IndexRole:     return(QVariant( i )); break;
        case BaseRole:      return(QVariant( item.role )); break;
        case NameRole:      return(QVariant( item.name )); break;
        case AddressRole:   return(QVariant( item.address.toString() )); break;
        case TimeRole:      return(QVariant( item.formattedTimeString() )); break;
        case RequestRole:   return(QVariant( (int)item.request )); break;
        case RequestStringRole:
        {
            QString outReq;
            if( item.request == registration::RegistrationRequest )
                outReq = "Регистрация";
            else if( item.request == registration::ChangeProfileRequest ) {
                outReq = "Смена профиля";
            }
            return(QVariant( outReq ));
            break;
        }
        case MessageRole:   return(QVariant( item.message )); break;
        case CheckedRole:   return(QVariant( item.ui.checked )); break;
        default: break;
        }
    }
    while(0);

    return(QVariant());
}


void ESURegistrationQueriesModel::clearData()
{
    if( m_modelData.isEmpty() ) return;

    beginResetModel();
    m_modelData.clear();
    endResetModel();

    Q_EMIT countChanged();
}


bool ESURegistrationQueriesModel::isEmpty() const
{
    return( m_modelData.isEmpty() );
}


void ESURegistrationQueriesModel::setData(const QList<RegistrationUserInfo> &data)
{
    if( data.isEmpty() ) return;

    beginResetModel();
    m_modelData = data;
    endResetModel();

    Q_EMIT countChanged();
}


QList<RegistrationUserInfo> ESURegistrationQueriesModel::modelData() const
{
    return( m_modelData );
}

// }}} [ MODEL INTERFACE ]
// [ PROPERTIES ]: {{{

int ESURegistrationQueriesModel::count() const
{
    return( (m_modelData.isEmpty())?(0):(m_modelData.count()) );
}


bool ESURegistrationQueriesModel::checkable() const
{
    return( m_checkable );
}


void ESURegistrationQueriesModel::setCheckable(bool checkable)
{
    if( m_checkable != checkable ) {
        m_checkable = checkable;

        Q_EMIT checkableChanged();
        Q_EMIT dataChanged( index(0), index(m_modelData.count()) );
    }
}


// }}} [ PROPERTIES ]
// [ MODEL DATA METHODS ]: {{{

void ESURegistrationQueriesModel::appendData(const QString &role, const QString &name, const QString &addr, int time)
{
    RegistrationUserInfo item(role, addr, name, time);

    if( m_modelData.contains(item) ) {
        qWarning() << "Contains item: " << item.name;
        return;
    }

    int pos = m_modelData.count();

    beginInsertRows(QModelIndex(), pos, pos);
    m_modelData.append(item);
    endInsertRows();

    Q_EMIT countChanged();
}


void ESURegistrationQueriesModel::appendData(const RegistrationUserInfo &info)
{
    if( m_modelData.contains(info) ) {
        qWarning() << "Contains item: " << info.name;
        return;
    }

    int pos = m_modelData.count();

    beginInsertRows(QModelIndex(), pos, pos);
    m_modelData.append(info);
    endInsertRows();

    Q_EMIT countChanged();
}


bool ESURegistrationQueriesModel::containsRole(const QString &role) const
{
    for( const RegistrationUserInfo& it: m_modelData ) {
        if( it.role == role ) return(true);
    }

    return( false );
}


bool ESURegistrationQueriesModel::containsAddress(const QString &address) const
{
    QHostAddress addr(address);
    for( const RegistrationUserInfo& it: m_modelData ) {
        if( it.address == addr ) return(true);
    }

    return( false );
}


void ESURegistrationQueriesModel::setChecked(bool checked, int index)
{
    if( index < 0 || m_modelData.isEmpty() || index >= m_modelData.count() )
        return;

    RegistrationUserInfo& u = m_modelData[index];
    if( u.ui.checked != checked ) {
        u.ui.checked = checked;
        Q_EMIT dataChanged(this->index(index), this->index(index));
    }
}


void ESURegistrationQueriesModel::updateData(const RegistrationUserInfo &info, int index)
{
    if( index < 0 || m_modelData.isEmpty() || index >= m_modelData.count() )
        return;

    m_modelData[index] = info;
    Q_EMIT dataChanged(this->index(index), this->index(index));
}


void ESURegistrationQueriesModel::remove(int index, int cnt)
{
    if( index < 0 || (index + cnt - 1) > count() || m_modelData.isEmpty() )
        return;

    beginRemoveRows(QModelIndex(), index, (index + cnt - 1));
    for(int i = 0; i < cnt; ++i)
        m_modelData.removeAt(i + index);
    endRemoveRows();

    Q_EMIT countChanged();
}


void ESURegistrationQueriesModel::removeFrom(int index)
{
    if( m_modelData.isEmpty() || index < 0 || index > (count()-1) )
        return;

    int last = m_modelData.count() - 1;
    beginRemoveRows(QModelIndex(), index, last);
    int i = last;
    while (i >= index) {
        m_modelData.removeAt(i);
        --i;
    }
    endRemoveRows();

    Q_EMIT countChanged();
}


void ESURegistrationQueriesModel::removeByRole(const QString &role)
{
    if( m_modelData.isEmpty() ) return;

    for(int i = 0; i < m_modelData.count(); ++i)
    {
        if( m_modelData[i].role == role ) {
            beginRemoveRows(QModelIndex(), i, i);
            m_modelData.removeAt(i);
            endRemoveRows();

            Q_EMIT countChanged();
            break;
        }
    }
}


void ESURegistrationQueriesModel::removeByAddress(const QString &address)
{
    if( m_modelData.isEmpty() ) return;

    for(int i = 0; i < m_modelData.count(); ++i)
    {
        if( m_modelData[i].addressString() == address ) {
            beginRemoveRows(QModelIndex(), i, i);
            m_modelData.removeAt(i);
            endRemoveRows();

            Q_EMIT countChanged();
            break;
        }
    }
}


void ESURegistrationQueriesModel::removeLast()
{
    remove(count() - 1);
}


void ESURegistrationQueriesModel::removeFirst()
{
    remove();
}

// [ GET MODEL DATA ]:

QString ESURegistrationQueriesModel::getAddress(int index)
{
    if( m_modelData.isEmpty() || index < 0 || index > (count()-1) )
        return QString();

    return m_modelData[index].addressString();
}


QString ESURegistrationQueriesModel::getName(int index)
{
    if( m_modelData.isEmpty() || index < 0 || index > (count()-1) )
        return QString();

    return m_modelData[index].name;
}


QString ESURegistrationQueriesModel::getRole(int index)
{
    if( m_modelData.isEmpty() || index < 0 || index > (count()-1) )
        return QString();

    return m_modelData[index].role;
}

// }}} [ MODEL DATA METHODS ]
