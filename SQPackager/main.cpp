#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QCommandLineParser>
#include <QDateTime>

#include <runner.h>
#include <sqpackager.h>
#include <basestuff.h>

QTextStream cout(stdout);

PackagerOptions gOptions;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;

    parser.addPositionalArgument("sqproject.json", "A path to a sqproject.json file");
    parser.addOptions({
                          {"version", "version", "Force the given version for the project"},
                          {"gen-flatpak", "Generate a flatpak manifest"},
                          {"gen-windows", "Check and generate Windows related stuff"},
                          {"windows-build-path", "path", "Set the base directory to build on Windows"},
                          {"windows-deploy-path", "path", "Set the base directory to deploy on Windows"}
                      });
    //return a.exec();
    parser.process(a);
    ProjectDefinition project;
    if (parser.positionalArguments().isEmpty())
    {
        project = getProjectDescription();
    } else {
        project = getProjectDescription(parser.positionalArguments().at(0));
    }
    findQtModules(project);
    if (parser.isSet("version"))
        project.version = parser.value("version");
    else
    {
        findVersion(project);
    }
    if (parser.isSet("windows-build-path"))
        gOptions.windowsBuildPath = parser.value("windows-build-path");
    if (parser.isSet("windows-deploy-path"))
        gOptions.windowsDeployPath = parser.value("windows-deploy-path");
    if (parser.isSet("gen-flatpak"))
        genFlatPakFile(project);
    if (parser.isSet("gen-windows"))
        genWindows(project);
    buildWindows(project);
}
