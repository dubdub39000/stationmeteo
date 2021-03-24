//
// Created by j.mailly on 07/01/2021.
//


#include "Setting.h"

Setting::Setting() {
    settingview = new QGridLayout();
    titre = new QLabel("PARAMETRE D'AFFICHAGE");
    parametre = new QLabel("paramètre");
    valeur = new QLabel("valeur");
    dureetendance = new QLabel("nombre max de données utilisées pour la tendance");
    rangetendance = new QLabel("doit être supérieur à 10");
    valeurtendance = new QLineEdit;
    dureerafraichissement = new QLabel("durée rafraichisement en s");
    rangerefresh = new QLabel("doit être superieur à  1");
    valeurrafraichissement = new QLineEdit;
    valider = new QPushButton("valider");
    erreursaisie = new QLabel("<span style=\"color:red\">Erreur de saisie </span>");
    test = new QVariant;
    choixcouleur = new QComboBox;
    couleur = new QLabel("couleur du fond");
    annuler = new QPushButton("annuler");
    inittsetting();
    setLayout(settingview);
}

Setting::~Setting() {
    delete settingview;
    delete erreursaisie;
    delete erreurrange;
}

void Setting::inittsetting() {

    settingview->addWidget(titre,0,1);
    settingview->addWidget(parametre,1,0);
    settingview->addWidget(valeur,1,3);
    settingview->addWidget(dureetendance,2,0);
    settingview->addWidget(rangetendance,3,0);
    valeurtendance->setText("100");// valeur a changer en cas de changement de valeur par défaut
    settingview->addWidget(valeurtendance,2,3);
    settingview->addWidget(dureerafraichissement,4,0);
    settingview->addWidget(rangerefresh,5,0);
    valeurrafraichissement->setText("20");// valeur a changer en cas de changement de valeur par défaut
    settingview->addWidget(valeurrafraichissement,4,3);
    valider->setObjectName("valider");
    settingview->addWidget(valider,7,0);
    choixcouleur->addItem("grey", *test);
    choixcouleur->addItem("white",*test);
    settingview->addWidget(couleur,6,2);
    settingview->addWidget(choixcouleur,6,3);
    annuler->setObjectName("annuler");
    settingview->addWidget(annuler,7,2);
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

void Setting::affichageerreur(int nbr) {
    switch (nbr) {
        case 1:
            settingview->addWidget(erreursaisie,3,3);
            erreursaisie->show();
            break;
        case 2:
            settingview->addWidget(erreursaisie,5,3);
            erreursaisie->show();
            break;
    }
}

void Setting::MAJsetting() {
    erreursaisie->hide();
}

QComboBox *Setting::getChoixcouleur() const {
    return choixcouleur;
}

