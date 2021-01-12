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
#include "Jute.h"
#include "Logview.h"
#include <chrono>

class Presenter : public QObject {
Q_OBJECT
private:
    QSerialPort *serial;
    QTimer *timer;
    View *fenetre;
    Setting *setting;
    Logview *log;
    float Temp;
    float Pressure;
    float Humidity;
    int dureetendance;//nombre max de trame traitéé
    int dureerefresh;//durée du timer entre deux traitements

    QVector<float> *tabtemp;
    QVector<float> *tabpress;
    QVector<float> *tabhum;

    bool keepwindows;//  permet de maintenir la fenetre active si erreur de saisie
public:
    Presenter();
    virtual ~Presenter();

    void init();
    void recupJson();
    void MAJprm(jute::jValue v);//met a jour les valeurs des capteurs
    ////////////////gestion de la trame JSON avec exception//////////////////

    void trameJson(QString *cmd); //methode pour éliminer les trame invalide

    ///////////////gestion fenêtre view///////////////////////

    void MAJtend(QVector<float> *tabtend,float *donnee, int index);

    ///////////////gestion fenêtre setting/////////////////
    void opensettingview();// permet d'afficher la fenetre des setting, elle s'initialise au lancement de l'appli
    void closebyvalidersetting();//on cache la fenêtre en cas de validation
    void closbyannulersetting();//on ferme la fenêtre setting
    void MAJparameter();// permet de prendre les valeurs de setting et les inclure dans view
    void rafraichissementtend();// remet à zero l'ensemble des variables de calcul de tendance

   ////////////////calcul tendance/////////////////////
   void inittab(); //initialize les tableau dynamique
    float calcultendance(QVector<float> *tab, float *donnee);

    ///////////////gestion de la fentre log///////////:
    void openlog();
    void closelog();
    void MAJLOG(int nbr1, QString *message); //affiche les messages d'erreurs dans la fenêtre
    void clear(int nbr); //methode clear des messages
};

#endif //STATIONMETEO_PRESENTER_H
