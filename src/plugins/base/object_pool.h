#ifndef BASE_OBJECT_POOL_H
#define BASE_OBJECT_POOL_H

#include <memory>
#include <QList>
#include <QMap>
#include <QObject>
#include "exception.h"

namespace base {
	
class ObjectPool : public QObject
{
public:
    ObjectPool(QObject *parent);
    void attach(QString id, QObject *object);
    bool contains(QString id);
    template <class T> T find(QString);
    QString diagnose();
private:
    typedef QMap<QString, QObject*> ObjectMap;
    ObjectMap objects;

    static ObjectPool *_objectPool;
    friend ObjectPool* objectPool();
};

template <class T> T ObjectPool::find(QString id) {
    if (!objects.contains(id))
        ThrowException("Cannot find object in object pool").value(id);
    else {
        T object = dynamic_cast<T>(objects.value(id));
        if (!object)
            ThrowException("Object found in object pool is not of expected type").value(id);
        return object;
    }
}

ObjectPool* objectPool();

} //namespace

#endif
