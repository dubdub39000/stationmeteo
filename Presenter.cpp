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
    timer->start(1000);//la première trame Json est incomplète donc on delay
    dureerefresh = 2000;
    dureetendance = 100;
    tabtemp = new QVector<float>;
    tabpress = new QVector<float>;
    tabhum = new QVector<float>;
    inittab();

    /////////////////connnecteur///////////////////
    connect(timer, &QTimer::timeout, this, &Presenter::recupJson);//multitache, permet de pas se faire bloquer en tache de fond
    connect(fenetre->getSetting(), &QPushButton::clicked, this, &Presenter::opensettingview);
    connect(setting->getAnnuler(), &QPushButton::clicked, this, &Presenter::closbyannulersetting);
    connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::MAJparameter);
    connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::closebyvalidersetting);
}
Presenter::~Presenter() {
    delete fenetre;
    delete setting;
    delete tabtemp;
    delete tabhum;
    delete tabpress;
}

//Récupération des données JSON
void Presenter::recupJson() {
    QString rxBytes;
    rxBytes.append(serial->readAll());
    int end = rxBytes.lastIndexOf("\r\n");
    QStringList cmds = QString(rxBytes.mid(0, end)).split("\r\n", QString::SkipEmptyParts);
    rxBytes = rxBytes.mid(end);//permet de positionner le début la chaine au début de trame suivante
    for (QString cmd:cmds) { //parcours la QstringList
        trameJson(&cmd);
    }
}

void Presenter::trameJson(QString *cmd) {
    jute::jValue v;
    try { //début code sous exception
        if (cmd->mid(0, 8).toStdString().find('Temp') && cmd->toStdString().size() >= 51
            && cmd->toStdString().size() <= 55) {//ensemble de conditions pour ne prendre que les trames JSON valide
            qDebug() << *cmd;
            timer->start(dureerefresh);//durée de rafraichissement
           v  = jute::parser::parse(cmd->toStdString());//le parser permet de découper la trame
        }
    } catch (overflow_error) {
    }
    MAJprm(v);
}
//Permet de mettre à jour les valeurs sur l'interface graphique
void Presenter::MAJprm(jute::jValue v) {

        if (tabtemp->value(4) < dureetendance) {
            ///////////température//////////////
            Temp = v["Temp"].as_double();
            fenetre->getMAirspeedNeedletemp()->setCurrentValue(Temp);//modifie la valeur de l'aiguille
            fenetre->getLab()->setText(fenetre->getLab()->text().mid(0, fenetre->getLab()->text().toStdString().find('.') + 2));
            fenetre->getMAirspeedGaugetemp()->repaint();
            MAJtendtemp();

            /////////////Pressure//////////////
            Pressure = v["Pressure"].as_double();
            fenetre->getMAirspeedNeedlepres()->setCurrentValue(Pressure);
            fenetre->getLab2()->setText(fenetre->getLab2()->text().mid(0, fenetre->getLab2()->text().toStdString().find('.') + 3));//permet à l'affichage de ne mettre que deux décimals
            fenetre->getMAirspeedGaugepressure()->repaint();
            MAJtendpress();

            ////////////////Humidity/////////////
            Humidity = v["Humidity"].as_double();
            fenetre->getMAirspeedNeedlehum()->setCurrentValue(Humidity);
            fenetre->getLab1()->setText(fenetre->getLab1()->text().mid(0, fenetre->getLab1()->text().toStdString().find('.') + 3));
            fenetre->getMAirspeedGaugehumidity()->repaint();
            MAJtendhum();
        } else
        {
            rafraichissementtend();
        }
}

////////////////////mise a jour des gauges tendance////////////

void Presenter::MAJtendtemp() {

    float coeftemp = calcultendance(tabtemp, &Temp)*10;//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction
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
    float coefpress = calcultendance(tabpress, &Pressure);//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction

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
    float  coefhum = calcultendance(tabhum, &Humidity)*10;//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction

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
///////////////////////////calcul des tendances//////////////////////
float Presenter::calcultendance(QVector <float> *tab, float *donnee) {
    //qDebug()<<"test";
    float A; //somme des index
    float B; // somme des data reçues
    float C; // somme du produit de la data et de l'index
    float D; //somme des index au carré
    float nbr; //nombre de data reçue
    float tendance=0;
    ////////////////////detail du calcul//////////
    nbr = tab->value(4);//nombre de trame traité
    A = tab->value(0) + nbr;
    tab->replace(0, A);

    B = tab->value(1) + *donnee;//somme des data reçues
    tab->replace(1, B);

    C = C + ((*donnee)*(nbr));//somme du produit de l'index et de la data
    tab->replace(2, C);

    D = D + pow(nbr,2) ; // somme des index au carré
    tab->replace(3, D);
    tab->replace(4,tab->value(4)+1);

/////////calcul final/////////

    tendance = (nbr*C)-(A*B);
    tendance = tendance/((nbr*D)-(pow(A,2)));
    return tendance;

}
//////////////////////////////////////////////////////////////
void Presenter::inittab() {
    for (int i = 0; i < 4; ++i) {
        tabtemp->push_back(0);
        tabpress->push_back(0);
        tabhum->push_back(0);
    }
    tabtemp->push_back(1);
    tabpress->push_back(1);
    tabhum->push_back(1);
}
//////////////////////////////gestion de la fenêtre setting///////////////////////////
void Presenter::opensettingview() {
    setting->show();
}

void Presenter::MAJparameter() {
    int temp1;
    int temp2;
    temp1 = setting->getValeurtendance()->text().toInt();
    temp2 = setting->getValeurrafraichissement()->text().toInt() * 1000;//pour mettre en seconde
    /////////////////////paramètre tendance/////////////////
    if (temp1 < 10) {
        setting->affichageerreur(1);
        keepwindows = false;
        ///////////////////paramètre refresh///////////////////
        if (temp2 <= 0) {
            setting->affichageerreur(2);
            keepwindows = false;
        }
    }
    ////////////////si pas d'erreur de saisie///////////////////
    else {
        dureetendance = temp1;
        dureerefresh = temp2;
        keepwindows = true;// on remet a 1 pour pouvoir revalider arpès une erreur
    }
    qDebug()<<temp1;
    qDebug()<<temp2;
    ///////////////////paramètre coulor////////////////////
    if (setting->getChoixcouleur()->currentText() == "grey"){
        fenetre->MAJcolor(1);
    } else{
        fenetre->MAJcolor(2);
    }
}
///////////////////////////////gestion de sa fermeture////////////////
void Presenter::closebyvalidersetting() {
    if (keepwindows){  //on garde la fenêtre ouverte si les valeurs sont incorrectes
        setting->MAJsetting(); //supprime le message d'erreur de la fenetre setting
    setting->hide();}
}

void Presenter::closbyannulersetting() {
    setting->MAJsetting();
setting->hide();
}
/////////////////////////////////////////reinit des valeurs//////////////////////
void Presenter::rafraichissementtend() {//on clear les tableaux et on réinit
    qDebug()<<"Rafraichissement enclenché";
    tabtemp->clear();
    tabhum->clear();
    tabpress->clear();
    inittab();
}
