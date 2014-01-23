#include <QtGui/QApplication>
#include "GravitySimulator.h"
#include <QIcon>
#include <QBitmap>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GravitySimulator gravSim;
    QPixmap iconPixmap("images/icon.bmp");
    iconPixmap.setMask(iconPixmap.createMaskFromColor(QColor(0,0,255)));
    gravSim.setWindowIcon(QIcon(iconPixmap));
    gravSim.show();

    return app.exec();
}
