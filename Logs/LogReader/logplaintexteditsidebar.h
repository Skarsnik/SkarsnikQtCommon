#ifndef LOGPLAINTEXTEDITSIDEBAR_H
#define LOGPLAINTEXTEDITSIDEBAR_H

#include "logplaintextedit.h"

#include <QWidget>

class LogPlainTextEditSideBar : public QWidget
{
    Q_OBJECT
public:
    LogPlainTextEditSideBar(LogPlainTextEdit* editor) : QWidget(editor), logEdit(editor) {}
    QSize sizeHint() const override
    {
        return QSize(logEdit->sideBarAreaWidth(), 0);
    }
protected:
    void paintEvent(QPaintEvent *event) override
    {
        logEdit->sideBarAreaPaintEvent(event);
    }
private:
    LogPlainTextEdit* logEdit;
};

#endif // LOGPLAINTEXTEDITSIDEBAR_H
