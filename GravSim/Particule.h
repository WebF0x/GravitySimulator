/*
    Copyright (c) 2013, Maxime Dupuis, Philippe Roy Villeneuve 
*/

#ifndef PARTICULE_H
#define PARTICULE_H

#include <SFML/Graphics.hpp>
#include <math.h>

#define PARTICULE_MINIMUM_SIZE 1
#define VOLUMIC_MASS 4128

class Particule
{

public:
    Particule(const sf::Image &image, const double masse = 1.,const sf::Vector2f position = sf::Vector2f(0,0), const sf::Vector2f vitesse = sf::Vector2f(0,0));
    Particule(const Particule &particuleModele);

    void applyForce(sf::Vector2f appliedForce)  { m_acceleration = sf::Vector2f(appliedForce.x / m_mass , appliedForce.y / m_mass); }
    void updateKineticProperties(const double dT);

    double getKineticEnergy()   { return m_mass/2 * (m_vitesse.x*m_vitesse.x + m_vitesse.y*m_vitesse.y); }
    sf::Vector2f getMomentum()  { return sf::Vector2f(m_mass*m_vitesse.x , m_mass*m_vitesse.y); } //Quantité de mouvement pour collisions (vecteur)

    void scaleToMass();

    sf::Sprite getSprite()  {return m_sprite;}
    void updateSprite();

    double getMass(){return m_mass;}
    void setMass(double m){m_mass = m;}

    sf::Vector2f getPosition(){return m_position;}
    void setPosition(const sf::Vector2f position){m_position = position;}

    sf::Vector2f getVitesse()   {return m_vitesse;}
    void setVitesse(sf::Vector2f vitesse)   {m_vitesse = vitesse;}

    sf::Vector2f getAcceleration()   {return m_acceleration;}
    void setAcceleration(sf::Vector2f acceleration)   {m_acceleration = acceleration;}

    double getRadius() {return m_sprite.GetSize().x/2;}    //Fonctionne juste si les particules sont rondes

private:
    sf::Sprite m_sprite;
    double m_mass;

    sf::Vector2f m_position;
    sf::Vector2f m_vitesse;
    sf::Vector2f m_acceleration;
    //double initGravEnergy //Si on veut un refroidissement ... or is it?
};


#endif // PARTICULE_H

