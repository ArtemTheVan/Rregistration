#ifndef GLOBAL1_H
#define GLOBAL1_H


//todo this macros for qt4 and etu
#define QT4_ETU
#ifdef QT4_ETU
#define nullptr NULL
#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()
#endif


#endif
