#ifndef QSFMLCANVAS_HPP
#define QSFMLCANVAS_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>

//////////////////////////////////////////////////////////////////////////
//Code du site: http://www.sfml-dev.org/tutorials/1.6/graphics-qt-fr.php//
//Ce code sert à utiliser sfml et Qt ensemble dans un projet            //
//////////////////////////////////////////////////////////////////////////

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :

    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

private :

    virtual void OnInit();
    virtual void OnUpdate();
    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);
    QTimer  myTimer;       ///< Timer used to update the view
    bool    myInitialized; ///< Tell whether the SFML window has been initialized or not
};


#endif // QSFMLCANVAS_HPP
