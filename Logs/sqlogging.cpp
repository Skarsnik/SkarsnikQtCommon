#include <QTextStream>
#include <QDateTime>
#include <QFile>

#ifndef SQPROJECT_HAS_NOUI
    #include <QMessageBox>
#endif

#include <signal.h>
#include <sqapplication.h>
#include <sqlogging.h>
#include <sqringlist.hpp>

static QTextStream  logfile;
static QTextStream  debugLogFile;
static QString      crashLogPath;

#ifdef SQPROJECT_DEVEL
static QTextStream cout(stdout);
#else

#endif

static SQRingList<QString> logDebugCrash(500);

bool    sqLoggingDontLogNext = false;

static void    sqLoggingDefaultMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QTextStream*    log = &logfile;
        //cout << msg;
    if (sqLoggingDontLogNext)
        return ;
    #ifdef QT_NO_DEBUG
        QString logString = QString("%6 %5 - %7: %1").arg(localMsg.constData()).arg(context.category, 20).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    #else
        QString logString = QString("%6 %5 - %7: %1 \t(%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(context.category, 20).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    #endif
        switch (type)
        {
            case QtDebugMsg:
                logDebugCrash.append(logString.arg("Debug"));
                break;
            case QtCriticalMsg:
                logDebugCrash.append(logString.arg("Critical"));
                *log << logString.arg("Critical");
                break;
            case QtWarningMsg:
                logDebugCrash.append(logString.arg("Warning"));
                *log << logString.arg("Warning");
                break;
            case QtFatalMsg:
                logDebugCrash.append(logString.arg("Fatal"));
                *log << logString.arg("Fatal");
                *log<< "\n"; log->flush();
                #ifndef SQPROJECT_HAS_NOUI
                    QMessageBox::critical(nullptr, QObject::tr("Critical error"), msg);
                #else
                fprintf(stderr, "Critical error :%s\n", msg.toLatin1().constData());
                #endif
                sqApp->exit(1);
                break;
            case QtInfoMsg:
                logDebugCrash.append(logString.arg("Info"));
                *log << logString.arg("Info");
                break;
        }
        if (debugLogFile.device() != nullptr)
        {
            debugLogFile << logString.arg("Debug");
            debugLogFile << "\n";
            debugLogFile.flush();
        }
        if (type != QtDebugMsg)
        {
            *log << "\n";
            log->flush();
        }
    #ifdef SQPROJECT_DEVEL
        cout << QString("%1 : %2").arg(context.category, 20).arg(msg) << "\n";
        cout.flush();
    #endif
}

static void onCrash()
{
    QFile crashLog(crashLogPath);
    if (!crashLog.open(QIODevice::WriteOnly | QIODevice::Text))
        exit(1);
    crashLog.write(QString("Runing QUsb2Snes version " + sqApp->applicationVersion() + "\n").toUtf8());
    crashLog.write(QString("Compiled against Qt" + QString(QT_VERSION_STR) + ", running" + qVersion() + "\n").toUtf8());
    for (unsigned int i = 0; i < logDebugCrash.size(); i++)
    {
        crashLog.write(logDebugCrash.at(i).toUtf8() + "\n");
    }
    crashLog.flush();
    crashLog.close();
    exit(1);
}

static void seg_handler(int sig)
{
    Q_UNUSED(sig);
    onCrash();
}

void installSQLogging(const QString &fileBaseName, const QString &debugFileBaseName = "debug-log.txt", bool debugEnabled = false , const QString &crashFile = "")
{
    QFile   mlog(sqApp->logDirectoryPath() + "/" + fileBaseName);
    QFile   mDebugLog(sqApp->logDirectoryPath() + "/" + debugFileBaseName);

    sqApp->setDebugLogFilename(debugFileBaseName);
    sqApp->setLogFilename(fileBaseName);
    if (!crashFile.isEmpty())
    {
        signal(SIGSEGV, seg_handler);
        std::set_terminate(onCrash);
    }
    if (mlog.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        logfile.setDevice(&mlog);
    }
    if (debugEnabled)
    {
        if (mDebugLog.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            debugLogFile.setDevice(&mDebugLog);
        }
    }
    qInstallMessageHandler(sqLoggingDefaultMessageHandler);
}
