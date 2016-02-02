#include "esu_registration_ranks_model.h"


// [ INIT/DESTROY ]: {{{

ESURegistrationRanksModel::ESURegistrationRanksModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roleNames[textValueRole]   = "textValue";
    m_roleNames[itemValueRole]   = "itemValue";

    m_roleNames[textRole]        = "textRole";
    m_roleNames[valueRole]       = "valueRole";

    setData();
}


ESURegistrationRanksModel::~ESURegistrationRanksModel()
{

}

// }}} [ INIT/DESTROY ]
// [ BASE MODEL METHODS ]: {{{

QHash<int, QByteArray> ESURegistrationRanksModel::roleNames() const
{
    return(m_roleNames);
}


int ESURegistrationRanksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return( (m_data.isEmpty())?(0):(m_data.count()) );
}


QVariant ESURegistrationRanksModel::data(const QModelIndex &index, int role) const
{
    do {
        int i = index.row();
        if( m_data.isEmpty() || i < 0 || i > m_data.count() ) break;

        switch (role)
        {
        case textRole:
        case textValueRole: return(QVariant( m_data[i] )); break;
        case valueRole:
        case itemValueRole:
        {
            if( !useCommandersRanks ) return(QVariant(i));
            else return(QVariant( (i == 0?(0):(i+5)) ));
            break;
        }
        default: break;
        }
    }
    while(0);

    return(QVariant());
}


void ESURegistrationRanksModel::clearModel()
{
    if( m_data.isEmpty() ) return;

    beginResetModel();
    useCommandersRanks = false;
    m_data.clear();
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationRanksModel::setData()
{
    if( !m_data.isEmpty() ) m_data.clear();
    useCommandersRanks = false;

    beginResetModel();
    m_data << "Не известно";
    m_data << "Рядовой (курсант)";
    m_data << "Ефрейтор";
    m_data << "Матрос (курсант)";
    m_data << "Старший матрос";
    m_data << "Младший сержант";
    m_data << "Сержант";
    m_data << "Старший сержант";
    m_data << "Старшина";
    m_data << "Старшина 1-ой статьи";
    m_data << "Старшина 2-ой статьи";
    m_data << "Главный старшина";
    m_data << "Главный корабельный старшина";
    m_data << "Прапорщик";
    m_data << "Старший прапорщик";
    m_data << "Мичман";
    m_data << "Старший мичман";
    m_data << "Младший лейтенант";
    m_data << "Лейтенант";
    m_data << "Старший лейтенант";
    m_data << "Капитан";
    m_data << "Младший лейтенант";
    m_data << "Лейтенант";
    m_data << "Старший лейтенант";
    m_data << "Капитан-лейтенант";
    m_data << "Офицер";
    m_data << "Майор";
    m_data << "Подполковник";
    m_data << "Полковник";
    m_data << "Капитан первого ранга";
    m_data << "Капитан второго ранга";
    m_data << "Капитан третьего ранга";
    m_data << "Генерал-майор";
    m_data << "Генерал-лейтенант";
    m_data << "Генерал-полковник";
    m_data << "Генерал армии";
    m_data << "Маршал";
    m_data << "Контр-адмирал";
    m_data << "Вице-адмирал";
    m_data << "Адмирал флота";
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationRanksModel::setSoldiersRanks()
{
    if( !m_data.isEmpty() ) m_data.clear();
    useCommandersRanks = false;

    beginResetModel();
    m_data << "Не известно";
    m_data << "Рядовой (курсант)";
    m_data << "Ефрейтор";
    m_data << "Матрос (курсант)";
    m_data << "Старший матрос";
    endResetModel();

    Q_EMIT countChanged();
}


void ESURegistrationRanksModel::setCommandersRanks()
{
    if( !m_data.isEmpty() ) m_data.clear();
    useCommandersRanks = true;

    beginResetModel();
    m_data << "Не известно";        // (0)
    m_data << "Младший сержант";    // (5)
    m_data << "Сержант";
    m_data << "Старший сержант";
    m_data << "Старшина";
    m_data << "Старшина 1-ой статьи";
    m_data << "Старшина 2-ой статьи";
    m_data << "Главный старшина";
    m_data << "Главный корабельный старшина";
    m_data << "Прапорщик";
    m_data << "Старший прапорщик";
    m_data << "Мичман";
    m_data << "Старший мичман";
    m_data << "Младший лейтенант";
    m_data << "Лейтенант";
    m_data << "Старший лейтенант";
    m_data << "Капитан";
    m_data << "Младший лейтенант";
    m_data << "Лейтенант";
    m_data << "Старший лейтенант";
    m_data << "Капитан-лейтенант";
    m_data << "Офицер";
    m_data << "Майор";
    m_data << "Подполковник";
    m_data << "Полковник";
    m_data << "Капитан первого ранга";
    m_data << "Капитан второго ранга";
    m_data << "Капитан третьего ранга";
    m_data << "Генерал-майор";
    m_data << "Генерал-лейтенант";
    m_data << "Генерал-полковник";
    m_data << "Генерал армии";
    m_data << "Маршал";
    m_data << "Контр-адмирал";
    m_data << "Вице-адмирал";
    m_data << "Адмирал флота";
    endResetModel();

    Q_EMIT countChanged();
}

// }}} [ BASE MODEL METHODS ]
// [ PROPERTIES ]:

int ESURegistrationRanksModel::count() const
{
    return( (m_data.isEmpty())?(0):(m_data.count()) );
}

// }}} [ PROPERTIES ]
// [ MODEL METHODS ]: {{{

void ESURegistrationRanksModel::add(const QString &name)
{
    beginResetModel();
    m_data.append( name );
    endResetModel();

    Q_EMIT countChanged();    
}


void ESURegistrationRanksModel::remove(int index, int cnt)
{
    if( index < 0 || (index + cnt - 1) >= count() || m_data.isEmpty() )
        return;

    beginResetModel();
    for(int i = 0; i < cnt; ++i)
        m_data.removeAt(i + index);
    endResetModel();    

    Q_EMIT countChanged();
}


void ESURegistrationRanksModel::removeFrom(int index)
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


void ESURegistrationRanksModel::removeByName(const QString &name)
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


void ESURegistrationRanksModel::removeLast()
{
    remove(count() - 1);
}


void ESURegistrationRanksModel::removeFirst()
{
    remove();
}


int ESURegistrationRanksModel::itemIndex(const QString &name)
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
