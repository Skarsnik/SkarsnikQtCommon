#pragma once

#include <QtGlobal>
#include <QLoggingCategory>

/*#define SQLOGGING_ADD_CATEGORY(categoryName, ...) \
    const QLoggingCategory& log_##categoryName() \
        { \
            static const QLoggingCategory category(categoryName, __VA_ARGS__); \
            return category; \
        } \
    static QDebug& sqDebug() \
    { \
        return  QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, log_##categoryName().categoryName()).debug(__VA_ARGS__); \
    } \
    static QDebug& sqInfo() \
    { \
        return  QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, log_##categoryName().categoryName()).info(__VA_ARGS__); \
    } */

/**
 * @brief Install a qtmessagehandler to store call to qDebug/qInfo to file
 * It handles path to store the file(s) in the appropriate location according to the build mode
 * and platform
 * @param fileBaseName The base name for the normal log file, catch message passed as info level by default
 * @param debugFileBaseName The base name for debug log, catch everything
 * @param debugEnabled Enable or not the debug log, default is false
 * @param crashFile setting this activate a crash log, all log entries are stored in a ringged list that get dumpped to a file
 *        when receiving a sigseg
 */


void    installSQLogging(const QString& fileBaseName, const QString &debugFileBaseName = "debug-log.txt", bool debugEnabled = false , const QString &crashFile = "");
