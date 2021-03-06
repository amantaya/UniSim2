#ifndef SIMULATION_H
#define SIMULATION_H
#include <QList>
#include <QTime>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class Simulation : public base::Box
{
public:
    Simulation(QString name, QObject *parent);
    void amend();
    void cleanup();
    void debrief();
    void run();
private:
    // Inputs
    int iterations, steps;
    bool stopIterations, stopSteps,
        useStopIterations, useStopSteps;
    // Outputs
    int iteration, step, finalStep, executionTime;
    bool hasError;
    QString errorMsg;
    // Data
    double nextShowProgress;
//    QList<base::Port*> _trackedPorts;
//    QFile _file;
//    QTextStream _stream;
    // Methods
    void collectTrackedPorts();
    void removeObsoletePorts();
    void makePortLabelsUnique();
    void show(QTime time);
    void writeDataFrame();
    void openFileStream(QString extension);
    static bool valuesAreEqual(const base::Port *port);
};

}

#endif
