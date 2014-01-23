#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <SFML/Graphics.hpp>
#include <QTimer>


class QSFMLCanvas : public QWidget , sf::RenderWindow
{
    public:

        QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

        virtual ~QSFMLCanvas();

    private:

        virtual void OnInit();

        virtual void OnUpdate();

        virtual QPaintEngine* paintEngine() const;

        virtual void showEvent(QShowEvent*);

        virtual void paintEvent(QPaintEvent*);

        QTimer myTimer;

        bool   myInitialized;


};

#endif // QSFMLCANVAS_H
