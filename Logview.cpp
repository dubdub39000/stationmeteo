//
// Created by j.mailly on 11/01/2021.
//

#include "Logview.h"

Logview::Logview() {
    logviewer = new QGridLayout;
   initlog();
    setLayout(logviewer);
}

Logview::~Logview() {
    delete logviewer;
}

void Logview::initlog() {
    zonetrame = new QTextEdit;
    zonetrame->setReadOnly(true);
    zonesystem = new QTextEdit;
    zonesystem->setReadOnly(true);
    fermer = new QPushButton("fermer");
    fermer->setObjectName("fermer");
    clearflow = new QPushButton("clear");
    clearflow->setObjectName("clear");
    clearlogsys = new QPushButton("clear");
    clearlogsys->setObjectName("clear");
    flow = new QLabel("Trames JSON reçues");
    erreur = new QLabel("erreur system");
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

