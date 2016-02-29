#ifndef TEST_H
#define TEST_H


//todo this macros for qt4 and etu
//#ifdef QT4_ETU

#include <QObject>


class Test: public QObject
{
    Q_OBJECT

public:
    Test(): QObject(0)
    {}

    ~Test() {}

public slots:
    void run(int id, int val);
    void showCurrentMessage();

};

//#endif

#endif
