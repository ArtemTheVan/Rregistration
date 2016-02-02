#include "esu_registration_staff_model.h"
#include "esu_registration_staff_model_p.h"



// [ INIT/DESTROY ]: {{{

ESURegistrationStaffModel::ESURegistrationStaffModel(QObject *parent):
    QAbstractListModel(parent)
  , d_ptr(new ESURegistrationStaffModelPrivate(this))
{
    Q_D(ESURegistrationStaffModel);

    d->roleNames[RoleNameFullRole]  = "roleNameFullRole";
    d->roleNames[DescrFirstRole]    = "descrFirstRole";
    d->roleNames[DescrSecondRole]   = "descrSecondRole";
    d->roleNames[RoleBaseRole]      = "roleNameBaseRole";
    d->roleNames[NameRole]          = "nameRole";
    d->roleNames[TypeRole]          = "typeRole";
    d->roleNames[TypeString]        = "typeString";
    d->roleNames[ImageRole]         = "imageRole";
    d->roleNames[AddressRole]       = "addressRole";
    d->roleNames[RegisteredRole]    = "registeredRole";
    d->roleNames[IndexRole]         = "indexRole";
}


ESURegistrationStaffModel::~ESURegistrationStaffModel()
{

}


void ESURegistrationStaffModel::setDataSource(ESURegistrationTableManager *source)
{
    Q_CHECK_PTR(source);
    Q_D(ESURegistrationStaffModel);

    d->dataSource = source;
    connect( source, &ESURegistrationTableManager::contentUpdated,
             this, &ESURegistrationStaffModel::onContentUpdated, Qt::QueuedConnection );
}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> ESURegistrationStaffModel::roleNames() const
{
    Q_D(const ESURegistrationStaffModel);

    return( d->roleNames );
}


int ESURegistrationStaffModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ESURegistrationStaffModel);
    Q_UNUSED(parent);

    if( d->data.isEmpty() ) return(0);
    return( d->data.count() );
}


QVariant ESURegistrationStaffModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ESURegistrationStaffModel);

    do {
        if( d->data.isEmpty() ) break;
        if ( index.row() < 0 || (index.row() > rowCount()) ) break;
        //qDebug() << rowCount();

        StaffObject e = d->data.at(index.row());
        if( e.isNull() ) break;

        switch (role)
        {
        case RoleNameFullRole:      return(QVariant(e.nameFull())); break;
        case RoleBaseRole:          return(QVariant(e.name())); break;
        case DescrFirstRole:    return(QVariant(e.nameDecription())); break;
        case DescrSecondRole:
        {
            if( e.isCommander() || e.isSoldier() ) {
                return(QVariant(staff::getRankDescription(e.toBaseUnit().rank())));
            } else if( e.isDivision() )
                return(QVariant(e.decription()));

            return(QVariant(QString()));
        }
        case NameRole:
        {
            if( e.isBaseUnit() )
                return(QVariant(e.toBaseUnit().FIO()));
            else return( QVariant(QString()) );
            break;
        }
        case TypeRole:
        {
            if( e.isBaseUnit() || !e.isDivision() ) return( QVariant((int)ESUStaffSystem::UnitType) );
            else if( e.isDivision() ) return( QVariant((int)ESUStaffSystem::DivisionType) );
            break;
        }
        case TypeString:
        {
            if( e.isBaseUnit() || !e.isDivision() ) return( QVariant("Состав") );
            else if( e.isDivision() ) return( QVariant("Подразделения") );
            break;
        }
        case ImageRole:
        {
            QString source;
            if( e.isSoldier() ) source = "qrc:/ui/staff/staff/soldier_60x60.png";
            else if( e.isCommander() ) source = "qrc:/ui/staff/staff/general_60x60.png";
            else if( e.isDivision() ) source = "qrc:/ui/staff/staff/division_60x60.png";
            return(QVariant(source));
            break;
        }
        case AddressRole:
        {
            if( e.isBaseUnit() )
                return(QVariant(e.toBaseUnit().netAddress().toString()));
            else return( QVariant(QString()) );
            break;
        }
        case RegisteredRole:
        {
            if( e.isBaseUnit() )
                return(QVariant(e.toBaseUnit().isRegistered()));
            else return( false );
            break;
        }
        case IndexRole: return( QVariant(index.row()) );
        default: break;
        }

    } while(0);

    return QVariant();
}


