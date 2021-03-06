#ifndef BASE_PRODUCT_H
#define BASE_PRODUCT_H
#include <QObject>
#include <QString>
#include "product_base.h"

#define PUBLISH(name) Product<name> name##Product(#name, productList());

namespace base {

template <class T>
class Product : public ProductBase
{
public:
    Product(QString name, ProductList &list);
    QObject* create(QString objectName, QObject *parent) const;
};

template <class T>
Product<T>::Product(QString name, ProductList &list)
    :  ProductBase(name, list)
{
}

template <class T>
QObject* Product<T>::create(QString objectName, QObject *parent) const
{
    return new T(objectName, parent);
}

} //namespace

#endif
