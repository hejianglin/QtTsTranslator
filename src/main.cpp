//Qt
#include <QApplication>
#include <QTranslator>

//QtTsTranslator
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTranslator translator;
    translator.load(QCoreApplication::applicationDirPath() +"/qt_QtTsTranslator_zh.qm");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
