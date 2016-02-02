#include "staff_structure_ui_model.h"
#include "staff_structure_ui_model_p.h"


// [ INIT/DESTROY ]: {{{

StaffStructureUIModel::StaffStructureUIModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new StaffStructureUIModelPrivate(this))
{
    Q_D(StaffStructureUIModel);

    d->roleNames[NameFullRole]      = "nameFullRole";
    d->roleNames[DescrFirstRole]    = "descrFirstRole";
    d->roleNames[DescrSecondRole]   = "descrSecondRole";
    d->roleNames[NameRole]          = "nameRole";
    d->roleNames[TypeRole]          = "typeRole";
    d->roleNames[ImageRole]         = "imageRole";

    d->init();
}


StaffStructureUIModel::~StaffStructureUIModel()
{

}


void StaffStructureUIModel::initData()
{
    Q_D(StaffStructureUIModel);

    d->init();
}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> StaffStructureUIModel::roleNames() const
{
    Q_D(const StaffStructureUIModel);

    return( d->roleNames );
}


int StaffStructureUIModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const StaffStructureUIModel);
    Q_UNUSED(parent);

    if( d->data.isEmpty() ) return(0);
    return( d->data.count() );
}


QVariant StaffStructureUIModel::data(const QModelIndex &index, int role) const
{
    Q_D(const StaffStructureUIModel);

    do {
        if( d->data.isEmpty() ) break;
        if ( index.row() < 0 || (index.row() > rowCount()) ) break;
        //qDebug() << rowCount();

        StaffObject e = d->data.at(index.row());
        if( e.isNull() ) break;

        switch (role)
        {
        case NameFullRole:      return(QVariant(e.nameFull())); break;
        case NameRole:          return(QVariant(e.name())); break;
        case DescrFirstRole:    return(QVariant(e.nameDecription())); break;
        case DescrSecondRole:
        {
            if( e.isCommander() || e.isSoldier() ) {
                return(QVariant(staff::getRankDescription(e.toBaseUnit().rank())));
            } else if( e.isDivision() )
                return(QVariant(e.decription()));

            return(QVariant(QString()));
        }    
        case TypeRole:
        {
            UIStaffApplication::ObjectType type = UIStaffApplication::UnknownObject;
            if( e.isSoldier() ) type = UIStaffApplication::SoldierObject;
            else if( e.isCommander() ) type = UIStaffApplication::CommanderObject;
            else if( e.isDivision() ) type = UIStaffApplication::DivisionObject;
            return(QVariant((int)type));
            break;
        }
        case ImageRole:
        {
            QString source;
            if( e.isSoldier() ) source = "/mods/military_staff/ui/images/soldier_60x60.png";
            else if( e.isCommander() ) source = "/mods/military_staff/ui/images/general_60x60.png";
            else if( e.isDivision() ) source = "/mods/military_staff/ui/images/division_60x60.png";
            return(QVariant(source));
            break;
        }
        default: break;
        }

    } while(0);

    return QVariant(QString());
}


void StaffStructureUIModel::clearData()
{
    Q_D(StaffStructureUIModel);

    beginResetModel();
    d->data.clear();
    endResetModel();
}


void StaffStructureUIModel::updateData()
{
    Q_EMIT dataChanged(index(0), index(count()));
}

// }}} [ BASE MODEL METHODS ]
// [ SWITCH MODEL METHODS ]: {{{

bool StaffStructureUIModel::toChildDivision(const QString &name)
{
    Q_D(StaffStructureUIModel);
    StaffDivision e;

    for( int i = 0; i < d->data.count(); ++i ) {
        if( d->data[i].name() == name && d->data[i].isDivision() )
        {
            e = d->data[i].toDivision();
            if( !e.hasObjects() ) break;

            beginResetModel();
            d->currentElement = e;
            d->data = d->currentElement.objects();            
            endResetModel();

            Q_EMIT countChanged();
            return(true);
        }
    }

    return(false);
}


void StaffStructureUIModel::parentDivision()
{
    Q_D(StaffStructureUIModel);

    if( !d->currentElement.hasParentDivision() ) return;

    beginResetModel();
    d->currentElement = d->currentElement.parentDivision();
    d->data = d->currentElement.objects();
    endResetModel();

    Q_EMIT countChanged();
}


bool StaffStructureUIModel::toParentDivision(const QString &name)
{
    Q_D(StaffStructureUIModel);
    if( d->currentElement.name() == name ) return(false);

    StaffDivision e = d->currentElement;
    while( e.hasParentDivision() && !e.isNull() ) {
        e = e.parentDivision();
        if( e.isNull() ) break;
        if( e.name() == name || e.nameFull() == name ) {
            if( !e.hasObjects() ) break;
            d->currentElement = e;
            beginResetModel();
            d->data = d->currentElement.objects();
            endResetModel();
            return(true);
        }
    }

    return(false);
}

// }}} [ SWITCH MODEL METHODS ]
// [OTHER METHODS]: {{{

QString StaffStructureUIModel::rootElementName() const
{
    Q_D(const StaffStructureUIModel);

    if( d->currentElement.isNull() )
        return(QString());

    return( d->currentElement.name() );
}


QString StaffStructureUIModel::rootElementNameFull() const
{
    Q_D(const StaffStructureUIModel);

    if( d->currentElement.isNull() )
        return(QString());

    return( d->currentElement.nameFull() );
}

// }}} [OTHER METHODS]
// [ PROPERTIES ]: {{{

int StaffStructureUIModel::count() const
{
    Q_D(const StaffStructureUIModel);

    if( !d->data.isEmpty() ) return(d->data.count());
    return(0);
}

// }}} [ PROPERTIES ]
// [ MODEL DATA ]: {{{

StaffDivision StaffStructureUIModel::modelRootElement() const
{
    Q_D(const StaffStructureUIModel);

    return(d->rootElement);
}


StaffObject StaffStructureUIModel::modelDataFromIndex(int index) const
{
    Q_D(const StaffStructureUIModel);

    if( d->data.isEmpty() || count() < (index+1) || (index+1) > count() )
        return(StaffObject());

    return( d->data[index] );
}

// }}} [ MODEL DATA ]
