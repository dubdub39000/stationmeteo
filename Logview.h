//
// Created by j.mailly on 11/01/2021.
//

#ifndef STATIONMETEO_LOGVIEW_H
#define STATIONMETEO_LOGVIEW_H


#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>

class Logview : public QFrame {

    Q_OBJECT
private:
    QGridLayout *logviewer;

    QLabel *flow;
    QLabel *erreur;
    QTextEdit *zonetrame;
    QTextEdit *zonesystem;

    QPushButton *fermer;
    QPushButton *clearflow;
    QPushButton *clearlogsys;
public:
explicit Logview();

    virtual ~Logview();
    void initlog();
    QTextEdit *getZonetrame() const;
    QTextEdit *getZonesystem() const;

    QPushButton *getFermer() const;
    QPushButton *getClearflow() const;
    QPushButton *getClearlogsys() const;

};

#endif //STATIONMETEO_LOGVIEW_H
