#include "staff_ui_path_model.h"


// [ INIT/DESTROY ]: {{{

StaffUIPathModel::StaffUIPathModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roleNames[NameRole]       = "nameRole";
    m_roleNames[NameFullRole]   = "nameFullRole";
    m_roleNames[IndexRole]      = "indexRole";
    m_roleNames[ArrowRole]      = "arrowRole";
}


StaffUIPathModel::~StaffUIPathModel()
{

}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> StaffUIPathModel::roleNames() const
{
    return(m_roleNames);
}


int StaffUIPathModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if( m_data.isEmpty() ) return(0);
    return( m_data.count() );
}


QVariant StaffUIPathModel::data(const QModelIndex &index, int role) const
{
    do {
        int r = index.row();
        if( r < 0 || r > count() || m_data.isEmpty() ) break;
        const ModelItem& it = m_data[r];

        switch (role)
        {
        case NameRole:      return(QVariant(it.name)); break;
        case NameFullRole:  return(QVariant(it.nameFull)); break;
        case IndexRole:     return(QVariant(r)); break;
        case ArrowRole:
        {
            if( r == (m_data.count() - 1) ) return(QVariant(false));
            else return(QVariant(true));
            break;
        }
        default: break;
        }
    }
    while(0);

    return(QVariant());
}


void StaffUIPathModel::clearModel()
{
    if( m_data.isEmpty() ) return;

    beginResetModel();
    m_data.clear();
    endResetModel();

    Q_EMIT countChanged();
}

// }}} [ BASE MODEL METHODS ]
// [ PROPERTIES ]:

int StaffUIPathModel::count() const
{
    if( m_data.isEmpty() ) return(0);
    return( m_data.count() );
}

// }}} [ PROPERTIES ]
// [ MODEL METHODS ]: {{{

void StaffUIPathModel::add(const QString &name, const QString &nameFull)
{
    beginResetModel();
    m_data.append(ModelItem(name, nameFull));
    endResetModel();

    Q_EMIT countChanged();    
}


void StaffUIPathModel::remove(int index, int cnt)
{
    if( index < 0 || (index + cnt - 1) > count() || m_data.isEmpty() )
        return;

    //beginRemoveRows(QModelIndex(), index, (index + cnt - 1));
    beginResetModel();
    for(int i = 0; i < cnt; ++i)
        m_data.removeAt(i + index);
    endResetModel();
    //endRemoveRows();

    Q_EMIT countChanged();
    //Q_EMIT dataChanged(this->index(index), this->index(index + cnt - 1));
}


void StaffUIPathModel::removeFrom(int index)
{
    if( m_data.isEmpty() || index < 0 || index > (count()-1) )
        return;

    //beginRemoveRows(QModelIndex(), index, (count() - 1));
    beginResetModel();
    int i = count() - 1;
    while (i >= index) {
        m_data.removeAt(i);
        --i;
    }
    endResetModel();
    //endRemoveRows();

    Q_EMIT countChanged();
}


void StaffUIPathModel::removeByName(const QString &name)
{
    if( !m_data.isEmpty() ) {
        for(int i = 0; i < m_data.count(); ++i) {
            if( m_data[i].name == name || m_data[i].nameFull == name ) {
                //beginRemoveRows(QModelIndex(), i, i);
                beginResetModel();
                m_data.removeAt(i);
                endResetModel();
                //endRemoveRows();
                Q_EMIT countChanged();
                break;
            }
        }
    }
}


void StaffUIPathModel::removeLast()
{
    remove(count() - 1);
}


void StaffUIPathModel::removeFirst()
{
    remove();
}


int StaffUIPathModel::itemIndex(const QString &name)
{
    do {
        if( m_data.isEmpty() ) break;
        for(int i = 0; i < m_data.count(); ++i) {
            if( m_data[i].name == name || m_data[i].nameFull == name ) {
                return(i);
            }
        }
    }
    while(0);

    return(-1);
}

// }}} [ MODEL METHODS ]
