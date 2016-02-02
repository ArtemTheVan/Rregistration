#ifndef UI_REGISTRATION_BASE_MODEL_H
#define UI_REGISTRATION_BASE_MODEL_H

#include <QObject>



class UIRegistrationBaseModel : public QObject
{
    Q_OBJECT
public:
    explicit UIRegistrationBaseModel(QObject *parent = 0);
    ~UIRegistrationBaseModel();

signals:

public slots:
};

#endif // UI_REGISTRATION_BASE_MODEL_H
