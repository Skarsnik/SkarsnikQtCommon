#include "logreader.h"
#include "ui_logreader.h"
#include "flowlayout.h"
#include "logtextblockuserdata.h"

#include <QCheckBox>
#include <QLabel>
#include <QRegularExpression>
#include <QTextBlock>
#include <QDebug>

LogReader::LogReader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogReader)
{
    ui->setupUi(this);
    ui->catGroupBox->setLayout(new FlowLayout());
    openLog("F:/Project/SkarsnikQtCommon/Logs/LogReader/beemer-debug.txt");
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->logTextView->setFont(font);
}

LogReader::~LogReader()
{
    delete ui;
}

/*
2022-05-15T20:35:37              default - Debug: Runing QUsb2Snes version  "0.7.23"
2022-05-15T20:35:37              default - Debug: Compiled against Qt 5.12.6 , running 5.12.6
2022-05-15T20:35:37  SNESClassic Factory - Debug: SNES Classic device will try to connect to  "169.254.13.37"
2022-05-15T20:35:37             WSServer - Debug: Adding Device Factory  "SNES Classic (Hakchi2CE)"
*/

void LogReader::openLog(QString path)
{
    QFile   logFile(path);
    categories.clear();
    ui->logTextView->clear();
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!logFile.atEnd())
        {
            auto entry = logFile.readLine();
            entry.chop(1);
            //ui->logTextView->appendPlainText(entry);
            QRegularExpression logRegex("([^\\s]+)\\s+([^-]+)-([^:]+):(.+)");
            auto match = logRegex.match(entry);
            LogTextBlockUserData* blockData = new LogTextBlockUserData();
            if (match.hasMatch())
            {
                categories.insert(match.captured(2).trimmed());
                QString stringLevel = match.captured(3).trimmed();
                if (stringLevel == "Debug")
                    blockData->type = QtDebugMsg;
                if (stringLevel == "Info")
                    blockData->type = QtInfoMsg;
                qDebug() << match.captured(1).trimmed();
                blockData->timestamp = QDateTime::fromString(match.captured(1).trimmed(), Qt::ISODate);
                blockData->category = match.captured(2).trimmed();
            }
            ui->logTextView->appendPlainText(entry.mid(19));
            ui->logTextView->document()->lastBlock().setUserData(blockData);
        }
    }
    for (auto cat : categories)
    {
        FlowLayout* layout = static_cast<FlowLayout*>(ui->catGroupBox->layout());
        QCheckBox* catCheckBox = new QCheckBox(cat);
        catCheckBox->setChecked(true);
        connect(catCheckBox, &QCheckBox::stateChanged, this, &LogReader::categoryCheckBoxStateChanged);
        layout->addWidget(catCheckBox);
    }
}

void LogReader::categoryCheckBoxStateChanged(int state)
{
    QCheckBox* check = qobject_cast<QCheckBox*>(this->sender());
    QTextBlock block = ui->logTextView->document()->begin();
    while (block.isValid())
    {
        LogTextBlockUserData* data = static_cast<LogTextBlockUserData*>(block.userData());
        //qDebug() << data->category;
        if (state == 0 && data->category == check->text())
            block.setVisible(false);
        if (state != 0 && data->category == check->text())
            block.setVisible(true);
        block = block.next();
    }
    ui->logTextView->setDocument(ui->logTextView->document());
}
