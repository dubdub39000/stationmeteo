//
// Created by j.mailly on 07/01/2021.
//

#ifndef STATIONMETEO_SETTING_H
#define STATIONMETEO_SETTING_H


#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include "FenêtreGauge.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>


class Setting : public QFrame {
Q_OBJECT

private:
    QGridLayout *settingview;

    QLabel *parametre;
    QLabel *valeur;
    QLabel *titre;
    QLabel *dureetendance;
    QLabel *erreursaisie;
    QLabel *erreurrange;
    QLabel *dureerafraichissement;
    QLabel *rangetendance;
    QLabel *rangerefresh;
    QLineEdit *valeurtendance;
    QLineEdit *valeurrafraichissement;
    QLabel *couleur;
    QComboBox *choixcouleur;
    QVariant *test;

    QPushButton *valider;// bouton valider de la fenetre setting
    QPushButton *annuler;

public:
    explicit Setting();
    virtual ~Setting();
    void inittsetting();

    QPushButton *getValider() const;
    QPushButton *getAnnuler() const;
    QLineEdit *getValeurtendance() const;
    QLineEdit *getValeurrafraichissement() const;
    void affichageerreur(int nbr);
    void MAJsetting(); //permet de modifier des paramètres à la volé
    QComboBox *getChoixcouleur() const;
};

#endif //STATIONMETEO_SETTING_H
