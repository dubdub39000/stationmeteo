//
// Created by j.mailly on 25/11/2020.
//

#include "View.h"
#include <QDebug>




View::View() :
        QFrame() {
    fenetre = new QGridLayout();
    mAirspeedGaugetemp = new QcGaugeWidget();
    mAirspeedGaugehumidity = new QcGaugeWidget();
    mAirspeedGaugepressure = new QcGaugeWidget();
    setting = new QPushButton("setting");
    log = new QPushButton("log");
    connection = new QLabel("Database unreachable.\n Please, check the connection of database or your computer with the Network");
    menu = new QToolBar("&menu");
    couleurpres=new QList<QPair<QColor, float>>;
    couleurhum=new QList<QPair<QColor, float>>;
    couleurtemp=new QList<QPair<QColor, float>>;
    couleur = new QList<QPair<QColor, float>>;//tableau des couleurs composant les bandes

    for (int i = 0; i < 3; i++) {
        tabgaugetend.push_back(new QcGaugeWidget);
        tabaiguille.push_back(new QcNeedleItem);
    }
    setLayout(fenetre);
}

View::~View()
{
    delete fenetre;
}

void View::initfenetre() {

/*******************************CREATION DES GAUGES DONT NOUS AURONS BESOIN*********************************/
        initgauge(); //initialisation des gauges supérieur
        inittendance();//initialisation des gauges inférieur
        setting->setObjectName("setting");
        log->setObjectName("log");
        connection->setStyleSheet("color : red");
        menu->addWidget(setting);
        menu->addWidget(log);
        connection->hide();

}

/********* fonction de placement des éléments présents sur l'interface graphique ******/
void View::initgauge() {

/*Création de la fenêtre et mise en place du thème gris */
    this->setStyleSheet("background-color : darkgrey");

    /************************************ TEMPERATURE ******************************************/

/*Placement des gauges et des couleurs*/
    mAirspeedGaugetemp->addArc(55);
    mAirspeedGaugetemp->addBackground(100);
    mAirspeedGaugetemp->addArc(90)->setColor(Qt::black);
    mAirspeedGaugetemp->addDegrees(65)->setValueRange(0,50);//longueur intervalle
    QcColorBand *clrBand = mAirspeedGaugetemp->addColorBand(50);//position de la bande de couleur
    clrBand->setValueRange(0,50);//longueur de la bande sur l'arc
    mAirspeedGaugetemp->addValues(80)->setValueRange(-10,40);//range des temp
    lab = mAirspeedGaugetemp->addLabel(40);
    mAirspeedGaugetemp->addLabel(70)->setText("Temp : °C");
    couleurtemp->push_back(QPair<QColor, float>(Qt::yellow, 10));
    couleurtemp->push_back(QPair<QColor, float>(Qt::green, 40));
    couleurtemp->push_back(QPair<QColor, float>(Qt::red, 50));
    clrBand->setColors(*couleurtemp);

/* creation des aiguilles*/

    lab->setText("0");//valeur du Json a recup
    mAirspeedNeedletemp = mAirspeedGaugetemp->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedletemp->setLabel(lab);
    mAirspeedNeedletemp->setColor(Qt::white);
    mAirspeedNeedletemp->setValueRange(-10,40);
    mAirspeedGaugetemp->addBackground(7);

/********************************************** HUMIDITY ***************************************************/

/*Placement des gauges et des couleurs*/

    mAirspeedGaugehumidity->addArc(55);
    mAirspeedGaugehumidity->addBackground(100);
    mAirspeedGaugehumidity->addArc(90)->setColor(Qt::black);
    mAirspeedGaugehumidity->addDegrees(65)->setValueRange(0,100);//longueur intervalle
    QcColorBand *clrBand1 = mAirspeedGaugehumidity->addColorBand(50);//position de la bande de couleur
    clrBand1->setValueRange(0,100);//longueur de la bande sur l'arc
    mAirspeedGaugehumidity->addValues(80)->setValueRange(0,100);//range des temp
    mAirspeedGaugehumidity->addLabel(70)->setText("Hum : %");
    lab1 = mAirspeedGaugehumidity->addLabel(40);
    couleurhum->push_back(QPair<QColor, float>(Qt::red, 20));
    couleurhum->push_back(QPair<QColor, float>(Qt::yellow, 40));
    couleurhum->push_back(QPair<QColor, float>(Qt::green, 60));
    couleurhum->push_back(QPair<QColor, float>(Qt::yellow, 80));
    couleurhum->push_back(QPair<QColor, float>(Qt::red, 100));
    clrBand1->setColors(*couleurhum);

/* creation des aiguilles*/
    lab1->text().toStdString().substr( 0,5);
    lab1->setText("0");//valeur du Json a recup
    mAirspeedNeedlehum = mAirspeedGaugehumidity->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedlehum->setLabel(lab1);
    mAirspeedNeedlehum->setColor(Qt::white);
    mAirspeedNeedlehum->setValueRange(0,100);
    mAirspeedGaugehumidity->addBackground(7);


/*************************************** PRESSURE ***********************************************/

/*Placement des gauges et des couleurs*/

    mAirspeedGaugepressure->addArc(55);
    mAirspeedGaugepressure->addBackground(100);
    mAirspeedGaugepressure->addArc(95)->setColor(Qt::black);
    mAirspeedGaugepressure->addDegrees(65)->setValueRange(0,100);//longueur intervalle
    QcColorBand *clrBand2 = mAirspeedGaugepressure->addColorBand(50);//position de la bande de couleur
    clrBand2->setValueRange(0,100);//longueur de la bande sur l'arc
    mAirspeedGaugepressure->addValues(80)->setValueRange(960,1060);//range des temp
    mAirspeedGaugepressure->addLabel(70)->setText("Press : Hpa");
    lab2 = mAirspeedGaugepressure->addLabel(40);
    lab2->setText("0");//valeur du Json a recup


    couleurpres->push_back(QPair<QColor, float>(Qt::red,40));
    couleurpres->push_back(QPair<QColor, float>(Qt::yellow, 60));
    couleurpres->push_back(QPair<QColor, float>(Qt::green, 100));
    clrBand2->setColors(*couleurpres);

/* creation des aiguilles*/

    mAirspeedNeedlepres = mAirspeedGaugepressure->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedlepres->setLabel(lab2);
    mAirspeedNeedlepres->setColor(Qt::white);
    mAirspeedNeedlepres->setValueRange(960,1060);
    mAirspeedGaugepressure->addBackground(7);

}


