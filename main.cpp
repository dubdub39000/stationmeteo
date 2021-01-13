#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include "Presenter.h"
#include <QDebug>

int main(int argc, char  *argv[]) {
    QApplication app(argc, argv);
    Presenter *presenter=new Presenter();
    return app.exec();
}
