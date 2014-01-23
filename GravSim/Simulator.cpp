#include "Simulator.h"


Simulator::Simulator(const QPoint& Position, const QSize& Size, QWidget* Parent) :
    QSFMLCanvas(Parent, Position, Size), m_isPaused(true), m_G(G_CONST),
    m_FPS(24.), m_TimeFactor(1.), m_CalculPerFrame(1.)
{

    //Images
    m_images.push_back(sf::Image());
    m_images.back().LoadFromFile(DEFAULT_IMAGE_FILE_PATH);
    m_images.back().CreateMaskFromColor(sf::Color(255,255,255));
    m_background.SetColor(sf::Color(50,50,200));

    //Timer
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()),this, SLOT(periodicUpdate()));

    //Size
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setMinimumSize(SIMULATOR_SIZE , SIMULATOR_SIZE);

    setBaseSize(500,500);

    //View
    m_view = sf::View(sf::FloatRect(0,0,SIMULATOR_SIZE,SIMULATOR_SIZE));

}

void Simulator::addParticule(const Particule& particuleModele)
{
    m_part.push_back(new Particule(particuleModele));
}

void Simulator::receiveParticuleData(double mass, sf::Vector2f position, sf::Vector2f vitesse)
{
    addParticule(Particule(m_images.back(), mass, position, vitesse));
}

void Simulator::removeParticule(const unsigned int i)
{
    delete m_part[i];
    m_part.erase(m_part.begin() + i);
}

void Simulator::clearAll()
{
    pause();
    for (unsigned int i=getParticuleCount(); i>0; i--)
    {
        removeParticule(i-1);
    }
}

void Simulator::start()
{
    m_timer->start(1000/m_FPS);
    m_isPaused = false;
}


void Simulator::pause()
{
    m_timer->stop();
    m_isPaused = true;
}

void Simulator::reset()
{
    clearAll();
    pause();

    for (unsigned int i(0); i<m_checkPointParticleList.size(); i++)
    {
        addParticule(m_checkPointParticleList[i]);
    }
}

void Simulator::startPause()
{
    if (m_isPaused)
    {
        start();
    }
    else
    {
        pause();
    }
}

/*SAVE START*/
void Simulator::createSaveFile()
{

    //QFile saveFile(SAVE_PATH);
    QFile saveFile(QFileDialog::getSaveFileName(this, "Save Current Simulation", "Save/newSave", "Simulation Save File (*.ssf)"));

    if(saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //Préparation à l'écriture du fichier
        QTextStream out(&saveFile);

        //Écriture dans le fichier de sauvegarde avec séparateurs
        for (unsigned int i(0); i<getParticuleCount(); i++)
        {
            QString saveQString;

            saveQString.append(QString::number(m_part[i]->getMass()) + ",");

            saveQString.append(QString::number(m_part[i]->getPosition().x) + ",");
            saveQString.append(QString::number(m_part[i]->getPosition().y) + ",");

            saveQString.append(QString::number(m_part[i]->getVitesse().x) + ",");
            saveQString.append(QString::number(m_part[i]->getVitesse().y) + ",");

            out<<saveQString + "\n";
        }
        saveFile.close();
    }
}


void Simulator::loadExistingSaveFile()
{
    pause();

    QFile saveFile(QFileDialog::getOpenFileName(this, "Load Simulation Save File", "Save", "Simulation Save File (*.ssf)"));

    if(saveFile.open(QIODevice::ReadOnly | QIODevice::Text) && saveFile.exists())
    {
        clearAll();

        //Transfer des données vers saveQString
        QTextStream saveData(&saveFile);
        QString saveQString = saveData.readAll();

        for (int i(0); i < saveQString.count(QChar('\n')) ; i++)
        {
            addParticule(saveQString.section(QChar('\n'),i,i, QString::SectionSkipEmpty));
        }
        saveFile.close();
    }

    createCheckPoint(m_part);
}

void Simulator::addParticule(QString particuleSaveQString)
{    
    double mass = particuleSaveQString.section(QChar(','),0,0) . toDouble();

    double x = particuleSaveQString.section(QChar(','),1,1) . toDouble();
    double y = particuleSaveQString.section(QChar(','),2,2) . toDouble();

    double vx = particuleSaveQString.section(QChar(','),3,3) . toDouble();
    double vy = particuleSaveQString.section(QChar(','),4,4) . toDouble();

    addParticule(Particule(m_images.back() , mass , sf::Vector2f(x,y) , sf::Vector2f(vx,vy)));
}
/*SAVE END*/

void Simulator::createCheckPoint(std::vector<Particule*> v_part)
{
    m_checkPointParticleList.clear();

    for (unsigned int i(0); i<getParticuleCount(); i++)
    {
        m_checkPointParticleList.push_back(*v_part[i]);
    }
}



void Simulator::periodicUpdate()
{
    for (unsigned int j(0); j<m_CalculPerFrame; j++)
    {
        while(checkCollision());

        for (unsigned int i=0; i<getParticuleCount(); i++)
        {
            m_part[i]->applyForce(getForce(i));
        }

        for (unsigned int i=0; i<getParticuleCount(); i++)
        {
            m_part[i]->updateKineticProperties(m_TimeFactor / (m_FPS * m_CalculPerFrame));
        }
    }

    for (unsigned int i=0; i<getParticuleCount(); i++)
    {
        m_part[i]->updateSprite();
    }
}

