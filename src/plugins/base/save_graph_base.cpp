#include "box.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "port.h"
#include "save_graph_base.h"

namespace base {

SaveGraphBase::SaveGraphBase() {
}

void SaveGraphBase::writeGraphFile() {
    openFileStream();
    writeBeginning();
    writeBoxes();
    writeBoxRelations();
    writePortEdges();
    writeEnd();
    closeFileStream();
}

QVector<Box*> SaveGraphBase::boxesToGraph() {
    QVector<Box*> boxes;
    Box *root = environment().root();
    if (root)
        boxes = root->findMany<Box>("*");
    return boxes;
}

QVector<QPair<Port*, Port*>> SaveGraphBase::portEdgesToGraph() {
    QVector<Port*> all;
    QVector<QPair<Port*, Port*>> edges;
    Box *root = environment().root();
    if (root) {
        root->initializeFamily();
        all = root->findMany<Port>("*[*]");
        for (Port *receiver : all) {
            for (Port *sender : receiver->importPorts()) {
                edges << qMakePair(sender, receiver);
            }
        }
    }
    return edges;
}

}
