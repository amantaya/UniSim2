//#include <QApplication>
#include <base/exception.h>
#include <base/organisation.h>
#include "main_window.h"

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    throw base::Exception(QString(msg));
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMsgHandler);

//    QCoreApplication::setOrganizationName("Aarhus University");
//    QCoreApplication::setOrganizationDomain("www.ecolmod.org");
//    QCoreApplication::setApplicationName("Universal Simulator");

    QApplication app(argc, argv);
    app.setObjectName("application");
    MainWindow window;
    window.show();
    return app.exec();
}
