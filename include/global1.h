#ifndef GLOBAL1_H
#define GLOBAL1_H


//todo this macros for qt4 and etu
#ifdef QT4_ETU
#define nullptr NULL
#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()

//#define NEW_PROTOBUF
#endif

#endif
