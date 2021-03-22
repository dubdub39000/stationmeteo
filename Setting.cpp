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
    delete erreursaisie;
    delete erreurrange;
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
    rangetendance = new QLabel("doit être supérieur à 10");
    settingview->addWidget(rangetendance,3,0);
    valeurtendance = new QLineEdit;
    valeurtendance->setText("100");// valeur a changer en cas de changement de valeur par défaut
    settingview->addWidget(valeurtendance,2,3);
    dureerafraichissement = new QLabel("durée rafraichisement en s");
    settingview->addWidget(dureerafraichissement,4,0);
    rangerefresh = new QLabel("doit être superieur à  1");
    settingview->addWidget(rangerefresh,5,0);
    valeurrafraichissement = new QLineEdit;
    valeurrafraichissement->setText("20");// valeur a changer en cas de changement de valeur par défaut
    settingview->addWidget(valeurrafraichissement,4,3);
    valider = new QPushButton("valider");
    valider->setObjectName("valider");
    settingview->addWidget(valider,7,0);
    erreursaisie = new QLabel("<span style=\"color:red\">Erreur de saisie </span>");
    test = new QVariant;
    choixcouleur = new QComboBox;
    choixcouleur->addItem("grey", *test);
    choixcouleur->addItem("white",*test);
    couleur = new QLabel("couleur du fond");
    settingview->addWidget(couleur,6,2);
    settingview->addWidget(choixcouleur,6,3);

    annuler = new QPushButton("annuler");
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

