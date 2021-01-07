//
// Created by j.mailly on 24/11/2020.
//

#include "Presenter.h"
#include "Jute.h"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QString>


using namespace std;
using namespace jute;

//Test d'ouverture du port
void Presenter::init() {
    serial->setPortName("/dev/ttyACM0"); //definition du port
    serial->setBaudRate(QSerialPort::Baud9600); //definition vitesse de transmission (doit être la meme que sur le arduino)
    serial->setDataBits(QSerialPort::Data8);//codage sur 8 bits
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);//un ou deux bits de stop
    serial->setFlowControl(QSerialPort::NoFlowControl);//pas de controle de flux
    serial->open(QIODevice::ReadWrite);//ouverture du port
    if (serial->isOpen())
    {
        qDebug("le port USB est ouvert");
    } else
        qDebug("Echec de l'ouverture du port");
}

//Constructeur
Presenter::Presenter(){
    serial=new QSerialPort();
    init();
    fenetre = new View;
    fenetre->show();
    setting = new Setting;
    timer=new QTimer();
    timer->start(2000);//la première trame Json est incomplète donc on delay
    dureerefresh = 2000;
    dureetendance = 100;
 /////////////////connnecteur///////////////////
    connect(timer, &QTimer::timeout, this, &Presenter::recupJson);//multitache, permet de pas se faire bloquer en tache de fond
    connect(fenetre->getSetting(), &QPushButton::clicked, this, &Presenter::opensettingview);
    connect(setting->getAnnuler(), &QPushButton::clicked, this, &Presenter::closesettingview);
    connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::MAJparameter);
    connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::closesettingview);
}
Presenter::~Presenter() {
    delete fenetre;
    delete setting;
}

//Récupération des données JSON
void Presenter::recupJson() {
    QString rxBytes;
    rxBytes.append(serial->readAll());
    int end = rxBytes.lastIndexOf("\r\n");
    QStringList cmds = QString(rxBytes.mid(0, end)).split("\r\n", QString::SkipEmptyParts);
    rxBytes = rxBytes.mid(end);//permet de positionner le début la chaine au début de trame suivante
    for (QString cmd:cmds) { //parcours la QstringList
        MAJprm(&cmd);}
    }

//Permet de mettre à jour les valeurs sur l'interface graphique
void Presenter::MAJprm(QString *cmd) {
    if (cmd->mid(0,8).toStdString().find('Temp') && cmd->toStdString().size() >= 51
    && cmd->toStdString().size() <= 55) {//ensemble de conditions pour ne prendre que les trames JSON valide
        qDebug() << *cmd;
        timer->start(dureerefresh);//durée de rafraichissement
        jute::jValue v = jute::parser::parse(cmd->toStdString());//le parser permet de découper la trame
        if (numberofdatatemp < dureetendance) {
            ///////////température//////////////
            Temp = v["Temp"].as_double();
            fenetre->getMAirspeedNeedletemp()->setCurrentValue(Temp);//modifie la valeur de l'aiguille
            fenetre->getLab()->setText(
                    fenetre->getLab()->text().mid(0, fenetre->getLab()->text().toStdString().find('.') + 2));
            fenetre->getMAirspeedGaugetemp()->repaint();
            MAJtendtemp();

            /////////////Pressure//////////////
            Pressure = v["Pressure"].as_double();
            fenetre->getMAirspeedNeedlepres()->setCurrentValue(Pressure);
            fenetre->getLab2()->setText(fenetre->getLab2()->text().mid(0, fenetre->getLab2()->text().toStdString().find(
                    '.') + 3));//permet à l'affichage de ne mettre que deux décimals
            fenetre->getMAirspeedGaugepressure()->repaint();
            MAJtendpress();

            ////////////////Humidity/////////////
            Humidity = v["Humidity"].as_double();
            fenetre->getMAirspeedNeedlehum()->setCurrentValue(Humidity);
            fenetre->getLab1()->setText(
                    fenetre->getLab1()->text().mid(0, fenetre->getLab1()->text().toStdString().find('.') + 3));
            fenetre->getMAirspeedGaugehumidity()->repaint();
            MAJtendhum();
        } else
        {
            rafraichissementtend();
        }
    } else
        qDebug()<<"trame invalide";
}

