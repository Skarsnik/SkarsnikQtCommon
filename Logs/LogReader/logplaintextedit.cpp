#include "logplaintextedit.h"
#include "logplaintexteditsidebar.h"
#include "logtextblockuserdata.h"

#include <QPainter>
#include <QTextBlock>
#include <QDebug>


LogPlainTextEdit::LogPlainTextEdit(QWidget* parent) : QPlainTextEdit (parent)
{
    sideBar = new LogPlainTextEditSideBar(this);
    syntax = new LogSyntaxHighlighter(this);
    syntax->setDocument(document());
    connect(this, &LogPlainTextEdit::blockCountChanged, this, &LogPlainTextEdit::updateSideBarAreaWidth);
    connect(this, &LogPlainTextEdit::updateRequest, this, &LogPlainTextEdit::updateSideBarArea);
    //connect(this, &LogPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    //updateLineNumberAreaWidth(0);
}


void LogPlainTextEdit::sideBarAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(sideBar);
    QColor bg(Qt::lightGray);
    bg = bg.lighter(120);
    painter.fillRect(event->rect(), bg);

    QTextBlock block = firstVisibleBlock();
    if (!block.isValid())
        return ;
    LogTextBlockUserData* blockData = static_cast<LogTextBlockUserData*>(document()->firstBlock().userData());
    QDateTime refTime = blockData->timestamp;
    //int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            blockData = static_cast<LogTextBlockUserData*>(block.userData());
            qint64 timediff = refTime.secsTo(blockData->timestamp);
            painter.setPen(QColor(Qt::black).lighter());
            painter.drawLine(sideBar->width() - 2, top, sideBar->width() - 2, bottom);
            painter.setPen(Qt::gray);
            painter.drawText(0, top, sideBar->width() - 2, fontMetrics().height(),
                             Qt::AlignRight, QString::number(timediff));
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
}

int LogPlainTextEdit::sideBarAreaWidth()
{
    return 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * 10;
}

void LogPlainTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    sideBar->setGeometry(QRect(cr.left(), cr.top(), sideBarAreaWidth(), cr.height()));
}

void LogPlainTextEdit::updateSideBarAreaWidth(int newBlockCount)
{
    setViewportMargins(sideBarAreaWidth(), 0, 0, 0);
}

void LogPlainTextEdit::updateSideBarArea(const QRect &rect, int dy)
{
    if (dy)
        sideBar->scroll(0, dy);
    else
        sideBar->update(0, rect.y(), sideBar->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateSideBarAreaWidth(0);
}
