#ifndef HELP_CLASS_H
#define HELP_CLASS_H
#include <QStringList>
#include <base/command.h>
#include <base/port.h>
#include <base/port_access.h>

namespace base {
    class Box;
}

namespace command {

class help_class : public base::Command
{
public:
    help_class(QString name, QObject *parent);
private:
    // data
    base::Box *_box;
    int _maxPortNameLength, _maxValueLength;
    // methods
    void doExecute();
    void createBox(QString className);
    void setNameLengths();
    void writeHelp();
    QStringList portsHelp(base::PortAccess access);
    QString sideEffects();
};

}

#endif
