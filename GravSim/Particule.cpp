#include "Particule.h"
Particule::Particule(const sf::Image &image, const double masse, const sf::Vector2f position,
                     const sf::Vector2f vitesse) :

    m_mass(masse), m_position(position), m_vitesse(vitesse), m_acceleration(sf::Vector2f(0,0))
{
    //Set sprite to loaded image
    m_sprite.SetImage(image);

    scaleToMass();
    updateSprite();
}

Particule::Particule(const Particule &particuleModele) :
    m_sprite(particuleModele.m_sprite), m_mass(particuleModele.m_mass), m_position(particuleModele.m_position),
    m_vitesse(particuleModele.m_vitesse), m_acceleration(particuleModele.m_acceleration)
{
}

void Particule::scaleToMass()
{
    double scaleFactor = log(m_mass);
    //double scaleFactor = pow(3 * m_mass / VOLUMIC_MASS / (4*3.1416) , 1./3.)*2; //V = 4*pi*r^3 / 3 //r = pow(3m/4/pi * M , 1/3)
    if (scaleFactor <= 1)
    {
        scaleFactor = PARTICULE_MINIMUM_SIZE;
    }
    m_sprite.Resize(scaleFactor , scaleFactor);
}

void Particule::updateSprite()
{
    m_sprite.SetPosition(sf::Vector2f(m_position.x - m_sprite.GetSize().x/2 ,
                                      m_position.y - m_sprite.GetSize().y/2 ));
}

void Particule::updateKineticProperties(const double dT)
{
    m_vitesse.x += m_acceleration.x * dT;
    m_vitesse.y += m_acceleration.y * dT;

    m_position.x =  m_position.x + (m_vitesse.x + .5 * m_acceleration.x * dT)*dT;
    m_position.y =  m_position.y + (m_vitesse.y + .5 * m_acceleration.y * dT)*dT;
}


/*
{
    //Connections
    //QObject::connect(this, SIGNAL(clicked()), this, SLOT(selected));
    //grabMouse();
    setFocusPolicy(Qt::StrongFocus);
}

void Particule::operator=(const Particule &particule)
{
    //Cinetic properties transfered too?
    mass = particule.mass;

}

void Particule::mousePressEvent(QMouseEvent* event)
{
    event->ignore();
    //this->hide();

    if ( (event->button() == Qt::LeftButton) && underMouse() )
    {
        emit clicked();
    }

}
*/