///////////////////////////calcul des tendances//////////////////////
float Presenter::calcultemp(const float *data) {
    Atemp = Atemp + numberofdatatemp;
    Btemp =Btemp + *data;
    Ctemp = Ctemp + ((*data)*(numberofdatatemp));
    Dtemp =Dtemp + pow(numberofdatatemp,2) ; // en attente de la fonction
    numberofdatatemp++;
/////////calcul final/////////
    float tendance=0;
    tendance = (numberofdatatemp*Ctemp)-(Atemp*Btemp);
    tendance = tendance/((numberofdatatemp*Dtemp)-(pow(Atemp,2)));
    return tendance*100;
}
float Presenter::calculpress(const float *data) {
    Apress = Apress + numeroofdatapress;
    Bpress =Bpress + *data;
    Cpress = Cpress + ((*data)*(numeroofdatapress));
    Dpress =Dpress + pow(numeroofdatapress,2) ; // en attente de la fonction
    numeroofdatapress++;
/////////calcul final/////////
    float tendance=0;
    tendance = (numeroofdatapress*Cpress)-(Apress*Bpress);
    tendance = tendance/((numeroofdatapress*Dpress)-(pow(Apress,2)));
    return tendance;
}

float Presenter::calculhum(const float *data) {
    Ahum = Ahum + numeroofdatahum;
    Bhum =Bhum + *data;
    Chum = Chum + ((*data)*(numeroofdatahum));
    Dhum =Dhum + pow(numeroofdatahum,2) ; // en attente de la fonction
    numeroofdatahum++;
/////////calcul final/////////
    float tendance=0;
    tendance = (numeroofdatahum*Chum)-(Ahum*Bhum);
    tendance = tendance/((numeroofdatahum*Dhum)-(pow(Ahum,2)));
    return tendance*100;
}

////////////////////mise a jour des gauges tendance////////////

void Presenter::MAJtendtemp() {

    float coeftemp = calcultemp(&Temp);//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction
    fenetre->getTabaiguille()[0]->setCurrentValue(coeftemp);//calcul la tendance

    if (coeftemp > 0 ) {
        fenetre->getFlechetemp()->setText((QString) 8593);
        fenetre->getFlechetemp()->setColor(Qt::green);
    }
    else if(coeftemp < 0) {
        fenetre->getFlechetemp()->setText((QString) 8595);
        fenetre->getFlechetemp()->setColor(Qt::red);
    }
    else {
        fenetre->getFlechetemp()->setText((QString) 61);
        fenetre->getFlechetemp()->setColor(Qt::yellow);
    }

    fenetre->getTabgaugetend()[0]->repaint();//indice 0 correspond à l'index du tableau de widget pour la temp



}

void Presenter::MAJtendpress() {
    float coefpress = calculpress(&Pressure);//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction

    fenetre->getTabaiguille()[1]->setCurrentValue(coefpress);//calcul la tendance

    if (coefpress > 0 ) {
        fenetre->getFlechepress()->setText((QString) 8593);
        fenetre->getFlechepress()->setColor(Qt::green);
    }
    else if(coefpress < 0) {
        fenetre->getFlechepress()->setText((QString) 8595);
        fenetre->getFlechepress()->setColor(Qt::red);
    }
    else {
        fenetre->getFlechepress()->setText((QString) 61);
        fenetre->getFlechepress()->setColor(Qt::yellow);
    }

    fenetre->getTabgaugetend()[1]->repaint();//indice 0 correspond à l'index du tableau de widget pour la temp
}

void Presenter::MAJtendhum() {
    float coefhum = calculhum(&Humidity);//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction

    fenetre->getTabaiguille()[2]->setCurrentValue(coefhum);//calcul la tendance

    if (coefhum > 0) {
        fenetre->getFlechehum()->setText((QString) 8593);
        fenetre->getFlechehum()->setColor(Qt::green);
    }
    else if(coefhum < 0) {
        fenetre->getFlechehum()->setText((QString) 8595);
        fenetre->getFlechehum()->setColor(Qt::red);
    }
    else {
        fenetre->getFlechehum()->setText((QString) 61);
        fenetre->getFlechehum()->setColor(Qt::yellow);
    }

    fenetre->getTabgaugetend()[2]->repaint();//indice 0 correspond à l'index du tableau de widget pour la temp

}

void Presenter::opensettingview() {
setting->show();
}

void Presenter::MAJparameter() {
dureetendance = setting->getValeurtendance()->text().toInt();
dureerefresh = setting->getValeurrafraichissement()->text().toInt()*1000;
}

void Presenter::closesettingview() {
    setting->hide();
}

void Presenter::rafraichissementtend() {

     Btemp=0;
     Atemp=0;
     Dtemp=0;
     Ctemp=0;
     numberofdatatemp=1;

     Apress=0;
     Bpress=0;
     Cpress=0;
     Dpress=0;
     numeroofdatapress=1;

     Ahum=0;
     Bhum=0;
     Chum=0;
     Dhum=0;
     numeroofdatahum=1;
}








