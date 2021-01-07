//
// Created by j.mailly on 24/11/2020.
//

#ifndef STATIONMETEO_PRESENTER_H
#define STATIONMETEO_PRESENTER_H


#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QTimer>
#include "View.h"
#include "Setting.h"
#include <chrono>

class Presenter : public QObject {
Q_OBJECT
private:
    QSerialPort *serial;
    QTimer *timer;
    View *fenetre;
    Setting *setting;
    float Temp;
    float Pressure;
    float Humidity;

    float Btemp; // somme des data reçu pour la temperature
    float Atemp; //somme des indexs de la temperature
    float Dtemp; //somme du carré des index de la temperature
    float Ctemp; // somme du produit de data et de son index de la temperature
    float numberofdatatemp=1; //nombre de data recu pour la temperature

    float Apress;
    float Bpress;
    float Cpress;
    float Dpress;
    float numeroofdatapress=1;

    float Ahum;
    float Bhum;
    float Chum;
    float Dhum;
    float numeroofdatahum=1;

    int dureetendance;//nombre max de trame traitéé
    int dureerefresh;//durée du timer entre deux traitements


public:
    Presenter();
    virtual ~Presenter();

    void init();
    void recupJson();
    void MAJprm(QString *cmd);//met a jour les valeurs des capteurs
    float calcultemp(const float *data);
    float calculpress(const float *data);
    float calculhum(const float *data);

    ///////////////gestion fenêtre view///////////////////////

    void MAJtendtemp(); //met  à jour la gauge tendance temp
    void MAJtendpress(); //met  à jour la gauge tendance pression
    void MAJtendhum(); //met  à jour la gauge tendance humidité

    ///////////////gestion fenêtre setting/////////////////
    void opensettingview();// permet d'afficher la fenetre des setting, elle s'initialise au lancement de l'appli
    void closesettingview();
    void MAJparameter();// permet de prendre les valeurs de setting et les inclure dans view
    void rafraichissementtend();// remet à zero l'ensemble des variables de calcul de tendance
};

#endif //STATIONMETEO_PRESENTER_H
