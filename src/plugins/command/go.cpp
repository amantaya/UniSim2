#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "general.h"
#include "go.h"

using namespace base;

namespace command {

PUBLISH(go)
HELP(help_go, "go <path expression>", "goes to object matching path expression, must be unique")

go::go(QString name, QObject *parent)
    : Command(name, parent)
{
}

void go::doExecute() {
    QVector<QObject*> matches;
    if (_args.size() == 2) {
        QString path = _args.at(1);
        Box *box = environment().current();
        if (box) {
            matches = box->findMany<QObject>(path);
            if (matches.isEmpty())
                dialog().error("No matches");
            else if (matches.size() > 1) {
                dialog().error("Path must be unique; too many matches:\n" + info(matches));
            }
            else {
                QObject *match = matches.at(0);
                Box *box = dynamic_cast<Box*>(match);
                if (!box)
                    dialog().error("Path does not match a box:\n" + info(match));
                else {
                    environment().current(box);
                    dialog().information("Now at " + info(box));
                }
            }
        }
        else
            dialog().error("No matches; no objects loaded");
    }
    else
        dialog().error("Write: 'go <path expression>'");
}

}
