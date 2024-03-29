#ifndef SQAPPLICATION_H
#define SQAPPLICATION_H

#include <QStandardPaths>
#include <QSettings>
#include <QObject>
#include <QTranslator>
/*/
 * Default mode is standalone, it's not really great when people
 * just want to build the app, but it ensure to keep it standalone
 * when developping
 */
#if !defined(SQPROJECT_WIN32_INSTALL) || !defined(SQPROJECT_UNIX_INSTALL)
#define SQPROJECT_STANDALONE
#endif
#ifdef SQPROJECT_WIN32_STANDALONE
#define SQPROJECT_STANDALONE
#endif

#ifdef QT_WIDGETS_LIB
#include <QApplication>
#define _SQAPPLICATION_BASE_QT_TYPE QApplication
#elif defined(QT_GUI_LIB)
#include <QGuiApplication>
#define _SQAPPLICATION_BASE_QT_TYPE QGuiApplication
#else
#include <QCoreApplication>
#define _SQAPPLICATION_BASE_QT_TYPE QCoreApplication
#endif

#include "sqpath.h"
#define sqApp   SQApplication::instance()

class SQApplication : public _SQAPPLICATION_BASE_QT_TYPE
{
public:
    SQApplication(int &argc, char** argv);

    bool        hasGitInformation() const;
    QString     gitCommitNumber() const;
    QString     gitTag() const;
    bool        isStandalone() const;
    bool        createSettings();
    QSettings*  settings();
    bool        setTranslation(const QString& fileSuffix);

// sqlogging pri file set this define
#ifdef SQ_PROJECT_HAS_LOGGING
    void        setLogFilename(const QString& logName)
    {
        m_logFilename = logName;
    }
    void        setDebugLogFilename(const QString& debugName)
    {
        m_debugLogFilename = debugName;
    }
    QString     logDirectoryPath() const
    {
        return SQPath::logDirectoryPath();
    }
    QString     logFileName() const {return m_logFilename;}
    QString     debugLogFileName() const {return m_debugLogFilename;}
    QString     logFilePath() const {return logDirectoryPath() + "/" + m_logFilename;}
    QString     debugLogFilePath() const {return logDirectoryPath() + "/" + m_debugLogFilename;}
#endif

    static SQApplication*   instance();
private:
    static SQApplication*   self;
    QTranslator*            m_translator;
    QString                 m_logFilename;
    QSettings*              m_settings;
    QString                 m_debugLogFilename;
};

#endif // SQAPPLICATION_H
