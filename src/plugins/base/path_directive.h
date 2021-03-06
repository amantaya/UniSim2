#ifndef PATH_DIRECTIVE_H
#define PATH_DIRECTIVEH

#include <QMap>
#include <QString>

namespace base {

namespace PathDirectives {
    enum Directive {
        Self, Children, Parent, Nearest,
        Descendants, Ascendants,
        AllSiblings, OtherSiblings, PreceedingSibling, FollowingSibling};
    Directive fromString(QString);
    QMap<QString, Directive> _pathDirectives;
}

}

#endif
