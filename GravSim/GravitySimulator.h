/*
    Copyright (c) 2013, Maxime Dupuis, Philippe Roy Villeneuve 
*/

#ifndef GRAVITYSIMULATOR_H
#define GRAVITYSIMULATOR_H

#include <QMainWindow>
#include "Particule.h"
#include "Simulator.h"
#include <QGridLayout>
#include <QLabel>
#include <QString>

namespace Ui {
class GravitySimulator;
}

class GravitySimulator : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GravitySimulator(QWidget *parent = 0);
    ~GravitySimulator();

private:
    Ui::GravitySimulator *m_ui;
    Simulator *m_simulator;
    QGridLayout *m_mainGridLayout;
    bool m_clickModeActivated;


private slots:
    void on_clickModeButton_clicked();
    void on_createParticuleButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void on_FPSLineEdit_textEdited(const QString &text);
    void on_TimeFactorLineEdit_textEdited(const QString &text);
    void on_CalculPerFrameLineEdit_textEdited(const QString &text);
    void on_InvertedGravityCheckBox_stateChanged(){m_simulator->invertGravity();}



};

#endif // GRAVITYSIMULATOR_H
