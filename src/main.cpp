#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ae::MainWindow mw;
    mw.show();
    return QApplication::exec();
}
