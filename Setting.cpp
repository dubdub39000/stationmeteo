//
// Created by j.mailly on 07/01/2021.
//

#include "Setting.h"

Setting::Setting() {
    settingview = new QGridLayout();
    inittsetting();
    setLayout(settingview);
}

Setting::~Setting() {
    delete settingview;
}

void Setting::inittsetting() {
    titre = new QLabel("PARAMETRE D'AFFICHAGE");
    settingview->addWidget(titre,0,1);
    parametre = new QLabel("paramètre");
    settingview->addWidget(parametre,1,0);
    valeur = new QLabel("valeur");
    settingview->addWidget(valeur,1,3);
    dureetendance = new QLabel("nombre max de données utilisées pour la tendance");
    settingview->addWidget(dureetendance,2,0);
    valeurtendance = new QLineEdit;
    valeurtendance->setText("100");
    settingview->addWidget(valeurtendance,2,3);
    dureerafraichissement = new QLabel("durée rafraichisement en s");
    settingview->addWidget(dureerafraichissement,3,0);
    valeurrafraichissement = new QLineEdit;
    valeurrafraichissement->setText("2");
    settingview->addWidget(valeurrafraichissement,3,3);

    valider = new QPushButton("valider");
    valider->setObjectName("valider");
    settingview->addWidget(valider,4,0);

    annuler = new QPushButton("annuler");
    annuler->setObjectName("annuler");
    settingview->addWidget(annuler,4,2);
}

QPushButton *Setting::getValider() const {
    return valider;
}

QPushButton *Setting::getAnnuler() const {
    return annuler;
}

QLineEdit *Setting::getValeurtendance() const {
    return valeurtendance;
}

QLineEdit *Setting::getValeurrafraichissement() const {
    return valeurrafraichissement;
}


