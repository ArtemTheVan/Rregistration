#ifndef ESULOGGER_H
#define ESULOGGER_H

#include <QtCore/qglobal.h>
#include <QtCore/QString>
#include <QtCore/qdebug.h>

#include <stdio.h>
#include <stdlib.h>


#define ESU_MESSAGE_LOG(msg, mod)    ESULogger::message(msg, mod)
#define ESU_WARNING_LOG(msg, mod)    ESULogger::warning(msg, mod)
#define ESU_STATUS_LOG(msg, mod)     ESULogger::status(msg, mod)
#define ESU_ERROR_LOG(msg, mod)      ESULogger::error(msg, mod)

#define ESU_MESSAGE_FUNC_LOG(msg, mod, func) ESULogger::message(msg, mod, ESULogger::DefaultLevel, func)
#define ESU_WARNING_FUNC_LOG(msg, mod, func) ESULogger::warning(msg, mod, func)
#define ESU_STATUS_FUNC_LOG(msg, mod, func)  ESULogger::warning(msg, mod, func)
#define ESU_ERROR_FUNC_LOG(msg, mod, func)   ESULogger::error(msg, mod, func)


class ESULogger
{
public:
    enum LogLevel {
        DefaultLevel, MessageLevel, StatusLevel, ErrorLevel, WarningLevel
    };

public:
    //! Base message method
    static void message(const QString& msg, const QString& module = QString(), LogLevel level = DefaultLevel, const QString& func = QString())
    {
#ifdef ESU_LOG_ENABLE
        QString out;

        if( module.isEmpty() ) out = "\n[ESU]: ";
        else out = QString("\n[ESU]:[%1] ").arg(module);

        switch (level)
        {
        case ErrorLevel:
            out += "<Error> ";
            break;
        case WarningLevel:
            out += "<Warning> ";
            break;
        case StatusLevel:
            out += "<Status> ";
            break;
        case MessageLevel:
        case DefaultLevel:
        default:
            break;
        }

        if( func.isEmpty() )
            out += QString("  %1").arg(msg);
        else
            out += QString("<%1>  %2").arg(func).arg(msg);

        //const char* out_msg = qPrintable(out);
        fprintf(stderr, "%s", qPrintable(out));

        if( level == ErrorLevel )
            exit(EXIT_FAILURE);
#endif
    }

    //! Warning level message
    static inline void warning(const QString& msg, const QString& module = QString(), const QString& func = QString())
    {
#ifdef ESU_LOG_ENABLE
        message(msg, module, WarningLevel, func);
#endif
    }

    //! Error level message
    static inline void error(const QString& msg, const QString& module = QString(), const QString& func = QString())
    {
#ifdef ESU_LOG_ENABLE
        message(msg, module, ErrorLevel, func);
#endif
    }

    //! Status level message
    static inline void status(const QString& msg, const QString& module = QString(), const QString& func = QString())
    {
#ifdef ESU_LOG_ENABLE
        message(msg, module, StatusLevel, func);
#endif
    }
};

#endif // ESULOGGER_H
