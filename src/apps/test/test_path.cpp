#include <iostream>
#include <QSet>
#include <base/box.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include "test_box_cases.h"
#include "test_path.h"

using namespace base;
using namespace std;

void TestPath::initTestCase() {
    _root = TestBoxCases::case1();
}

void TestPath::cleanupTestCase() {
    delete _root;
}

void TestPath::setContext(QString path) {
    if (path.isEmpty()) {
        _context = 0;
        return;
    }
    Path p{path};
    QVector<QObject*> objects;
    try {
        objects = p.resolveMany();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    int n = objects.length();
    QString msg{"Wrong number of box matches in '%1'. Expected 1 but found %2"};
    msg = msg.arg(path).arg(n);
    QVERIFY2(n==1, msg.toLocal8Bit());
    _context = objects.at(0);
}

namespace {
    QString names(QSet<QObject*> set) {
        QStringList s;
        for (QObject *item : set) {
            s << fullName(item);
        }
        return s.join(" ");
    }
}

void TestPath::compareVectors(QVector<QObject*> v1, QVector<QObject*> v2, int size) {
    QSet<QObject*> set1 = QSet<QObject*>::fromList( QList<QObject*>::fromVector(v1) ),
                   set2 = QSet<QObject*>::fromList( QList<QObject*>::fromVector(v2) );
    QString msgNames = "Object sets differ:\n(%1)\n(%2)",
            msgSize = "Object set(s) of wrong size. Excepted %1 got %2 and %3.\nExpected: '%4'";
    QString s1 = msgNames.arg(names(set1)).arg(names(set2));
    QString s2 = msgSize.arg(size).arg(set1.size()).arg(set2.size());

    QVERIFY2(set1==set2, msgNames.arg(names(set1)).arg(names(set2)).toLocal8Bit());

    QVERIFY2(set1.size()==size, msgSize.arg(size).arg(set1.size()).arg(set2.size()).arg(names(set2)).toLocal8Bit());
}

void TestPath::testValidateName() {
    Path path("/A/A2");
    try {
        path.validateName("ABC");
        path.validateName("*");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }

    bool excepted = false;
    try {
        path.validateName(".");
        path.validateName("A*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPath::testValidateStep() {
    Path path("/A/A2");
    try {
        path.validateStep("ABC");
        path.validateStep("ABC:");
        path.validateStep("*");
        path.validateStep(".");
        path.validateStep("..");
        path.validateStep("...");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }

    bool excepted = false;
    try {
        path.validateName(".A");
        path.validateName("A:B");
        path.validateStep("....");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPath::testNormalise() {
    QCOMPARE(Path("plant/fruit/area[v]").normalise(),
         QString("selfordescendants:plant<Box>/children:fruit<Box>/children:area<Box>/children:v<Port>"));

    QCOMPARE(Path("children::plant/fruit/nearest:area[v]").normalise(),
         QString("children:plant<Box>/children:fruit<Box>/nearest:area<Box>/children:v<Port>"));

    QCOMPARE(Path("./plant/fruit/area[v]").normalise(),
        QString("self:*<Box>/children:plant<Box>/children:fruit<Box>/children:area<Box>/children:v<Port>"));

    QCOMPARE(Path("../plant/fruit/area[v]").normalise(),
        QString("parent:*<Box>/children:plant<Box>/children:fruit<Box>/children:area<Box>/children:v<Port>"));

    QCOMPARE(Path("/plant/fruit/area[v]").normalise(),
        QString("self:plant<Box>/children:fruit<Box>/children:area<Box>/children:v<Port>"));

    QCOMPARE(Path(".[v]").normalise(),
        QString("self:*<Box>/children:v<Port>"));

    QCOMPARE(Path("..[v]").normalise(),
        QString("parent:*<Box>/children:v<Port>"));

    QCOMPARE(Path("...[v]").normalise(),
        QString("nearest:v<Port>"));

    QCOMPARE(Path("/").normalise(),
        QString("self:*<Box>"));

    QCOMPARE(Path("/plant").normalise(),
        QString("self:plant<Box>"));

    QCOMPARE(Path("/plant[v]").normalise(),
        QString("self:plant<Box>/children:v<Port>"));

    QCOMPARE(Path("../plant[v]").normalise(),
        QString("parent:*<Box>/children:plant<Box>/children:v<Port>"));

    QCOMPARE(Path("../../plant[v]").normalise(),
        QString("parent:*<Box>/parent:*<Box>/children:plant<Box>/children:v<Port>"));

    QCOMPARE(Path(".../plant[v]").normalise(), Path("nearest:plant[v]").normalise());

    QCOMPARE(Path(".../plant[v]").normalise(),
        QString("nearest:plant<Box>/children:v<Port>"));
}


void TestPath::testRobustness() {
    // Search paths may be dubious but should not cause a hard crash
    setContext("A2");
    try {
        Path("plant/.../fruit/area[v]", _context).resolveMany();
        Path("plant/fruit/area/...", _context).resolveMany();
        Path("././plant/fruit/area", _context).resolveMany();
        Path("./../plant/fruit/area", _context).resolveMany();
        Path("./.../plant/fruit/area", _context).resolveMany();
        Path(".././plant/fruit/area", _context).resolveMany();
        Path("../.../plant/fruit/area", _context).resolveMany();
        Path("..././plant/fruit/area", _context).resolveMany();
        Path(".../../plant/fruit/area", _context).resolveMany();
        Path(".../.../plant/fruit/area", _context).resolveMany();
    }
    catch (Exception &ex) {
    }
}

void TestPath::testSetContext() {
    setContext("A2");
    QCOMPARE(_context->objectName(), QString("A2"));
}

void TestPath::testSelf() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("self:*", _context).resolveMany(),
    absolute = Path("/A/A2").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("self:*<Box>", _context).resolveMany(),
    absolute = Path("/A/A2").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path(".[v2]", _context).resolveMany(),
    absolute = Path("/A/A2[v2]").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("./b[v1]", _context).resolveMany(),
    absolute = Path("/A/A2/b[v1]").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("./*[*]", _context).resolveMany(),
    absolute = Path("/A/A2/*[*]").resolveMany();
    compareVectors(relative, absolute, 2);

    QCOMPARE(Path("self:X", _context).resolveMany().size(), 0);
}

void TestPath::testChildren() {
    setContext("A1");
    QVector<QObject*> relative, absolute;

    relative = Path("children:a[v1]", _context).resolveMany(),
    absolute = Path("/A/A1/a[v1]").resolveMany();
    compareVectors(relative, absolute, 2);

    QVERIFY(Path("children:x[v1]", _context).resolveMany().isEmpty());
}

void TestPath::testParent() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    try {
        relative = Path("../*", _context).resolveMany(),
        absolute = Path("/A/*").resolveMany();
        compareVectors(relative, absolute, 3);

        relative = Path("..[v3]", _context).resolveMany(),
        absolute = Path("/A[v3]").resolveMany();
        compareVectors(relative, absolute, 1);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }

    QVERIFY(Path("parent:X", _context).resolveMany().isEmpty());
}

void TestPath::testNearest() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    try {
        relative = Path(".../c", _context).resolveMany(),
        absolute = Path("/A/A2/c").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path(".../A2", _context).resolveMany(),
        absolute = Path("/A/A2").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path(".../A1", _context).resolveMany(),
        absolute = Path("/A/A1").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path(".../c[v2]", _context).resolveMany(),
        absolute = Path("/A/A2/c[v2]").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path("...[v2]", _context).resolveMany(),
        absolute = Path("/A/A2[v2]").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path("...[v3]", _context).resolveMany(),
        absolute = Path("/A[v3]").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path(".../*<Port>", _context).resolveMany(),
        absolute = Path("/A/A2[v2]").resolveMany();
        compareVectors(relative, absolute, 1);

        relative = Path(".../*<Box>", _context).resolveMany(),
        absolute = Path("/A/A2/a").resolveMany();
        compareVectors(relative, absolute, 1);
    }
    catch (Exception &ex) {
        QFAIL( qPrintable("Unexpected: " + ex.what()) );
    }
}

void TestPath::testDescendants() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("descendants:c[v2]", _context).resolveMany(),
    absolute = Path("/A/A2/c[v2]").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("descendants:*[*]", _context).resolveMany(),
    absolute = Path("/A/A2/*[*]").resolveMany();
    compareVectors(relative, absolute, 2);

    QVERIFY(Path("descendants:x[*]", _context).resolveMany().isEmpty());
}

void TestPath::testAncestors() {
    setContext("A2/c");
    QVector<QObject*> relative, absolute;

    relative = Path("ancestors:A", _context).resolveMany(),
    absolute = Path("/A").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("ancestors:A2", _context).resolveMany(),
    absolute = Path("/A/A2").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("ancestors:*", _context).resolveMany(),
    absolute = Path("/A/A2").resolveMany() << Path("/A").resolveMany();
    compareVectors(relative, absolute, 2);

    relative = Path("ancestors:A[v1]", _context).resolveMany(),
    absolute = Path("/A[v1]").resolveMany();
    compareVectors(relative, absolute, 1);

    QVERIFY(Path("ancestors:A3", _context).resolveMany().isEmpty());
}

void TestPath::testAllSiblings() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("allSiblings:*", _context).resolveMany(),
    absolute = Path("/A/*").resolveMany();
    compareVectors(relative, absolute, 3);

    setContext("/A");
    QVERIFY(Path("allSiblings:*", _context).resolveMany().isEmpty());
}

