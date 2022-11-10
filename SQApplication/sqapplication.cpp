#include <QStandardPaths>
#include <sqapplication.h>


SQApplication* SQApplication::self = nullptr;

SQApplication::SQApplication(int &argc, char **argv) : _SQAPPLICATION_BASE_QT_TYPE(argc, argv)
{
    Q_ASSERT_X(!SQApplication::self, "SQApplication", "there should be only one SQApplication instance");
    SQApplication::self = this;
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
    if (isStandalone())
        return true;
}

QSettings* SQApplication::settings()
{
    return m_settings;
}


SQApplication *SQApplication::instance()
{
    return SQApplication::self;
}
