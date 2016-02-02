#ifndef UI_REGISTRATION_APPLICATION_H
#define UI_REGISTRATION_APPLICATION_H

#include <QtCore>
#include <QDeclarativeContext>
#include <QDeclarativeItem>

#include "esu_registration.h"
#include "i_listener_esu.h"


class ESURegistration;
class UIRegistrationApplicationPrivate;

class UIRegistrationApplication : public QObject,
        public IListenerESU
{
    Q_OBJECT

    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    Q_PROPERTY(bool progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QDeclarativeItem* uiForm READ uiForm WRITE setUiForm NOTIFY uiFormChanged)

public:
    explicit UIRegistrationApplication(QObject *parent = nullptr);
    ~UIRegistrationApplication();

public:
    // [ BASE METHODS ]:
    void registerQmlProperties(QDeclarativeContext* context);
    static void declareQmlTypes();

    // [ PROPERTIES ]:
    QString currentMessage() const;
    void setCurrentMessage(const QString& message);

    bool progress() const;
    void setProgress(bool progress);

    int mode() const;
    void setMode(int mode);

    QDeclarativeItem* uiForm() const;
    void setUiForm(QDeclarativeItem* uiForm);

    void setRegistration(ESURegistration* regPtr);    

    // [ IListenerESU ]: {{
public:
    bool isObject() const { return(true); }
    QObject* toObject() { return(this); }
    quint32 ID() const;

public Q_SLOTS:
    void onProcessAppMessage(quint32 senderID, int msg);

    // }} [ IListenerESU ]

public:
    // [ UI METHODS/SLOTS ]:
    Q_INVOKABLE void requestRegistration();

protected:
    void customEvent(QEvent *event);

Q_SIGNALS:
    void currentMessageChanged();
    void progressChanged();
    void modeChanged();
    void uiFormChanged();
    void errorChanged();

private Q_SLOTS:
    void onStateChanged(int state);
    void onNotifySend(QString msg);
    void onNotifyAccepted();

protected:
    UIRegistrationApplicationPrivate* d;

private:
    Q_DECLARE_PRIVATE_D(UIRegistrationApplication::d, UIRegistrationApplication)
    Q_DISABLE_COPY(UIRegistrationApplication)
};

#endif // UI_REGISTRATION_APPLICATION_H
