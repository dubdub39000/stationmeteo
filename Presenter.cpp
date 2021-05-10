//
// Created by j.mailly on 24/11/2020.
//

#include "Presenter.h"
#include "Jute.h"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>


using namespace std;
using namespace jute;

//Constructeur
Presenter::Presenter() {
    fenetre = new View();
    setting = new Setting();
    log = new Logview();
    /////////////////la fenetre principale////////
    QFuture<void> thread1 = QtConcurrent::run(fenetre, &View::initfenetre);
    thread1.waitForFinished();
    MAJLOG(2, new QString("initialisation de la fenêtre principale"));
///////////////////////la fenetre log////////////////////////////////
    QFuture<void> thread2 = QtConcurrent::run(log , &Logview::initlog);
    thread2.waitForFinished();
    MAJLOG(2, new QString("initialisation de la fenêtre de log"));
////////////////////la fenetre setting/////////////////////////////
    QFuture<void> thread3 = QtConcurrent::run(setting, &Setting::inittsetting);
    thread3.waitForFinished();
    MAJLOG(2, new QString("initialisation de la fenêtre de setting"));
////////////////////initialization des valeurs avec le fichier ini////////////////////
    QSettings fichier(QApplication::applicationDirPath() + "/conf.ini", QSettings::IniFormat);//ouverture du fichier de conf
    if(!QFile::exists(QApplication::applicationDirPath() + "/conf.ini"))
    {
        qDebug() << "pas de fichier de conf présent";
        MAJLOG(2, new QString("fichier init introuvable"));
    }
    qDebug() << fichier.allKeys();
dureerefresh = fichier.value("Duree/rafraichissement","1000").toInt();
dureetendance = fichier.value("Duree/tendances","50").toInt();
ipserveur = fichier.value("Ipserveur/ipadresse", "0.0.0.0").toString();

timerinit(); //initialize le timerjson;
/////////////////////////////////////////
    fenetre->show();
    tabtemp = new QVector<float>;
    tabpress = new QVector<float>;
    tabhum = new QVector<float>;
    inittab();
        /////////////////connnecteur///////////////////
        connect(timerjson, &QTimer::timeout, this, &Presenter::TestConnection);//a chaque timeout du timer (2s) il relance une requête
        connect(fenetre->getSetting(), &QPushButton::clicked, this, &Presenter::opensettingview);
        connect(setting->getAnnuler(), &QPushButton::clicked, this, &Presenter::closbyannulersetting);
        connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::MAJparameter);
        connect(setting->getValider(), &QPushButton::clicked, this, &Presenter::closebyvalidersetting);
        connect(fenetre->getLog(), &QPushButton::clicked, this, &Presenter::openlog);
        connect(log->getFermer(), &QPushButton::clicked, this, &Presenter::closelog);
        connect(log->getClearflow(), &QPushButton::clicked, this, [this] { clear(1); });//syntaxe permettant de passer un argument
        connect(log->getClearlogsys(), &QPushButton::clicked, this, [this] { clear(2); });
}

Presenter::~Presenter() {
    delete fenetre;
    delete setting;
    delete tabtemp;
    delete tabhum;
    delete tabpress;
    delete  log;
}

/////////////Récupération des données JSON/////////////////////////

void Presenter::TestConnection() {
    manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl("http://" + (ipserveur) +"/meteo/read.php")));
    connect(manager, &QNetworkAccessManager::finished, this, &Presenter::errorconnection);
    }

void Presenter::errorconnection(QNetworkReply *networkReply) {
    try {
        if (networkReply->error() != QNetworkReply::NoError) {
            throw overflow_error("cannot connect with database");
        }
        fenetre->connexion(2); //hide la fenetre d'erreur
        recupJson(networkReply);
        MAJLOG(2, new QString("request successfull"));
    } catch (overflow_error &cl) {
        MAJLOG(2, new QString(cl.what()));
        fenetre->connexion(1);//show la fenetre d'erreur
    }
}

void Presenter::recupJson(QNetworkReply *qNetworkReply) {
    QString answer =qNetworkReply->readAll();
        trameJson(&answer);
        qNetworkReply->deleteLater();
}

void Presenter::trameJson(QString *cmd) {
    jute::jValue v;
    try { //début code sous exception
        if (cmd->mid(0, 8).toStdString().find('temp') )
             {//conditions pour ne prendre que les trames JSON valide
            MAJLOG(1,cmd);//affiche la trame reçue dans la fenetre de log
            v = jute::parser::parse(cmd->toStdString());//le parser permet de découper la trame
        }
        else {
            throw overflow_error("trame invalide");
        }
    } catch (overflow_error &oe) {
        MAJLOG(1, new QString("trame invalide"));
    }
    MAJprm(v);
}