/**********Getter des gauges, des aiguilles et de l'affichage des valeurs***************/
QcGaugeWidget *View::getMAirspeedGaugetemp() const {
    return mAirspeedGaugetemp;
}

QcGaugeWidget *View::getMAirspeedGaugehumidity() const {
    return mAirspeedGaugehumidity;
}

QcGaugeWidget *View::getMAirspeedGaugepressure() const {
    return mAirspeedGaugepressure;
}

QcNeedleItem *View::getMAirspeedNeedletemp() const {
    return mAirspeedNeedletemp;
}

QcNeedleItem *View::getMAirspeedNeedlehum() const {
    return mAirspeedNeedlehum;
}

QcNeedleItem *View::getMAirspeedNeedlepres() const {
    return mAirspeedNeedlepres;
}

QcLabelItem *View::getLab() const {
    return lab;
}

QcLabelItem *View::getLab1() const {
    return lab1;
}

QcLabelItem *View::getLab2() const {
    return lab2;
}

void View::inittendance() {
    couleur->push_back(QPair<QColor, float>(Qt::red, 50));
    couleur->push_back(QPair<QColor, float>(Qt::green, 100));

    for (int i = 0; i < 3; i++) {//la boucle permet la création des trois gauges

        tabgaugetend[i]->addArc(55);
        qDebug() << i;
        tabgaugetend[i]->addBackground(100);
        tabgaugetend[i]->addArc(90)->setColor(Qt::black);
        tabgaugetend[i]->addDegrees(65)->setValueRange(0, 100);//longueur intervalle
        QcColorBand *clrBandtend = tabgaugetend[i]->addColorBand(50);//position de la bande de couleur
        clrBandtend->setValueRange(0,100);

        //////////////////////////////////////////////////////////////////

        clrBandtend->setColors(*couleur);
        tabaiguille.push_back(&needletend);//tableau contenant les aiguilles
        tabaiguille[i] = tabgaugetend[i]->addNeedle(60);//taille de l'aiguille
        tabaiguille[i]->setColor(Qt::white);
        tabaiguille[i]->setValueRange(-100, 100);//range permettant d'avoir des valeurs de tendances négatives
        tabaiguille[i]->setCurrentValue(0);//place l'aiguille au milieu par défaut
        tabgaugetend[i]->addBackground(7);

        /////////////////////////////////////////////////////////////////
        if (i == 0)
        {
            tabgaugetend[i]->addLabel(70)->setText("Tendance C°");

        } else if (i == 1)
        {
            tabgaugetend[i]->addLabel(70)->setText("Tendance Hpa");

        }
        else if (i == 2){
            tabgaugetend[i]->addLabel(70)->setText("Tendance %");
        }
        fleche.push_back(tabgaugetend[i]->addLabel(40));
        fleche[i]->setText((QString) 61);

    }
}

const QVector<QcNeedleItem *> &View::getTabaiguille() const {
    return tabaiguille;
}


const QVector<QcGaugeWidget *> &View::getTabgaugetend() const {
    return tabgaugetend;
}

QPushButton *View::getSetting() const {
    return setting;
}

void View::MAJcolor(int nbr) {
    switch (nbr) {
        case 1:
            this->setStyleSheet("background-color : darkgrey;");
            mAirspeedNeedletemp->setColor(Qt::white);
            mAirspeedNeedlepres->setColor(Qt::white);
            mAirspeedNeedlehum->setColor(Qt::white);
            for (int i = 0; i < 3; ++i) {
                tabaiguille[i]->setColor(Qt::white);
            }
            break;
        case 2:
            this->setStyleSheet("background-color : white;");
            mAirspeedNeedletemp->setColor(Qt::black);
            mAirspeedNeedlepres->setColor(Qt::black);
            mAirspeedNeedlehum->setColor(Qt::black);
            for (int i = 0; i < 3; ++i) {
                tabaiguille[i]->setColor(Qt::black);
            }
            break;
    }
    this->repaint();
}

void View::connexion(int a) {
    if (a == 1){
        connection->show();
    } else {
        connection->hide();
    }
}

const QVector<QcLabelItem *> &View::getFleche() const {
    return fleche;
}

QPushButton *View::getLog() const {
    return log;
}

QGridLayout *View::getFenetre() const {
    return fenetre;
}

QToolBar *View::getMenu() const {
    return menu;
}

