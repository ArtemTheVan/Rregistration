#include "esu_registration_path_model.h"


// [ INIT/DESTROY ]: {{{

ESURegistrationPathModel::ESURegistrationPathModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roleNames[NameRole]       = "nameRole";
    m_roleNames[NameFullRole]   = "nameFullRole";
    m_roleNames[IndexRole]      = "indexRole";
    m_roleNames[ArrowRole]      = "arrowRole";
}


ESURegistrationPathModel::~ESURegistrationPathModel()
{

}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> ESURegistrationPathModel::roleNames() const
{
    return(m_roleNames);
}


int ESURegistrationPathModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if( m_data.isEmpty() ) return(0);
    return( m_data.count() );
}


QVariant ESURegistrationPathModel::data(const QModelIndex &index, int role) const
{
    do {
        int i = index.row();
        if( i < 0 || i > count() || m_data.isEmpty() ) break;

        switch (role)
        {
        case NameRole:      return(QVariant( m_data[i] )); break;
        //case NameFullRole:  return(QVariant(it.nameFull)); break;
        case IndexRole:     return(QVariant(i)); break;
        case ArrowRole:
            if( i == (m_data.count() - 1) ) return(QVariant(false));
            else return(QVariant(true));
            break;
        default: break;
        }
    }
    while(0);

    return(QVariant());
}


void ESURegistrationPathModel::clearModel()
{
    if( m_data.isEmpty() ) return;

    beginResetModel();
    m_data.clear();
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationPathModel::setData(const QStringList &data)
{
    beginResetModel();
    m_data = data;
    endResetModel();

    Q_EMIT countChanged();
}

// }}} [ BASE MODEL METHODS ]
// [ PROPERTIES ]:

int ESURegistrationPathModel::count() const
{
    if( m_data.isEmpty() ) return(0);
    return( m_data.count() );
}

// }}} [ PROPERTIES ]
// [ MODEL METHODS ]: {{{

void ESURegistrationPathModel::add(const QString &name)
{
    beginResetModel();
    m_data.append( name );
    endResetModel();

    Q_EMIT countChanged();    
}


void ESURegistrationPathModel::remove(int index, int cnt)
{
    if( index < 0 || (index + cnt - 1) > count() || m_data.isEmpty() )
        return;

    beginResetModel();
    for(int i = 0; i < cnt; ++i)
        m_data.removeAt(i + index);
    endResetModel();    

    Q_EMIT countChanged();
}


void ESURegistrationPathModel::removeFrom(int index)
{
    if( m_data.isEmpty() || index < 0 || index > (count()-1) )
        return;

    beginResetModel();
    int i = count() - 1;
    while (i >= index) {
        m_data.removeAt(i);
        --i;
    }
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationPathModel::removeByName(const QString &name)
{
    if( m_data.isEmpty() ) return;

    for(int i = 0; i < m_data.count(); ++i) {
        if( m_data[i] == name ) {
            beginResetModel();
            m_data.removeAt(i);
            endResetModel();
            Q_EMIT countChanged();
            break;
        }
    }
}


void ESURegistrationPathModel::removeLast()
{
    remove(count() - 1);
}


void ESURegistrationPathModel::removeFirst()
{
    remove();
}


int ESURegistrationPathModel::itemIndex(const QString &name)
{
    do {
        if( m_data.isEmpty() ) break;        
        for(int i = 0; i < m_data.count(); ++i) {
            if( m_data[i] == name ) {
                return(i);
            }
        }
    }
    while(0);

    return(-1);
}

// }}} [ MODEL METHODS ]
