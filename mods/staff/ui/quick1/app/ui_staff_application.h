#ifndef UI_STAFF_APPLICATION_H
#define UI_STAFF_APPLICATION_H

#include <QtCore>
#include <QDeclarativeContext>
#include <QDeclarativeItem>

#include "esu_military_staff.h"
#include "i_listener_esu.h"


class UIStaffApplicationPrivate;

class UIStaffApplication : public QObject,
        public IListenerESU
{
    Q_OBJECT

    Q_PROPERTY(QDeclarativeItem* uiStaffForm READ uiStaffForm WRITE setUiStaffForm NOTIFY uiStaffFormChanged)
    Q_PROPERTY(QDeclarativeItem* uiStructureForm READ uiStructureForm WRITE setUiStructureForm NOTIFY uiStructureFormChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString caption READ caption WRITE setCaption NOTIFY captionChanged)

public:
    enum Mode { ViewMode, ChooseProfileMode };
    enum ObjectType { UnknownObject = 0, CommanderObject = 1, SoldierObject = 2, DivisionObject = 3 };

    Q_ENUMS(Mode)
    Q_ENUMS(ObjectType)

public:
    explicit UIStaffApplication(QObject *parent = 0);
    ~UIStaffApplication();

public:
    // [ BASE METHODS ]:
    void registerQmlProperties(QDeclarativeContext* context);
    static void declareQmlTypes();

    void loadQmlModels();

    // [ PROPERTIES ]
    QDeclarativeItem* uiStaffForm() const;
    void setUiStaffForm(QDeclarativeItem* uiStaffForm);

    QDeclarativeItem* uiStructureForm() const;
    void setUiStructureForm(QDeclarativeItem* uiStructureForm);

    int mode() const;
    void setMode(int mode);

    QString caption() const;
    void setCaption(const QString& caption);

    // [ IListenerESU ]: {{
    quint32 ID() const;
    bool isObject() const;
    QObject* toObject() { return(this); }
    // }}

public:
    // [ UI METHODS ]
    Q_INVOKABLE void registerProfile(const QString& name);

protected:
    // [ Q_EVENTS ]
    void customEvent(QEvent*);

Q_SIGNALS:
    void uiStaffFormChanged();
    void uiStructureFormChanged();
    void modeChanged();
    void captionChanged();

protected:
    UIStaffApplicationPrivate* d;

private:
    Q_DECLARE_PRIVATE_D(UIStaffApplication::d, UIStaffApplication)
    Q_DISABLE_COPY(UIStaffApplication)
};

#endif // UI_STAFF_APPLICATION_H