void TestPath::testOtherSiblings() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("otherSiblings:*", _context).resolveMany(),
    absolute = Path("/A/A1").resolveMany() << Path("/A/A3").resolveMany();
    compareVectors(relative, absolute, 2);

    setContext("/A");
    QVERIFY(Path("otherSiblings:*", _context).resolveMany().isEmpty());
}

void TestPath::testPreceedingSibling() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("preceedingSibling:*", _context).resolveMany(),
    absolute = Path("/A/A1").resolveMany();
    compareVectors(relative, absolute, 1);

    setContext("/A");
    Path path = Path("preceedingSibling:*", _context);
    relative = path.resolveMany();
    QVERIFY(Path("preceedingSibling:*", _context).resolveMany().isEmpty());
}

void TestPath::testFollowingSibling() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("followingSibling:*", _context).resolveMany(),
    absolute = Path("/A/A3").resolveMany();
    compareVectors(relative, absolute, 1);

    setContext("/A");
    QVERIFY(Path("followingSibling:*", _context).resolveMany().isEmpty());
}

void TestPath::testPathList() {
    setContext("A2");
    QStringList relPaths, absPaths;
    relPaths << "A3[v3]" << "A[*]";
    absPaths << "/A/A3[v3]" << "/A[*]";

    QVector<QObject*> relative, absolute;
    relative = Path(relPaths, _context).resolveMany(),
    absolute = Path(absPaths).resolveMany();
    compareVectors(relative, absolute, 3);
}

