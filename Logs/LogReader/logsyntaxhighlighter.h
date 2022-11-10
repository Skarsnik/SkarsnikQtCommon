#ifndef LOGSYNTAXHIGHLIGHTER_H
#define LOGSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "loghighlighentry.h"


class LogSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    LogSyntaxHighlighter(QObject *parent = 0);
    void        setFormatList(QList<LogHighLighEntry>);


    // QSyntaxHighlighter interface
protected:
    void highlightBlock(const QString &text);

private:
    QList<LogHighLighEntry> formats;
};

#endif // LOGSYNTAXHIGHLIGHTER_H