void ESURegistrationStaffModel::clearData()
{
    Q_D(ESURegistrationStaffModel);

    beginResetModel();
    d->data.clear();
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationStaffModel::updateData()
{
    Q_EMIT dataChanged(index(0), index(count()));
}

// }}} [ BASE MODEL METHODS ]
// [ SWITCH MODEL METHODS ]: {{{

bool ESURegistrationStaffModel::toChildDivision(const QString &name)
{
    Q_D(ESURegistrationStaffModel);
    int i;
    StaffDivision e;

    for( i = 0; i < d->data.count(); ++i ) {

        if( d->data[i].name() == name && d->data[i].isDivision() )
        {
            e = d->data[i].toDivision();
            if( !e.hasObjects() ) break;

            beginResetModel();
            d->currentElement = e;
            d->data = d->currentElement.objects();
            d->modelPath.add(name);
            endResetModel();

            Q_EMIT countChanged();
            return(true);
        }
    }

    return(false);
}


bool ESURegistrationStaffModel::setParentDivision()
{
    Q_D(ESURegistrationStaffModel);

    if( !d->currentElement.hasParentDivision() ) return(false);

    beginResetModel();
    d->currentElement = d->currentElement.parentDivision();
    d->data = d->currentElement.objects();
    endResetModel();

    Q_EMIT countChanged();
    return(true);
}


void ESURegistrationStaffModel::toParentDivision(const QString &name)
{
    Q_D(ESURegistrationStaffModel);

    if( d->currentElement.name() == name ||
            d->currentElement.nameFull() == name) return;
    StaffDivision e = d->currentElement;

    while( e.hasParentDivision() && !e.isNull() )
    {
        e = e.parentDivision();
        if( e.isNull() || !e.hasObjects() ) break;
        if( e.name() == name || e.nameFull() == name ) {
            d->currentElement = e;
            beginResetModel();
            d->data = d->currentElement.objects();
            endResetModel();

            Q_EMIT countChanged();
            break;
        }
    }
}

// }}} [ SWITCH MODEL METHODS ]
// [MODEL INFO METHODS]: {{{

QString ESURegistrationStaffModel::rootElementName() const
{
    Q_D(const ESURegistrationStaffModel);
    if( d->rootElement.isNull() )
        return(QString());

    return(d->rootElement.name());
}


QString ESURegistrationStaffModel::rootElementNameFull() const
{
    Q_D(const ESURegistrationStaffModel);
    if( d->rootElement.isNull() )
        return(QString());

    return(d->rootElement.nameFull());
}

// }}} [MODEL INFO METHODS]
// [ PROPERTIES ]:

int ESURegistrationStaffModel::count() const
{
    Q_D(const ESURegistrationStaffModel);

    return( (d->data.isEmpty())?(0):(d->data.count()) );
}


bool ESURegistrationStaffModel::checkable() const
{
    Q_D(const ESURegistrationStaffModel);

    return( d->checkable );
}

void ESURegistrationStaffModel::setCheckable(bool checkable)
{
    Q_D(ESURegistrationStaffModel);

    if( d->checkable != checkable ) {
        d->checkable = checkable;
        Q_EMIT checkableChanged();
        Q_EMIT dataChanged(index(0), index(count()));
    }
}

// }}} [ PROPERTIES ]
// [ MODEL DATA ]: {{{

void ESURegistrationStaffModel::setModelData(const StaffDivision &data)
{
    Q_D(ESURegistrationStaffModel);

    if( data.isNull() ) return;

    beginResetModel();
    d->rootElement = data;
    d->currentElement = d->rootElement;
    d->data = d->currentElement.objects();
    endResetModel();

    d->modelPath.clearModel();
    d->modelPath.add(d->rootElement.name());

    Q_EMIT countChanged();
}


ESURegistrationPathModel *ESURegistrationStaffModel::pathModel()
{
    Q_D(ESURegistrationStaffModel);

    return( &d->modelPath );
}

// }}} [ MODEL DATA ]
// [ Q_SLOTS ]: {{{

void ESURegistrationStaffModel::onContentUpdated()
{
    Q_D(ESURegistrationStaffModel);
    if( !d->dataSource ) return;

    static ESUStaffStructure &staff = esuStaff->data();
    bool changed = false;

    RegistrationTableData& data = d->dataSource->contentData();
    if( data.isEmpty() && staff.isEmpty() ) return;

    for( const RegistrationUserInfo& user: data ) {
        StaffBaseUnit unitStaff = staff.findUnitByName( user.role );
        if( unitStaff.isNull() ) continue;
        unitStaff.setRegistered(true);
        unitStaff.setRegistrationTime( user.time );
        unitStaff.setUnitFIO( user.name );
        unitStaff.setNetAddress( user.address );
        if( !changed ) changed = true;
    }

    if( changed )
        Q_EMIT dataChanged(index(0), index(count()));
}

// }}}
