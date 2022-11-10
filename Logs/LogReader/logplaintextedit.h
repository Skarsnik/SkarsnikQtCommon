#ifndef LOGPLAINTEXTEDIT_H
#define LOGPLAINTEXTEDIT_H



#include "logsyntaxhighlighter.h"

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>

class LogPlainTextEdit : public QPlainTextEdit
{
public:
    LogPlainTextEdit(QWidget *parent = nullptr);
    void sideBarAreaPaintEvent(QPaintEvent *event);
    int  sideBarAreaWidth();

protected:
    void    resizeEvent(QResizeEvent*   event) override;
private slots:
    void updateSideBarAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateSideBarArea(const QRect &rect, int dy);

private:
    QWidget* sideBar;
    LogSyntaxHighlighter*   syntax;
};

#endif // LOGPLAINTEXTEDIT_H