/**********************************************************
*
 *                 gestion interface graphique
 *
 **********************************************************/
//Permet de mettre à jour les valeurs sur l'interface graphique
void Presenter::MAJprm(jute::jValue v) {
    timerjson->start(dureerefresh);
        if (tabtemp->value(4) < dureetendance) {
            ///////////température//////////////
            Temp = v["temp"].as_double();
            fenetre->getMAirspeedNeedletemp()->setCurrentValue(Temp);//modifie la valeur de l'aiguille
            fenetre->getLab()->setText(fenetre->getLab()->text().mid(0, fenetre->getLab()->text().toStdString().find('.') + 2));
            fenetre->getMAirspeedGaugetemp()->repaint();
            MAJtend(tabtemp, &Temp, 0);
            /////////////Pressure//////////////
            Pressure = v["pressure"].as_double();
            fenetre->getMAirspeedNeedlepres()->setCurrentValue(Pressure);
            fenetre->getLab2()->setText(fenetre->getLab2()->text().mid(0, fenetre->getLab2()->text().toStdString().find('.') + 3));//permet à l'affichage de ne mettre que deux décimals
            fenetre->getMAirspeedGaugepressure()->repaint();
            MAJtend(tabpress, &Pressure, 1);
            ////////////////Humidity/////////////
            Humidity = v["humidity"].as_double();
            fenetre->getMAirspeedNeedlehum()->setCurrentValue(Humidity);
            fenetre->getLab1()->setText(fenetre->getLab1()->text().mid(0, fenetre->getLab1()->text().toStdString().find('.') + 3));
            fenetre->getMAirspeedGaugehumidity()->repaint();
            MAJtend(tabhum, &Humidity,2);
        } else
        {
            rafraichissementtend();
        }
}

////////////////////mise a jour des gauges tendance////////////

void Presenter::MAJtend(QVector<float> *tabtend,float *donnee, int index) {
    float  coef = calcultendance(tabtend, donnee);//obligé car le temps de traitement de l'opération est trop long pour répéter l'appel de la fonction

    if (index == 0 || index == 2){
        coef = coef*10;
    }
    fenetre->getTabaiguille()[index]->setCurrentValue(coef);//calcul la tendance

    if (coef > 0) {
        fenetre->getFleche()[index]->setText((QString) 8593);
        fenetre->getFleche()[index]->setColor(Qt::green);
    }
    else if(coef < 0) {
        fenetre->getFleche()[index]->setText((QString) 8595);
        fenetre->getFleche()[index]->setColor(Qt::red);
    }
    else {
        fenetre->getFleche()[index]->setText((QString) 61);
        fenetre->getFleche()[index]->setColor(Qt::yellow);
    }

    fenetre->getTabgaugetend()[index]->repaint();//indice 0 correspond à l'index du tableau de widget pour la temp

}


///////////////////////////calcul des tendances//////////////////////
float Presenter::calcultendance(QVector <float> *tab, float *donnee) {
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
/**********************************************************************************
 *
 *                     gestion de la fenêtre setting
 *
 * **********************************************************************************/
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
    }
        ///////////////////paramètre refresh///////////////////
    else if (temp2 < 1) {
            setting->affichageerreur(2);
            keepwindows = false;
        }
    ////////////////si pas d'erreur de saisie///////////////////
    else {
        dureetendance = temp1;
        dureerefresh = temp2;
        keepwindows = true;// on remet a 1 pour pouvoir revalider arpès une erreur
    }
    ///////////////////paramètre coulor////////////////////
    if (setting->getChoixcouleur()->currentText() == "light mode"){
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
    MAJLOG(2,new QString("Remise à zéro des tableaux de tendances enclenchée"));
    tabtemp->clear();
    tabhum->clear();
    tabpress->clear();
    inittab();
}

/*******************************************************************
 *
 *                      gestion de la fentre log
 *
*********************************************************************/

void Presenter::openlog() {
log->show();
}

void Presenter::closelog() {
    log->hide();
}

void Presenter::clear(int nbr) {
    switch (nbr) {
        case 1:
            log->getZonetrame()->clear();
            break;
        case 2:
            log->getZonesystem()->clear();
            break;
    }
}

void Presenter::MAJLOG(int nbr1, QString *message) {
    //////////////envoi dans la zone corespondante//////////
    switch (nbr1) {
        case 1:
            log->getZonetrame()->insertPlainText(*message);
            log->getZonetrame()->insertPlainText("\n");
            break;
        case 2:
            log->getZonesystem()->insertPlainText(*message);
            log->getZonesystem()->insertPlainText("\n");
            break;
    }
}

void Presenter::timerinit() {
    timerjson=new QTimer();
    timerjson->start(2000);//durée de rafraichissement
}
