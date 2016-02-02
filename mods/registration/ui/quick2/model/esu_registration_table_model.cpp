#include "esu_registration_table_model.h"

#include "esu_registration_table_manager.h"


// [ INIT/DESTROY ]: {{{

ESURegistrationTableModel::ESURegistrationTableModel(QObject *parent):
    QAbstractListModel(parent)
  , m_checkable(false), m_dataSource(nullptr)
{
    m_roleNames[ProfileRole]      = "profileRole";
    m_roleNames[NameRole]         = "nameRole";
    m_roleNames[AddressRole]      = "addressRole";
    m_roleNames[IndexRole]        = "indexRole";
    m_roleNames[TimeRole]         = "timeRole";
}


ESURegistrationTableModel::~ESURegistrationTableModel()
{
    this->disconnect();
    if( m_dataSource )
        m_dataSource->disconnect(this);
}


void ESURegistrationTableModel::setDataSource(ESURegistrationTableManager *source)
{
    Q_CHECK_PTR(source);

    m_dataSource = source;
    connect( source, &ESURegistrationTableManager::contentUpdated,
             this, &ESURegistrationTableModel::onTableUpdated, Qt::QueuedConnection );

    updateData();
}

// }}} [ INIT/DESTROY ]
// [ DATA MODEL ]: {{{

QHash<int, QByteArray> ESURegistrationTableModel::roleNames() const
{
    return( m_roleNames );
}


int ESURegistrationTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return( m_data.isEmpty() ? (0):(m_data.count()) );
}


QVariant ESURegistrationTableModel::data(const QModelIndex &index, int role) const
{
    do {
        int i = index.row();
        if( i < 0 || i > count() || m_data.isEmpty() ) break;

        switch (role)
        {
        case ProfileRole:   return(QVariant( m_data[i].role )); break;
        case NameRole:      return(QVariant( m_data[i].name )); break;
        case AddressRole:   return(QVariant( m_data[i].addressString() )); break;
        case TimeRole:      return(QVariant( m_data[i].formattedTimeString() )); break;
        case IndexRole:     return(QVariant( i )); break;
        case CheckedRole:   return(QVariant( m_data[i].ui.checked )); break;
        default: break;
        }
    }
    while(0);

    return(QVariant());
}


void ESURegistrationTableModel::updateData()
{
    if( m_data.isEmpty() ) return;

    Q_EMIT dataChanged(index(0), index(m_data.count()));
    Q_EMIT countChanged();
}


void ESURegistrationTableModel::clearData()
{
    if( !m_data.isEmpty() ) {

        beginResetModel();
        m_data.clear();
        endResetModel();

        Q_EMIT countChanged();
    }
}

// [ PROPERTIES ]: {{{

int ESURegistrationTableModel::count() const
{
    return( m_data.isEmpty() ? (0):(m_data.count()) );
}


bool ESURegistrationTableModel::checkable() const
{
    return( m_checkable );
}


void ESURegistrationTableModel::setCheckable(bool checkable)
{
    if( m_checkable != checkable ) {
        m_checkable = checkable;

        Q_EMIT checkableChanged();
        Q_EMIT dataChanged(index(0), index(m_data.count()));
    }
}

// }}} [ PROPERTIES ]
// [ Q_SLOTS ]: {{{

void ESURegistrationTableModel::onTableUpdated()
{
    if( !m_dataSource ) return;

    RegistrationTableData& src = m_dataSource->contentData();
    if( src.isEmpty() ) {
        clearData();
        return;
    }

    beginResetModel();
    m_data = src;
    endResetModel();
}


// }}} [ Q_SLOTS ]
