#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputR);
    Input(xAxis).imports("/*[step]");
}

//
// amend
//

void OutputR::amend() {
    // Collect names of pages and plots attributed to tracked ports
    QVector<Port*> trackedPorts = Port::trackedPorts();
    QMap<QString, QSet<QString>> pagesWithPlots;
    for (Port *port : trackedPorts) {
        pagesWithPlots[port->page()] << port->plot();
    }
    // If more than one page exists then remove default page named "page"
    if (pagesWithPlots.size() > 1)
        pagesWithPlots.remove("page");
    // Create pages with plots
    QMapIterator<QString, QSet<QString>> it(pagesWithPlots);
    while (it.hasNext()) {
        it.next();
        QString pageName = it.key();
        QSet<QString> plotNames = it.value();
        Box *page = MegaFactory::create<PageR>("PageR", pageName, this);
        // The page imports the same axis as the general one for OutputR
        page->port("xAxis")->imports( port("xAxis")->importPath() );
        for (QString plotName : plotNames)
            MegaFactory::create<PlotR>("PlotR", plotName, page);
    }
    // Make certain that x-axis is include in output data frame
    setTrackX();
}

void OutputR::setTrackX() {
    Port *xPort = port("xAxis");
    xPort->resolveImports();
    QVector<Port*> importPorts = xPort->importPorts();
    if (importPorts.size() != 1) {
        QString msg{"Expected one x-axis, got '%1'"};
        throw Exception(msg.arg(importPorts.size()), port("xAxis")->importPath(), this);
    }
    importPorts[0]->trackOn();
}


//
// initialize
//

void OutputR::initialize() {
    try {
        collectInfo();
    }
    catch (Exception &ex) {
        QString msg{"Unexpected error in OutputR::collectInfo:\n"};
        throw( Exception(msg+ex.what(), "", this) );
    }
}

void OutputR::collectInfo() {
    QVector<Box*> pages = Path("children::*{PageR}", this).resolve<Box>();
    for (Box *page : pages)
        _pageInfos << PageInfo(page);
}

QString OutputR::toString() {
    QString s;
    for (PageInfo pageInfo : _pageInfos)
        s += pageInfo.toString();
    return s;
}

QString OutputR::toScript() {
    QString s;
    for (PageInfo pageInfo : _pageInfos)
        s += pageInfo.toScript();
    return s;
}

OutputR::PageInfo::PageInfo(Box *page)
    : _page(page)
{
    QVector<Box*> plots = Path("children::*{PlotR}", page).resolve<Box>();
    for (Box *plot : plots)
        _plotInfos << PlotInfo(plot);
}

QString OutputR::PageInfo::toString() {
    QString s = _page->className() + " " + _page->objectName() + "\n";
    for (PlotInfo plotInfo : _plotInfos)
        s += plotInfo.toString();
    return s;
}

QString OutputR::PageInfo::toScript() {
    QString string;
    QTextStream s(&string);
    s << _page->objectName() << " = function(df) {\n"
      << "  windows("
      << _page->port("width")->value<int>()
      << ", "
      << _page->port("height")->value<int>()
      << ")\n"
      << "  grid.arrange(\n" ;
    for (PlotInfo plotInfo : _plotInfos)
        s << plotInfo.toScript();
    s << "    nrow = " << _page->port("nrow")->value<int>() << "\n  )\n}\n";
    return string;
}

OutputR::PlotInfo::PlotInfo(Box *plot)
    : _plot(plot)
{
    collectPorts();
}

void OutputR::PlotInfo::collectPorts() {
    Box *parent = dynamic_cast<Box*>(_plot->parent());
    Q_ASSERT(parent);
    QString pageName = parent->objectName(),
            plotName = _plot->objectName();

    for (Port *port : Port::trackedPorts()) {
        if (port->page() == pageName && port->plot() == plotName && port->trackPtr())
            _ports << port;
    }
}

QString OutputR::PlotInfo::toString() {
    QString s = "Plot: " + _plot->objectName() + "\n"; //" " + _parent->test +
    for (const Port *port : _ports)
        s += "  Port: " + port->objectName() + "\n";
    return s;
}

inline QString apostrophed(QString s) {
    return "\"" + s + "\"";
}

QString OutputR::PlotInfo::toScript() {
    QStringList portLabels;
    for (const Port *port : _ports)
        portLabels << apostrophed(port->label());
    QString xLabel = apostrophed(xPortLabel());
    return "    unisim_plot(df, " + xLabel + ", c(" + portLabels.join(", ") + ")),\n";
}

QString OutputR::PlotInfo::xPortLabel() {
    Box *parent = dynamic_cast<Box*>(_plot->parent());
    Q_ASSERT(parent);
    QString xPath = parent->port("xAxis")->importPath();
    Port *xPort = Path(xPath).resolveOne<Port>();
    return xPort->label();
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
}

void OutputR::writeScript() {
    openFile();
    QTextStream script(&_file);
    script << toScript();
    _file.close();
}

void OutputR::openFile() {
    QString filePath = environment().outputFilePath(".R");
    filePath.replace("\\", "/");
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        throw Exception("Cannot open file for output", filePath, this);
    environment().copyToClipboard("source(\""+filePath+"\")\n");
}


}
