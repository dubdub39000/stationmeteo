//
// Created by j.mailly on 24/11/2020.
//

#ifndef STATIONMETEO_PRESENTER_H
#define STATIONMETEO_PRESENTER_H


#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QTimer>
#include "View.h"

class Presenter : public QObject {
Q_OBJECT
private:
    QSerialPort *serial;
    QTimer *timer;
    View *fenetre;
    float *Temp;
    float *Pressure;
    float *Humidity;
public:
    Presenter();
    void init();
    void recupJson();
    void MAJprm(QString *cmd);//met a jour les valeurs des capteurs

};


#endif //STATIONMETEO_PRESENTER_H
