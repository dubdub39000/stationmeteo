//
// Created by j.mailly on 25/11/2020.
//

#include "View.h"
#include "Presenter.h"




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
        this->setStyleSheet("background-color : white;");
        initgauge(1); //initialisation des gauges supérieur
        inittendance(1);//initialisation des gauges inférieur

    lab1->setText("0");//valeur du Json a recup
    lab->setText("0");//valeur du Json a recup
    lab2->setText("0");//valeur du Json a recup

        setting->setObjectName("setting");
        log->setObjectName("log");
        connection->setStyleSheet("color : red");
        menu->addWidget(setting);
        menu->addWidget(log);
        connection->hide();
    fenetre->addWidget(mAirspeedGaugetemp,1,0);
    fenetre->addWidget(mAirspeedGaugepressure,1,1);
    fenetre->addWidget(mAirspeedGaugehumidity,1,2);
    fenetre->addWidget(tabgaugetend[0],2,0);
    fenetre->addWidget(tabgaugetend[1],2,1);
    fenetre->addWidget(tabgaugetend[2],2,2);
    fenetre->addWidget(menu, 0, 0);

}

/********* fonction de placement des éléments présents sur l'interface graphique ******/
void View::initgauge(int nbr) {


    /************************************ TEMPERATURE ******************************************/

/*Placement des gauges et des couleurs*/
    mAirspeedGaugetemp->addArc(55);
    setbkgrnd(nbr); //mise en place du fond des jauges du haut
    mAirspeedGaugetemp->addArc(90)->setColor(Qt::black);
    mAirspeedGaugetemp->addDegrees(65)->setValueRange(0,50);//longueur intervalle
    QcColorBand *clrBand = mAirspeedGaugetemp->addColorBand(50);//position de la bande de couleur
    clrBand->setValueRange(0,50);//longueur de la bande sur l'arc
    mAirspeedGaugetemp->addValues(80)->setValueRange(-10,40);//range des temp
    lab = mAirspeedGaugetemp->addLabel(40);
    mAirspeedGaugetemp->addLabel(70)->setText("Temp : °C");
    couleurtemp->push_back(QPair<QColor, float>(Qt::yellow, 10));
    couleurtemp->push_back(QPair<QColor, float>(Qt::darkGreen, 40));
    couleurtemp->push_back(QPair<QColor, float>(Qt::red, 50));
    clrBand->setColors(*couleurtemp);

/* creation des aiguilles*/

    //lab->setText("0");//valeur du Json a recup
    mAirspeedNeedletemp = mAirspeedGaugetemp->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedletemp->setLabel(lab);
    mAirspeedNeedletemp->setColor(Qt::black);
    mAirspeedNeedletemp->setValueRange(-10,40);
    //mAirspeedGaugetemp->addBackground(7, Qt::darkRed, Qt::red);

/********************************************** HUMIDITY ***************************************************/

/*Placement des gauges et des couleurs*/

    mAirspeedGaugehumidity->addArc(55);
    mAirspeedGaugehumidity->addArc(90)->setColor(Qt::black);
    mAirspeedGaugehumidity->addDegrees(65)->setValueRange(0,100);//longueur intervalle
    QcColorBand *clrBand1 = mAirspeedGaugehumidity->addColorBand(50);//position de la bande de couleur
    clrBand1->setValueRange(0,100);//longueur de la bande sur l'arc
    mAirspeedGaugehumidity->addValues(80)->setValueRange(0,100);//range des temp
    mAirspeedGaugehumidity->addLabel(70)->setText("Hum : %");
    lab1 = mAirspeedGaugehumidity->addLabel(40);
    couleurhum->push_back(QPair<QColor, float>(Qt::red, 20));
    couleurhum->push_back(QPair<QColor, float>(Qt::yellow, 40));
    couleurhum->push_back(QPair<QColor, float>(Qt::darkGreen, 60));
    couleurhum->push_back(QPair<QColor, float>(Qt::yellow, 80));
    couleurhum->push_back(QPair<QColor, float>(Qt::red, 100));
    clrBand1->setColors(*couleurhum);

/* creation des aiguilles*/
    lab1->text().toStdString().substr( 0,5);
    //lab1->setText("0");//valeur du Json a recup
    mAirspeedNeedlehum = mAirspeedGaugehumidity->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedlehum->setLabel(lab1);
    mAirspeedNeedlehum->setColor(Qt::black);
    mAirspeedNeedlehum->setValueRange(0,100);



/*************************************** PRESSURE ***********************************************/

/*Placement des gauges et des couleurs*/

    mAirspeedGaugepressure->addArc(55);

    mAirspeedGaugepressure->setStyleSheet("background-color : darkgreen;");
    mAirspeedGaugepressure->addArc(95)->setColor(Qt::black);
    mAirspeedGaugepressure->addDegrees(65)->setValueRange(0,100);//longueur intervalle
    QcColorBand *clrBand2 = mAirspeedGaugepressure->addColorBand(50);//position de la bande de couleur
    clrBand2->setValueRange(0,100);//longueur de la bande sur l'arc
    mAirspeedGaugepressure->addValues(80)->setValueRange(960,1060);//range des temp
    mAirspeedGaugepressure->addLabel(70)->setText("Press : Hpa");
    lab2 = mAirspeedGaugepressure->addLabel(40);
    //lab2->setText("0");//valeur du Json a recup


    couleurpres->push_back(QPair<QColor, float>(Qt::red,40));
    couleurpres->push_back(QPair<QColor, float>(Qt::yellow, 60));
    couleurpres->push_back(QPair<QColor, float>(Qt::darkGreen, 100));
    clrBand2->setColors(*couleurpres);

/* creation des aiguilles*/

    mAirspeedNeedlepres = mAirspeedGaugepressure->addNeedle(60);//taille de l'aiguille
    mAirspeedNeedlepres->setLabel(lab2);
    mAirspeedNeedlepres->setColor(Qt::black);
    mAirspeedNeedlepres->setValueRange(960,1060);


}