void TestPath::testGlobal() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("A3[v3]", _context).resolveMany(),
    absolute = Path("/A/A3[v3]").resolveMany();
    compareVectors(relative, absolute, 1);
}

void TestPath::testRoot() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("A", _context).resolveMany(),
    absolute = Path("/").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("A", _context).resolveMany(),
    absolute = Path("/A").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("A[v1]", _context).resolveMany(),
    absolute = Path("/A[v1]").resolveMany();
    compareVectors(relative, absolute, 1);

    relative = Path("A[*]", _context).resolveMany(),
    absolute = Path("/A[*]").resolveMany();
    compareVectors(relative, absolute, 2);
}

void TestPath::testBarePort() {
    setContext("A2");
    QVector<QObject*> relative;

    relative = Path("*<Port>", _context).resolveMany();
    QCOMPARE(relative.size(), 12);
}

void TestPath::testNumberOfMatches() {
    QString path("/A/A2/*[*]");

    bool excepted{false};
    try {
        Path(path).resolveOne(0);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        Path(path).resolveMany();
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(!excepted);
}

void TestPath::testEmpty() {
    QVector<QObject*> none;
    try {
        none = Path("").resolveMany();
        QCOMPARE(none.size(), 0);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(none.size(), 0);
}

void TestPath::testResolveInvalid() {
    bool excepted{false};
    try {
        Path(".[v2]").resolveMany();
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPath::testIndirections() {
    setContext("A2/b");
    QObject *relative, *absolute;

    try {
        relative = Path("../../A1/c[v2]", _context).resolveOne(0);
        absolute = Path("/A/A1/c[v2]").resolveOne(0);
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.what()) << "\n";
        QFAIL("Unexpected exception");
    }

    QCOMPARE(relative, absolute);

}
