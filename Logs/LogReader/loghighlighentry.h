#ifndef LOGHIGHLIGHENTRY_H
#define LOGHIGHLIGHENTRY_H

#include <QRegularExpression>
#include <QTextCharFormat>


struct LogHighLighEntry {
    QRegularExpression  pattern;
    QTextCharFormat     format;
};

#endif // LOGHIGHLIGHENTRY_H
