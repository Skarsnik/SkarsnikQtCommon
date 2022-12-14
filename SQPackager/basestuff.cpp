#include <QDir>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

#include <basestuff.h>
#include <runner.h>
#include <print.h>

void    error_and_exit(QString error)
{
   fprintf(stderr, "%s\n", error.toLocal8Bit().constData());
   exit(1);
}

ProjectDefinition    getProjectDescription(QString path)
{
    QString jsonPath = "sqproject.json";
    QString basePath = QDir::currentPath();
    if (!path.isEmpty())
    {
        jsonPath = path;
        basePath = QFileInfo(path).absolutePath();
    }
    QFile   desc(jsonPath);
    if (!desc.open(QIODevice::ReadOnly))
        error_and_exit("Can't open the project description file : " + desc.errorString());
    QByteArray json = desc.readAll();
    QJsonParseError* err = nullptr;
    QJsonDocument jsondoc = QJsonDocument::fromJson(json, err);
    if (err != nullptr)
        error_and_exit("Error parsing the description file : " + err->errorString());
    if (jsondoc.isNull())
    {
        error_and_exit("Invalid Json file somehow");
    }
    QJsonObject obj = jsondoc.object();
    println("Project name is : " + obj["name"].toString() + "\n");
    ProjectDefinition def;
    def.name = obj["name"].toString();
    def.shortDescription = obj["short-description"].toString();
    def.description = obj["description"].toString();
    def.icon = obj["icon"].toString();
    def.org = obj["org"].toString();
    def.proFile = basePath + "/" + def.name + ".pro";
    if (obj.contains("pro-file"))
        def.proFile = basePath + "/" + obj["pro-file"].toString();
    def.basePath = basePath;
    def.qtMajorVersion = "auto";
    if (obj.contains("qt-major-version"))
        def.qtMajorVersion = obj["qt-major-version"].toString();
    return def;
}


void    findQtModules(ProjectDefinition& def)
{
    if (QFileInfo::exists(def.name + ".pro"))
    {
        QFile proFile(def.name + ".pro");
        while (!proFile.atEnd())
        {
            QString line = proFile.readLine();
            QRegExp qtDef("QT\\s+\\+=");
            if (qtDef.indexIn(line) != -1)
            {
                QString stringDef = line.split("+=").at(1);
                QStringList qtmodule = stringDef.split(QRegExp("\\s+"));
                def.qtModules = qtmodule;
                break;
            }
        }
    }
}

/*
 * The version field behave like this
 * If the user set the version already, don't bother
 * the value "git" this will try to find a current tag, otherwise use a shortened commit number
 * the value "date" use the current date as date
*/

void    findVersion(ProjectDefinition& proj)
{
    std::function<QString()> gitFind([basePath=proj.basePath] {
        Runner run;
        bool ok = run.run("git", basePath, QStringList() << "describe" << "--tags" << "--exact-match");
        if (ok)
        {
            QString out = run.getStdout();
            if (out.isEmpty())
            {
                ok = run.run("git", basePath, QStringList() << "rev-parse" << "--verify master");
                return out.left(8);
            } else {
                return out.trimmed();
            }
        }
        return QString();
    });
    if (!proj.version.isEmpty())
    {
        println("Project version is user specified : " + proj.version);
        return ;
    }
    if (proj.version == "git")
    {
        proj.version = gitFind();
        println("Project version specified to use git");
        if (proj.version.isEmpty())
        {
            error_and_exit("Did not managed to determine a version using git");
        }
        println("Project version is " + proj.version);
        return ;
    }
    if (proj.version == "date")
    {
        println("Project version specified to use current date");
        proj.version = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        println("Project version is " + proj.version);
        return ;
    }
    println("Trying to find project version");
    proj.version = gitFind();
    if (!proj.version.isEmpty())
    {
        println("Project version is " + proj.version);
        return ;
    }
    println("Git failed, falling back to using current date");
    proj.version = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    println("Project version is " + proj.version);
}

const QRegularExpression varEntry("%%([\\w_]+)%%");

QString    useTemplateFile(QString rcPath, QMap<QString, QString> mapping)
{
    QFile   templateFile(rcPath);
    if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error_and_exit("Can't open template file " + templateFile.fileName() + " : " + templateFile.errorString());
    }
    QString toret;
    while (!templateFile.atEnd())
    {
        QString line = templateFile.readLine();
        auto matchs = varEntry.globalMatch(line);
        if (matchs.hasNext())
        {
            QString generatedLine;
            unsigned int indexStart = 0;
            while (matchs.hasNext())
            {
                auto match = matchs.next();
                // This take the part before the matching %%xx%%
                generatedLine.append(line.mid(indexStart, match.capturedStart(0) - indexStart));
                indexStart = match.capturedEnd(0);
                const QString key = match.captured(1);
                if (mapping.contains(key))
                {
                    generatedLine.append(mapping.value(key));
                }
            }
            generatedLine.append(line.mid(indexStart));
            toret.append(generatedLine);
        } else {
            toret.append(line);
        }
    }
    return toret;
}
