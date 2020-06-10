#include "qenergyloggerdecoder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEnergyLoggerDecoder w;
    w.show();
    w.setMinimumHeight(700);
    //w.adjustSize();

    return a.exec();
}
