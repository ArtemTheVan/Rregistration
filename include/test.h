#ifndef REGSTART_H
#define REGSTART_H


//todo this macros for qt4 and etu
//#ifdef QT4_ETU

#include <QObject>


class RegStart: public QObject
{
    Q_OBJECT

public:
    RegStart(): QObject(0)
    {}

    ~RegStart() {}

public slots:
    void run(int id, int val);
    void showCurrentMessage();

    void registrationStateChanged();
};

//#endif

#endif
