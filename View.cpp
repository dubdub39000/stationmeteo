//
// Created by j.mailly on 25/11/2020.
//

#include "View.h"


View::View() :
QFrame(){

/*******************************CREATION DES GAUGES DONT NOUS AURONS BESOIN*********************************/
    mAirspeedGaugetemp = new QcGaugeWidget;
    mAirspeedGaugehumidity = new QcGaugeWidget;
    mAirspeedGaugepressure = new QcGaugeWidget;
    initgauge(); //initialisation des gauges supérieur
    inittendance();//initialisation des gauges inférieur
    setLayout(fenetre);
}

View::~View()
{
    delete fenetre;
}

/********* fonction de placement des éléments présents sur l'interface graphique ******/
void View::initgauge() {

/*Création de la fenêtre et mise en place du thème gris */
    fenetre = new QGridLayout();
    this->setStyleSheet("background-color : grey;");
    QList<QPair<QColor, float>> *tabcouleur=new QList<QPair<QColor, float>>;


    /************************************ TEMPERATURE ******************************************/

QList<QPair<QColor, float>> *couleurtemp=new QList<QPair<QColor, float>>;

/*Placement des gauges et des couleurs*/
    mAirspeedGaugetemp->addArc(55);
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

    /*On ajoute la gauges à la fenêtre*/

    fenetre->addWidget(mAirspeedGaugetemp,0,0);//position de la gauge dans la fenetre


/********************************************** HUMIDITY ***************************************************/


    QList<QPair<QColor, float>> *couleurhum=new QList<QPair<QColor, float>>;

/*Placement des gauges et des couleurs*/

    mAirspeedGaugehumidity->addArc(55);
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


    /*On ajoute la gauges à la fenêtre*/

    fenetre->addWidget(mAirspeedGaugehumidity,0,2);

/*************************************** PRESSURE ***********************************************/

    QList<QPair<QColor, float>> *couleurpres=new QList<QPair<QColor, float>>;

/*Placement des gauges et des couleurs*/

    mAirspeedGaugepressure->addArc(55);
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


/*On ajoute la gauges à la fenêtre*/
    fenetre->addWidget(mAirspeedGaugepressure,0,1);


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
    for (int i = 0; i < 3; i++) {//la boucle permet la créetion des trois gauges
        tabgaugetend.push_back(new QcGaugeWidget);//gauge tendance temperature
        //tabcolor.push_back(new QcColorBand);//tableau des bandes de couleur
        auto *couleur = new QList<QPair<QColor, float>>;//tableau des couleurs composant les bandes
        tabaiguille.push_back(new QcNeedleItem);//tableau contenant les aiguilles

        tabgaugetend[i]->addArc(55);
        tabgaugetend[i]->addDegrees(65)->setValueRange(0, 100);//longueur intervalle
        QcColorBand *clrBandtend = tabgaugetend[i]->addColorBand(50);//position de la bande de couleur
        clrBandtend->setValueRange(0,100);

        //////////////////////////////////////////////////////////////////

        fleche = tabgaugetend[i]->addLabel(40);
        fleche->setText((QString) 61);//fleche bas 8595, fleche haut 8593
        fleche->setColor(Qt::yellow);
        couleur->push_back(QPair<QColor, float>(Qt::red, 50));
        couleur->push_back(QPair<QColor, float>(Qt::green, 100));
        clrBandtend->setColors(*couleur);

        tabaiguille[i] = tabgaugetend[i]->addNeedle(60);//taille de l'aiguille
        tabaiguille[i]->setColor(Qt::white);
        tabaiguille[i]->setValueRange(0, 100);
        tabgaugetend[i]->addBackground(7);

        /////////////////////////////////////////////////////////////////

        if (i == 0)
        {
            tabgaugetend[i]->addLabel(70)->setText("Tendance °C");
        }
        if (i == 1) {
            tabgaugetend[i]->addLabel(70)->setText("Tendance Hpa");
        }
        if (i == 2) {
            tabgaugetend[i]->addLabel(70)->setText("Tendance %");
        }
        fenetre->addWidget(tabgaugetend[i], 1, 0+i);
    }
}
