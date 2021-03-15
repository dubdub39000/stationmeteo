#include <iostream>
#include <QtWidgets/QApplication>
#include "Presenter.h"


int main(int argc, char  *argv[]) {
    QApplication *app= new QApplication(argc, argv);
    Presenter *presenter=new Presenter(app);
    return app->exec();
}
