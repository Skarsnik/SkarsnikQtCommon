#ifndef LOGTEXTBLOCKUSERDATA_H
#define LOGTEXTBLOCKUSERDATA_H

#include <QDateTime>
#include <QTextBlockUserData>

class LogTextBlockUserData : public QTextBlockUserData
{
    public:
        QDateTime   timestamp;
        QString     category;
        QtMsgType   type;
};

#endif // LOGTEXTBLOCKUSERDATA_H
