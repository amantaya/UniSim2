#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "vectorize.h"

namespace base {

unsigned Port::_trackFlags = Reset | Update;

Port::Port(QString name, QObject *parent, bool orphan)
    : QObject(parent), _valuePtr(0), _valueType(Null), _importPath(""),
      _access(Input), _reset(false), _track(this)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent) {
        if (orphan)
            boxParent->addOrphanPort(this);
        else
            boxParent->addPort(this);
    }
    _attributes["format"] = QString();
    _attributes["page"] = QString();
    _attributes["plot"] = QString();
    _attributes["label"] = QString();
    _attributes["transform"] = QString();
}

// Configure

Port& Port::equals(const char *value) {
    return equals(QString(value));
}

Port& Port::equals(QStringList value) {
    return equals(value.toVector());
}

Port& Port::imports(QString pathToPort) {
    _importPath = pathToPort;
    return *this;
}

Port& Port::access(Access acc) {
    _access = acc;
    return *this;
}

Port& Port::zeroAtReset() {
    _reset = true;
    return *this;
}

Port& Port::noReset() {
    _reset = false;
    return *this;
}


QStringList Port::attributes() {
    return QStringList( _attributes.keys() );
}


// Set attributes

Port& Port::attribute(QString name, QString value) {
    if (!_attributes.contains(name))
        ThrowException("Unknown attribute").value(name).context(this);
    _attributes[name] = value;
    return *this;
}

#define SET_ATTRIBUTE(X) \
Port& Port::X(QString fo) { \
    _attributes[#X] = fo; \
    return *this; \
}

SET_ATTRIBUTE(format)
SET_ATTRIBUTE(page)
SET_ATTRIBUTE(plot)
SET_ATTRIBUTE(label)
SET_ATTRIBUTE(transform)

Port& Port::transform(PortTransform tr) {
    return transform(convert<QString>(tr));
}

// Get attributes

QString Port::attribute(QString name) const {
    if (!_attributes.contains(name))
        ThrowException("Unknown attribute").value(name).context(this);
    return _attributes.value(name);
}

#define GET_ATTRIBUTE(X) \
QString Port::X() const { \
    return _attributes.value(#X); \
}

GET_ATTRIBUTE(format)
GET_ATTRIBUTE(page)
GET_ATTRIBUTE(plot)
GET_ATTRIBUTE(label)
GET_ATTRIBUTE(transform)

// Change

void Port::resolveImports() {
    if (_importPath.isEmpty())
        return;
    Box *context = boxParent();
    Path path = Path(_importPath, context);
    _importPorts = path.resolveMany<Port>();
    if (_importPorts.isEmpty())
        ThrowException("No matching import ports found").value(_importPath).context(this);
    _importType = commonType(_importPorts);
    if (_importType == Null)
        _importType = _valueType;
    // Check update order relatice to import
    for (Port *port : _importPorts) {
        int myOrder = context->order(),
            importOrder = port->boxParent()->order();
        QString s = "%1(#%2) imports %3(#%4)";
        QString msg = s.arg(fullName(this)).arg(myOrder).arg(fullName(port)).arg(importOrder);
        if (myOrder == importOrder)
            _warnings << "Warning: Port imports sibling port but the update order of siblings is undefined\n" + msg;
        else if (myOrder < importOrder)
            _warnings << "Warning: Port imports a port that will be updated after this port\n" + msg;
    }
}

void Port::allocatePortBuffer() {
    if (doTrack()) {
        Box *root = boxParent()->currentRoot();
        Port *iterations = root->peakPort("iterations"),
             *steps = root->peakPort("steps");
        int ite = iterations ? iterations->value<int>() : 1,
            ste = steps ? steps->value<int>() : 1;
        _track.reserve(ite*ste);
    }
}

void Port::reset() {
    if (!hasImport() && _reset)
        base::initialize(_valueType, _valuePtr);
}

void Port::copyFromImport() {
    if (!hasImport())
        return;
    assign(_importPorts);
}


void Port::assign(const QVector<Port*> &sources) {
    PortTransform trans = convert<PortTransform>(transform());
    // Create buffer for value if necessary
    if (!_valuePtr) {
        _valueType = sources.at(0)->type();
        if (sources.size() > 1 && trans != Identity)
            _valueType = asVector(_valueType);
        _valuePtr = portBuffer().createBuffer(_valueType);
    }
    // Now assign as scalar or vector
    if (sources.size() == 1) {
        const Port *source = sources.at(0);
        base::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, trans, this);
    }
    else {
        const void *sourceVector = vectorize(_importType, sources);
        base::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, trans, this);
    }
}

void Port::track(Step step) {
    if (doTrack() && (step & _trackFlags))
        _track.append(_valuePtr);
}

void Port::format(PortType type) {
    switch (type) {
        case Date:
        case DateVector:
            format("ymd");
            break;
        case Time:
        case TimeVector:
            format("hms");
            break;
        case DateTime:
        case DateTimeVector:
            format("ymdhms");
            break;
        default:
            format("NA");
    }
}

// Access

Box *Port::boxParent() {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        ThrowException("Application error: Port has no Box parent").context(this);
    return par;
}

bool Port::hasValue() const {
    return _valuePtr != 0;
}

#define CASE_VALUE_SIZE(X,Y) \
    case Y##Vector: \
        return reinterpret_cast<QVector<X>*>(_valuePtr)->size(); \
        break

int Port::valueSize() const {
    switch (_valueType) {
    CASE_VALUE_SIZE(bool, Bool);
    CASE_VALUE_SIZE(char, Char);
    CASE_VALUE_SIZE(int, Int);
    CASE_VALUE_SIZE(long int, LongInt);
    CASE_VALUE_SIZE(long long int, LongLongInt);
    CASE_VALUE_SIZE(float, Float);
    CASE_VALUE_SIZE(double, Double);
    CASE_VALUE_SIZE(long double, LongDouble);
    CASE_VALUE_SIZE(QString, String);
    CASE_VALUE_SIZE(QDate, Date);
    CASE_VALUE_SIZE(QDateTime, DateTime);
    CASE_VALUE_SIZE(QTime, Time);
    default:
        return 1;
    }
}

template <> const void* Port::valuePtr() const {
    return _valuePtr;
}

const Vector* Port::trackPtr() const {
    return doTrack() ? &_track : 0;
}

PortType Port::type() const {
    return _valueType;
}

Port::Access Port::access() const {
    return _access;
}

bool Port::doTrack() const {
    return !(attribute("plot").isNull() && attribute("page").isNull());
}

bool Port::hasImport() const {
    return !_importPath.isEmpty();
}

QString Port::importPath() const {
    return _importPath;
}

QVector<Port*> Port::importPorts() const {
    return _importPorts;
}

QStringList Port::warnings() const {
    return _warnings;
}

QVector<Port*> Port::trackedPorts() {
    QVector<base::Port*> result, all = Path("*{Port}").resolveMany<Port>();
    for (Port *port : all) {
        if (port->trackPtr())
            result << port;
    }
    return result;
}

PortType Port::commonType(const QVector<Port*> &ports) {
    PortType commonType = ports.at(0)->_valueType;
    for (Port *port : ports) {
        if (port->_valueType != commonType) {
            commonType = Null;
            break;
        }
    }
    return commonType;
}

}
