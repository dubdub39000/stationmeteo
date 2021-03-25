//
// Created by j.mailly on 11/01/2021.
//

#include <iostream>
#include "Logview.h"

Logview::Logview() {
    logviewer = new QGridLayout();
    zonetrame = new QTextEdit;
    zonesystem = new QTextEdit;
    fermer = new QPushButton("fermer");
    clearflow = new QPushButton("clear");
    clearlogsys = new QPushButton("clear");
    flow = new QLabel("Trames JSON reçues");
    erreur = new QLabel("erreur system");
    setLayout(logviewer);
}

Logview::~Logview() {
    delete logviewer;
}

void Logview::initlog() {

    zonetrame->setReadOnly(true);
    zonesystem->setReadOnly(true);
    fermer->setObjectName("fermer");
    clearflow->setObjectName("clear");
    clearlogsys->setObjectName("clear");
    logviewer->addWidget(flow,0,0);
    logviewer->addWidget(erreur,0,2);
    logviewer->addWidget(zonetrame,2,0);
    logviewer->addWidget(zonesystem,2,2);
    logviewer->addWidget(fermer,3,1);
    logviewer->addWidget(clearflow,1,0);
    logviewer->addWidget(clearlogsys,1,2);
}

QTextEdit *Logview::getZonetrame() const {
    return zonetrame;
}

QTextEdit *Logview::getZonesystem() const {
    return zonesystem;
}

QPushButton *Logview::getFermer() const {
    return fermer;
}

QPushButton *Logview::getClearflow() const {
    return clearflow;
}

QPushButton *Logview::getClearlogsys() const {
    return clearlogsys;
}
