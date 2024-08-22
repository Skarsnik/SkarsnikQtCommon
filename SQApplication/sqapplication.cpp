#include <QStandardPaths>
#include <QTranslator>
#include <sqapplication.h>


SQApplication* SQApplication::self = nullptr;

SQApplication::SQApplication(int &argc, char **argv) : _SQAPPLICATION_BASE_QT_TYPE(argc, argv)
{
    Q_ASSERT_X(!SQApplication::self, "SQApplication", "there should be only one SQApplication instance");
    SQApplication::self = this;
    m_settings = nullptr;
}

bool SQApplication::hasGitInformation() const
{
#ifdef SQPROJECT_HAS_GIT
    return true;
#endif
    return false;
}

QString SQApplication::gitCommitNumber() const
{
#ifdef SQPROJECT_HAS_GIT
    return QString(SQPROJECT_GIT_COMMIT);
#endif
    return QString();
}

QString SQApplication::gitTag() const
{
#ifdef SQPROJECT_HAS_GIT
    return QString(SQPROJECT_GIT_TAG);
#endif
    return QString();
}


bool SQApplication::isStandalone() const
{
#ifdef SQPROJECT_STANDALONE
    return true;
#endif
    return false;
}

bool SQApplication::createSettings()
{
    if (applicationName().isEmpty())
        return false;
    if (isStandalone())
    {
        m_settings = new QSettings(applicationDirPath() + "/" + applicationName() + ".ini", QSettings::IniFormat);
    } else {
#ifdef Q_OS_WIN
        m_settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" + applicationName() + ".ini", QSettings::IniFormat);
#else
        m_settings = new QSetting();
#endif
    }
    return true;
}

QSettings* SQApplication::settings()
{
    return m_settings;
}

bool SQApplication::setTranslation(const QString &fileSuffix)
{
    m_translator = new QTranslator();
    QString locale = QLocale::system().name().split('_').first();
    return m_translator->load(SQPath::translationsPath() + "/" + fileSuffix + locale + ".qm");
}


SQApplication *SQApplication::instance()
{
    return SQApplication::self;
}
