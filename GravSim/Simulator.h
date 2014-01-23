#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <SFML/Graphics.hpp>
#include "QSFMLCanvas.hpp"
#include <QPoint>
#include <QTimer>
#include "Particule.h"
#include <vector>
#include <math.h>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QTime>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QMessageBox>

#define SIMULATOR_SIZE 400
#define DEFAULT_IMAGE_FILE_PATH "images/defaultParticule.bmp"
#define CAMERA_PAN_SPEED 10
#define ZOOM_FACTOR 1
#define G_CONST 0.0000000000667384

/////////////////////////////////////////////////////////////////////////////////////
//QSFMLCanvas inspiré de : http://www.sfml-dev.org/tutorials/1.6/graphics-qt-fr.php//
/////////////////////////////////////////////////////////////////////////////////////

class Simulator :  public QSFMLCanvas
{
    Q_OBJECT

public :

    Simulator(const QPoint& Position = QPoint(0,0), const QSize& Size = QSize(100,100), QWidget* Parent = 0);
    void addParticule(const Particule& particuleModele);
    void addParticule(QString saveFile);
    void receiveParticuleData(double mass, sf::Vector2f position, sf::Vector2f vitesse);
    void removeParticule(const unsigned int i);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    bool isPaused(){return m_isPaused;}
    unsigned int getParticuleCount(){return m_part.size();}
    void resizeEvent(QResizeEvent *event);
    void invertGravity(){m_G *= -1;}
    void setFPS(double newFPS){m_FPS = newFPS;}
    void setTimeFactor(double newTimeFactor){m_TimeFactor = newTimeFactor;}
    void setCalculPerFrame(int newCalculPerFrame){m_CalculPerFrame = newCalculPerFrame;}

public slots:
    void clearAll();
    void start();
    void pause();
    void startPause();
    void reset();
    void createSaveFile();
    void loadExistingSaveFile();
    void setCheckPoint(){createCheckPoint(m_part);}

private :
    sf::Vector2f getForce(unsigned int partActuel);
    void OnUpdate();
    void collision(int a, int b);
    bool checkCollision();
    void createCheckPoint(std::vector<Particule*>);

    std::vector<Particule*> m_part;
    std::vector<Particule> m_checkPointParticleList;
    std::vector<sf::Image> m_images;
    QTimer *m_timer;
    sf::View m_view;
    sf::Sprite m_background;
    bool m_isPaused;

    double m_G;

    double m_FPS;
    double m_TimeFactor;
    unsigned int m_CalculPerFrame;

private slots:

    void periodicUpdate();

};

#endif // SIMULATOR_H
