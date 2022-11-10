#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleApp; }
QT_END_NAMESPACE

class ExampleApp : public QMainWindow
{
    Q_OBJECT

public:
    ExampleApp(QWidget *parent = nullptr);
    ~ExampleApp();

private:
    Ui::ExampleApp *ui;
};
#endif // EXAMPLEAPP_H
