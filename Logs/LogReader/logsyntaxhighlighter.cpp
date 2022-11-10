#include "logsyntaxhighlighter.h"
#include <QTextCharFormat>

LogSyntaxHighlighter::LogSyntaxHighlighter(QObject* parent) : QSyntaxHighlighter(parent)
{

}

void LogSyntaxHighlighter::setFormatList(QList<LogHighLighEntry> piko)
{
    formats = piko;
}

//  SNESClassic Factory - Debug: Command finished, triming the 4 trailing 0
void LogSyntaxHighlighter::highlightBlock(const QString &text)
{
    // Base renderding
    if (text.size() == 0)
        return;
    QTextCharFormat lightGray;
    lightGray.setForeground(Qt::lightGray);
    QTextCharFormat lightBlue;
    lightBlue.setForeground(Qt::blue);
    setFormat(23, 8, lightGray);
    setFormat(0, 21, lightBlue);
    for (auto format :  formats)
    {
        if (format.pattern.match(text).isValid())
        {
            setFormat(0, 100, format.format);
        }
    }
}