void Simulator::OnUpdate()
{
    //Background
    Clear(sf::Color(80, 80, 255));

    //View
    SetView(m_view);

    //Draw all particules
    for (unsigned int i=0; i<getParticuleCount(); i++)
    {
        sf::Vector2f pos = m_part[i]->getPosition();
        if(     (pos.x + m_part[i]->getRadius() > m_view.GetRect().Left) && (pos.x - m_part[i]->getRadius() < m_view.GetRect().Right) &&
                (pos.y + m_part[i]->getRadius() > m_view.GetRect().Top) && (pos.y - m_part[i]->getRadius() < m_view.GetRect().Bottom))
        {
            Draw(m_part[i]->getSprite());
        }
    }
}


/////////////
///Physics///
/////////////
bool Simulator::checkCollision()
{
    sf::Vector2f pos1;
    sf::Vector2f pos2;
    sf::Vector2f deltaPos;
    double distance;
    for (unsigned int i(0); i<getParticuleCount(); i++)
    {
        for (unsigned int j(i+1); j<getParticuleCount(); j++)
        {
            pos1 = m_part[i]->getPosition();
            pos2 = m_part[j]->getPosition();
            deltaPos = pos1 - pos2;
            distance = sqrt((double)(deltaPos.x*deltaPos.x + deltaPos.y*deltaPos.y));

            if ( (distance < m_part[i]->getRadius())  ||  (distance < m_part[j]->getRadius()) )
            {
                collision(i,j);
                return true;
            }
        }
    }

    return false;
}

void Simulator::collision(int a, int b)
{

    double newMass = m_part[a]->getMass() + m_part[b]->getMass();

    double posFactor = m_part[a]->getMass()/newMass;
    sf::Vector2f position(  posFactor*m_part[a]->getPosition().x + (1-posFactor)*m_part[b]->getPosition().x ,
                            posFactor*m_part[a]->getPosition().y + (1-posFactor)*m_part[b]->getPosition().y);

    sf::Vector2f newVelocity(   (m_part[a]->getMass()*m_part[a]->getVitesse().x + m_part[b]->getMass()*m_part[b]->getVitesse().x)/newMass,
                                (m_part[a]->getMass()*m_part[a]->getVitesse().y + m_part[b]->getMass()*m_part[b]->getVitesse().y)/newMass);


    Particule newParticule(m_images.back(), newMass, position, newVelocity);
    addParticule(newParticule);

    //Détruire les anciennes particules dans le bon ordre
    if (a>b)
    {
        removeParticule(a);
        removeParticule(b);
    }
    else
    {
        removeParticule(b);
        removeParticule(a);
    }


}

sf::Vector2f Simulator::getForce(unsigned int partIndex)
{
    if ( (partIndex >= 0) && (partIndex < getParticuleCount()) )    //Index valide
    {
        double forceX(0);
        double forceY(0);

        for(unsigned int i(0); i < getParticuleCount(); i++)
        {
            if(i != partIndex)
            {
                if (m_part[i]->getPosition() != m_part[partIndex]->getPosition())
                {
                    double dX = m_part[i]->getPosition().x - m_part[partIndex]->getPosition().x;
                    double dY = m_part[i]->getPosition().y - m_part[partIndex]->getPosition().y;
                    double k = m_G * m_part[i]->getMass() * m_part[partIndex]->getMass() / sqrt(pow( dX*dX + dY*dY , 3)) ;

                    forceX += k*dX;
                    forceY += k*dY;
                }
            }
        }
        return sf::Vector2f(forceX,forceY);
    }

    else    //Index invalide
    {
        qDebug() << "Error: getForce() received invalid index";
        return sf::Vector2f(0,0);
    }
}


////////////
///Events///
////////////

void Simulator::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    float ratio = (float)newSize.width()/(float)newSize.height();

    double rX = (double)event->size().width() / (double)event->oldSize().width();
    if(rX <= 0){ rX = 1;}

    double rY = (double)event->size().height() / (double)event->oldSize().height();
    if(rY <= 0){ rY = 1;}

m_view = sf::View(sf::FloatRect( m_view.GetRect().Left - (rX - 1) * m_view.GetHalfSize().x + (rY - 1) * m_view.GetHalfSize().y,
                                     m_view.GetRect().Top ,
                                     m_view.GetRect().GetHeight()*ratio + m_view.GetRect().Left - (rX - 1) * m_view.GetHalfSize().x + (rY - 1) * m_view.GetHalfSize().y,
                                     m_view.GetRect().GetHeight() + m_view.GetRect().Top ) );

}

void Simulator::keyPressEvent(QKeyEvent *event)
{
    double SPEED_FACTOR = m_view.GetHalfSize().x/100;
    switch (event->key())
    {
        //Camera movements
        case Qt::Key_Left :
        {
        m_view.Move(-CAMERA_PAN_SPEED*SPEED_FACTOR,0);
            break;
        }

        case Qt::Key_Right :
        {
            m_view.Move(CAMERA_PAN_SPEED*SPEED_FACTOR,0);
            break;
        }

        case Qt::Key_Up :
        {
            m_view.Move(0,-CAMERA_PAN_SPEED*SPEED_FACTOR);
            break;
        }

        case Qt::Key_Down :
        {
            m_view.Move(0,CAMERA_PAN_SPEED*SPEED_FACTOR);
            break;
        }
    }
}

void Simulator::wheelEvent(QWheelEvent *event)
{

    double K;
    if (ZOOM_FACTOR > 0)
        K = ZOOM_FACTOR;
    else
    {
        K = .5;
        qDebug()<<"ZOOM_FACTOR invalide!";
    }

    m_view.Zoom(1 + (float)event->delta()/1200 * K);
}
