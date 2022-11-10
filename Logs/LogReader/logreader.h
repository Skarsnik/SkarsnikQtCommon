#ifndef LOGREADER_H
#define LOGREADER_H

#include <QMainWindow>
#include <QSet>
#include <QTextDocument>

namespace Ui {
class LogReader;
}

class LogReader : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogReader(QWidget *parent = nullptr);
    ~LogReader();

private:
    Ui::LogReader *ui;
    QTextDocument   logDocument;
    QSet<QString>     categories;

    void    openLog(QString path);
    void    categoryCheckBoxStateChanged(int state);
};

#endif // LOGREADER_H
