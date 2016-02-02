#include "staff_ui_model.h"
#include "staff_ui_model_p.h"

#include "private/military_staff_globals_p.h"


// [ INIT/DESTROY ]: {{{

StaffUIModel::StaffUIModel(QObject *parent):
    QAbstractListModel(parent)
  , d_ptr(new StaffUIModelPrivate(this))
{
    Q_D(StaffUIModel);

    d->roleNames[NameFullRole]      = "nameFullRole";
    d->roleNames[DescrFirstRole]    = "descrFirstRole";
    d->roleNames[DescrSecondRole]   = "descrSecondRole";
    d->roleNames[NameRole]          = "nameRole";
    d->roleNames[TypeRole]          = "typeRole";
    d->roleNames[ImageRole]         = "imageRole";
    d->roleNames[AddressRole]       = "addressRole";
}


StaffUIModel::~StaffUIModel()
{

}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> StaffUIModel::roleNames() const
{
    Q_D(const StaffUIModel);

    return( d->roleNames );
}


int StaffUIModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const StaffUIModel);
    Q_UNUSED(parent);

    if( d->data.isEmpty() ) return(0);
    return( d->data.count() );
}


QVariant StaffUIModel::data(const QModelIndex &index, int role) const
{
    Q_D(const StaffUIModel);

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
            MilitaryStaffGlobals::StaffObjectType type = MilitaryStaffGlobals::UnknownType;

            if( e.isSoldier() ) type = MilitaryStaffGlobals::SoldierType;
            else if( e.isCommander() ) type = MilitaryStaffGlobals::CommanderType;
            else if( e.isDivision() ) type = MilitaryStaffGlobals::DivisionType;
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
        case AddressRole:
        {
            if( e.isBaseUnit() )
                return(QVariant(e.toBaseUnit().netAddress().toString()));

            break;
        }
        default: break;
        }

    } while(0);

    return QVariant();
}


void StaffUIModel::clearData()
{
    Q_D(StaffUIModel);

    beginResetModel();
    d->data.clear();
    endResetModel();

    Q_EMIT countChanged();
}


void StaffUIModel::updateData()
{
    Q_EMIT dataChanged(index(0), index(count()));
}

// }}} [ BASE MODEL METHODS ]
// [ SWITCH MODEL METHODS ]: {{{

bool StaffUIModel::toChildDivision(const QString &name)
{
    Q_D(StaffUIModel);
    int i;
    StaffDivision e;

    for( i = 0; i < d->data.count(); ++i ) {
        qDebug() << "Division check: " << d->data[i].isDivision();
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


bool StaffUIModel::parentDivision()
{
    Q_D(StaffUIModel);

    if( !d->currentElement.hasParentDivision() ) return(false);

    beginResetModel();
    d->currentElement = d->currentElement.parentDivision();
    d->data = d->currentElement.objects();
    endResetModel();

    Q_EMIT countChanged();
    return(true);
}


void StaffUIModel::toParentDivision(const QString &name)
{
    Q_D(StaffUIModel);

    if( d->currentElement.name() == name ||
            d->currentElement.nameFull() == name) return;
    StaffDivision e = d->currentElement;

    while( e.hasParentDivision() && !e.isNull() ) {
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

QString StaffUIModel::rootElementName() const
{
    Q_D(const StaffUIModel);
    if( d->rootElement.isNull() )
        return(QString());

    return(d->rootElement.name());
}


QString StaffUIModel::rootElementNameFull() const
{
    Q_D(const StaffUIModel);
    if( d->rootElement.isNull() )
        return(QString());

    return(d->rootElement.nameFull());
}

// }}} [MODEL INFO METHODS]
// [ PROPERTIES ]:

int StaffUIModel::count() const
{
    Q_D(const StaffUIModel);

    if( !d->data.isEmpty() ) return(d->data.count());
    return(0);
}

// }}} [ PROPERTIES ]
// [ MODEL DATA ]: {{{

void StaffUIModel::setModelData(const StaffDivision &data)
{
    Q_D(StaffUIModel);

    if( data.isNull() ) return;

    beginResetModel();
    d->rootElement = data;
    d->currentElement = d->rootElement;
    d->data = d->currentElement.objects();
    endResetModel();

    Q_EMIT countChanged();
}


void StaffUIModel::updateModel(const StaffDivision &data)
{
    Q_D(StaffUIModel);

    if( data.isNull() ) return;

    beginResetModel();
    d->rootElement = data;
    d->currentElement = d->rootElement;
    d->data = d->currentElement.objects();
    endResetModel();

    Q_EMIT countChanged();
}

// }}} [ MODEL DATA ]
