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

class Setting : public QFrame {
Q_OBJECT

private:
    QGridLayout *settingview;

    QLabel *parametre;
    QLabel *valeur;
    QLabel *titre;
    QLabel *dureetendance;
    QLabel *dureerafraichissement;
    QLineEdit *valeurtendance;
    QLineEdit *valeurrafraichissement;

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
    //met les valeurs par défaut
};

#endif //STATIONMETEO_SETTING_H