void View::inittendance(int nbr) {
    couleur->push_back(QPair<QColor, float>(Qt::red, 50));
    couleur->push_back(QPair<QColor, float>(Qt::darkGreen, 100));
fleche.clear(); //si pas appelé, raffraichissement tendance plantouille
    for (int i = 0; i < 3; i++) {//la boucle permet la création des trois gauges
        tabgaugetend[i]->addArc(55);
        if (nbr==1) {
            tabgaugetend[i]->addBackground(100, Qt::cyan, Qt::lightGray);
            tabgaugetend[i]->addBackground(7, Qt::darkRed, Qt::red);

        }
            else {
                tabgaugetend[i]->addBackground(100, Qt::white, Qt::darkCyan);
                tabgaugetend[i]->addBackground(7, Qt::darkRed, Qt::red);
            }

        tabgaugetend[i]->addArc(90)->setColor(Qt::black);
        tabgaugetend[i]->addDegrees(65)->setValueRange(0, 100);//longueur intervalle
        QcColorBand *clrBandtend = tabgaugetend[i]->addColorBand(50);//position de la bande de couleur
        clrBandtend->setValueRange(0,100);

        //////////////////////////////////////////////////////////////////

        clrBandtend->setColors(*couleur);
        tabaiguille.push_back(&needletend);//tableau contenant les aiguilles
        tabaiguille[i] = tabgaugetend[i]->addNeedle(60);//taille de l'aiguille
        tabaiguille[i]->setColor(Qt::black);
        tabaiguille[i]->setValueRange(-100, 100);//range permettant d'avoir des valeurs de tendances négatives
        tabaiguille[i]->setCurrentValue(0);//place l'aiguille au milieu par défaut

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


void View::setbkgrnd(int nbr) {

    switch (nbr) {
        case 1: //thème darkgrey
            mAirspeedGaugetemp->addBackground(100, Qt::cyan, Qt::lightGray);
            mAirspeedGaugetemp->addBackground(7, Qt::darkRed, Qt::red);
            mAirspeedGaugehumidity->addBackground(100, Qt::cyan, Qt::lightGray);
            mAirspeedGaugehumidity->addBackground(7, Qt::darkRed, Qt::red);
            mAirspeedGaugepressure->addBackground(100, Qt::cyan, Qt::lightGray);
            mAirspeedGaugepressure->addBackground(7, Qt::darkRed, Qt::red);

            break;

       /* case 2: //thème white
            mAirspeedGaugetemp->addBackground(100, Qt::cyan, Qt::darkGray);
            mAirspeedGaugetemp->addBackground(7, Qt::black, Qt::white);
            mAirspeedGaugehumidity->addBackground(100, Qt::cyan, Qt::darkGray);
            mAirspeedGaugehumidity->addBackground(7, Qt::black, Qt::white);
            mAirspeedGaugepressure->addBackground(100, Qt::cyan, Qt::darkGray);
            mAirspeedGaugepressure->addBackground(7, Qt::black, Qt::white);

            break;
*/
        case 2: //thème black
            mAirspeedGaugetemp->addBackground(100, Qt::white, Qt::darkCyan);
            mAirspeedGaugetemp->addBackground(7, Qt::white, Qt::blue);
            mAirspeedGaugehumidity->addBackground(100, Qt::white, Qt::darkCyan);
            mAirspeedGaugehumidity->addBackground(7, Qt::white, Qt::blue);
            mAirspeedGaugepressure->addBackground(100, Qt::white, Qt::darkCyan);
            mAirspeedGaugepressure->addBackground(7, Qt::white, Qt::blue);

            break;
    }
}
void View::MAJcolor(int nbr) {
    switch (nbr) {
        case 1:
            this->setStyleSheet("background-color : white;");
            initgauge(1);
            inittendance(1);
            for (int i = 0; i < 3; ++i) {
                tabaiguille[i]->setColor(Qt::black);
            }
            this->repaint();
            break;

        case 2:
            this->setStyleSheet("background-color : black;");
            initgauge(2);
            inittendance(2);
            log->setStyleSheet("background-color : darkGray;");
            setting->setStyleSheet("background-color : darkGray;");
            for (int i = 0; i < 3; ++i) {
                tabaiguille[i]->setColor(Qt::black);
            }
            this->repaint();
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


const QVector<QcLabelItem *> &View::getFleche() const {
    return fleche;
}

QPushButton *View::getLog() const {
    return log;
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