//
// Created by j.mailly on 25/11/2020.
//

#ifndef STATIONMETEO_VIEW_H
#define STATIONMETEO_VIEW_H

#include "FenêtreGauge.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>

class View : public QFrame
{
    Q_OBJECT

public:
    explicit View();
    ~View();
    void initgauge();
    void inittendance();


    QcGaugeWidget *getMAirspeedGaugetemp() const;
    QcGaugeWidget *getMAirspeedGaugehumidity() const;
    QcGaugeWidget *getMAirspeedGaugepressure() const;

    QcLabelItem *getLab() const;
    QcLabelItem *getLab1() const;
    QcLabelItem *getLab2() const;

    QcNeedleItem *getMAirspeedNeedletemp() const;
    QcNeedleItem *getMAirspeedNeedlehum() const;
    QcNeedleItem *getMAirspeedNeedlepres() const;

private:

    QGridLayout *fenetre;//gestionnaire de la fenêtre de controle
    /////////les gauges/////////
    //il faut les mettre en attribut car ils ont des parents
    QcGaugeWidget *mAirspeedGaugetemp;
    QcGaugeWidget *mAirspeedGaugehumidity;
    QcGaugeWidget *mAirspeedGaugepressure;
    ///////les Aiguilles////////
    QcNeedleItem *mAirspeedNeedletemp;
    QcNeedleItem *mAirspeedNeedlehum;
    QcNeedleItem *mAirspeedNeedlepres;
    //////l'affichage des valeurs/////
    //permet la création des getters pour manipuler l'affichage dans le Presenter
    QcLabelItem *lab;
    QcLabelItem *lab1;
    QcLabelItem *lab2;

    QVector<QcGaugeWidget*> tabgaugetend;//tableau contenant le gauges
    QVector<QcNeedleItem*> tabaiguille;
    QcLabelItem *fleche; //la fleche des tendances présente sous les trois gauges de tendances

};

#endif //STATIONMETEO_VIEW_H
