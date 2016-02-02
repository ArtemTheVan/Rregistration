#ifndef ESU_REGISTRATION_UI_H
#define ESU_REGISTRATION_UI_H

#ifdef ESU_UI_QUICK
#include <QQmlContext>
#include <QQuickItem>
#else
#include <QObject>
#endif

#include "esu_registration_models.h"


class ESURegistration;

class ESURegistrationUI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal fieldHeight READ fieldHeight WRITE setFieldHeight NOTIFY fieldHeightChanged)

public:
    ESURegistrationUI(ESURegistration* _base = nullptr, QObject *parent = nullptr);
    ~ESURegistrationUI();

public:
    // [ QML Methods ]:
#ifdef ESU_UI_QUICK
    void registerQmlProperties(QQmlContext* context = nullptr);
    static void declareQmlTypes();
    void createQmlModels(QQmlContext* context = nullptr);
#else
    void createModels();
#endif

public:
    // [ PROPERTIES ]:
    qreal fieldHeight() const;
    void setFieldHeight(qreal height);

public:
    void startTaskTimer(int msecs);
    void stopTaskTimer();

public Q_SLOTS:
    // [ Events ]:
    void showFormEvent();

Q_SIGNALS:   
    void fieldHeightChanged();

private:
    ESURegistration *base;
#ifdef ESU_UI_QUICK
    QQmlContext     *m_context;
#endif
    qreal            m_fieldHeight;

    bool             m_taskTimerRunning;
    int              m_taskTimerInterval;

public:
    ESURegistrationRanksModel   modelRanks;
    ESURegistrationStaffModel   modelStaff;
    ESURegistrationQueriesModel modelQueries;
    ESURegistrationQueriesModel modelUpdateQueries;
    ESURegistrationTableModel   modelRegistrationTable;
};

#endif // ESU_REGISTRATION_UI_H
