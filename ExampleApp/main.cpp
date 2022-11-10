
#include <sqlogging.h>
#include <sqapplication.h>
#include "exampleapp.h"

int main(int argc, char *argv[])
{
    SQApplication a(argc, argv);
    ExampleApp w;
    installSQLogging("log.txt", "debug-log.txt", true, "crash-log.txt");
    w.show();
    return a.exec();
}
