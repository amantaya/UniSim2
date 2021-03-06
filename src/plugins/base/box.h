#ifndef BASE_BOX_H
#define BASE_BOX_H
#include <QMap>
#include <QObject>
#include <QTextStream>
#include <QVariant>
#include <QVector>
#include "box_step.h"
#include "construction_step.h"
#include "enum_functions.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "port_access.h"

#define RETURN_PLUGIN_NAME(x) #x

#define Input(X) (*new base::Port(#X, this)).data(& X).access(base::PortAccess::Input)
#define Output(X) (*new base::Port(#X, this)).data(& X).access(base::PortAccess::Output).zeroAtReset()
#define NamedOutput(X,Y) (*new base::Port((X), this)).data(& (Y)).access(PortAccess::Output).zeroAtReset()

namespace base {

class Port;
class Timer;

class Box : public QObject, public ConstructionStep
{
public:
    enum class ToTextOptions{None=0, Boxes=1, Ports=2, ChangedPorts=4, Help=8, Recurse=16};
    Box(QString name, QObject *parent);
    ~Box();
    QString pluginName() const { return RETURN_PLUGIN_NAME(BOXES_PLUGIN_NAME); }
    ComputationStep computationStep() const;
    void addPort(Port *port);
    Port* peakPort(QString name);
    const Port* peakPort(QString name) const;
    Port* port(QString name);
    const Port* port(QString name) const;

    void help(QString s);
    QString help() const;
    void sideEffects(QString s);
    QString sideEffects() const;

    static Box* currentRoot();
    static void saveCurrentRoot();
    static void restoreCurrentRoot();

    QString className() const;
    QString name() const;
    QString fullName() const;
    int order() const;
    static int count();
    QString profileReport() const;

    template<class T=QObject> T* findOne(QString path);
    template<class T=QObject> T* findMaybeOne(QString path);
    template<class T=QObject> QVector<T*> findMany(QString path);

    virtual void amend() {}
    virtual void initialize() {}
    virtual void reset() {}
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    virtual void import() {}
    virtual void run();

    void amendFamily();
    void initializeFamily();
    void resetFamily();
    void updateFamily();
    void cleanupFamily();
    void debriefFamily();

    void resolvePortImports();
    void updateImports();

    void cloneFamily(QString name, QObject *parent);

    void toText(QTextStream &text, ToTextOptions options, int indentation = 0) const;
private:
    // Data
    QString _name, _help, _sideEffects;
    ComputationStep _computationStep;
    QMap<QString,Port*> _ports, _orphanPorts;
    QVector<Port*> _trackedPorts;
    int _order;
    bool _amended;
    Timer *_timer;
    static Box *_currentRoot, *_savedCurrentRoot;
    static int _count;
    // Methods
    void createTimers();
    void addPort(QMap<QString,Port*> &ports, Port *port);
    void enumerateBoxes(int &i);
    void allocatePortBuffers();
    void collectTrackedPorts();
    void resetPorts();
    void trackPorts(Step step);
};

DEFINE_ENUM_FUNCTIONS(Box::ToTextOptions)

template<class T> T* Box::findOne(QString path) {
    return Path(path, this).resolveOne<T>(this);
}

template<class T> T* Box::findMaybeOne(QString path) {
    return Path(path, this).resolveMaybeOne<T>(this);
}

template<class T> QVector<T*> Box::findMany(QString path) {
    return Path(path, this).resolveMany<T>(this);
}

}
#endif
