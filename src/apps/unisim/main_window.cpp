#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QtWidgets>
#include <base/dialog.h>
#include <base/version.h>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget( _dialog = new base::Dialog(this) );
    setWindowTitle("Universal Simulator " + base::version());
    restore();
}


void MainWindow::restore() {
    // Get screen dimensions
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->availableGeometry(this);
    int W = screen.width(),
        H = screen.height();
    // Get latest screen geometry
    QSettings settings;
    QPoint position = settings.value("main_window_geometry/position", QPoint(W/3, H/3)).toPoint();
    QSize size = settings.value("main_window_geometry/size", QSize(W/3, H/3)).toSize();
    // Use restore window if geometry is OK
    bool badGeometry = position.isNull() || size.isNull(),
         tooBig = size.width() > W || size.height() > H,
         outside = position.x() + size.width() > W || position.y() + size.height() > H;
    if (!(badGeometry || tooBig || outside)) {
        move(position);
        resize(size);
    }
}

MainWindow::~MainWindow() {
    QSettings settings;
    settings.setValue("main_window_geometry/position", pos());
    settings.setValue("main_window_geometry/size", size());
}
