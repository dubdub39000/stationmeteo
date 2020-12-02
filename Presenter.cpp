//
// Created by j.mailly on 24/11/2020.
//

#include "Presenter.h"
#include "Jute.h"

#include <QDebug>
#include <QTimer>

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
    timer=new QTimer();
    timer->start(2000);//la première trame Json est incomplète donc on delay
    connect(timer, &QTimer::timeout, this, &Presenter::recupJson);//multitache, permet de pas se faire bloquer en tache de fond

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
    if (cmd->mid(0,8).toStdString().find('Humidity') && cmd->toStdString().size() >= 51
    && cmd->toStdString().size() <= 55) {
        qDebug() << *cmd;
        timer->start(2000);//rend le démarrage plus stable
        jute::jValue v = jute::parser::parse(cmd->toStdString());//le parser permet de découper la trame
        ///////////température//////////////
        *Temp = v["Temp"].as_double();
        fenetre->getMAirspeedNeedletemp()->setCurrentValue(*Temp);//modifie la valeur de l'aiguille
        fenetre->getLab()->setText(fenetre->getLab()->text().mid(0,fenetre->getLab()->text().toStdString().find('.')+2));
        fenetre->getMAirspeedGaugetemp()->repaint();

        ////////////////Humidity/////////////
        *Humidity = v["Humidity"].as_double();
        fenetre->getMAirspeedNeedlehum()->setCurrentValue(*Humidity);
        fenetre->getLab1()->setText(fenetre->getLab1()->text().mid(0,fenetre->getLab1()->text().toStdString().find('.')+3));
        fenetre->getMAirspeedGaugehumidity()->repaint();

        /////////////Pressure//////////////
        *Pressure = v["Pressure"].as_double();
        fenetre->getMAirspeedNeedlepres()->setCurrentValue(*Pressure);
        fenetre->getLab2()->setText(fenetre->getLab2()->text().mid(0,fenetre->getLab2()->text().toStdString().find('.')+3));//permet à l'affichage de ne mettre que deux décimals
        fenetre->getMAirspeedGaugepressure()->repaint();

    } else
        qDebug()<<"trame invalide";
}
