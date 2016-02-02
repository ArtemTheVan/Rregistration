#ifndef UI_REGISTRATION_SOLDIERS_MODEL_H
#define UI_REGISTRATION_SOLDIERS_MODEL_H

#include <QObject>


class UIRegistrationSoldiersModel : public QObject
{
    Q_OBJECT
public:
    explicit UIRegistrationSoldiersModel(QObject *parent = 0);
    ~UIRegistrationSoldiersModel();

signals:

public slots:
};

#endif // UI_REGISTRATION_SOLDIERS_MODEL_H
