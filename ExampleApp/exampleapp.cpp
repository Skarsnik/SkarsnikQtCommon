
#include <sqapplication.h>
#include "exampleapp.h"
#include "ui_exampleapp.h"


ExampleApp::ExampleApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleApp)
{
    ui->setupUi(this);
    ui->labelDebugLogPath->setText(sqApp->debugLogFilePath());
    ui->labelInfoLogPath->setText(sqApp->logFilePath());
    ui->labelTypeInstall->setText(sqApp->isStandalone() ? "The application is build in standalone mode" : "The application is build for installation");
    ui->labelInfoComp->setText(QString("Compiled with %1 running on %2. Build Architecture is : %3, running Architecture is %4").arg(QT_VERSION_STR).arg(qVersion()).arg(QSysInfo::buildCpuArchitecture()).arg(QSysInfo::currentCpuArchitecture()));
}

ExampleApp::~ExampleApp()
{
    delete ui;
}

