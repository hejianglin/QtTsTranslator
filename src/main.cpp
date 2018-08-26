//Qt
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDesktopWidget>

//QtTsTranslator
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //load local translation
    QString locale = QLocale::system().name();

    //Qt translation
    QTranslator qtTranslator;
#ifdef QBT_USES_QT5
    qtTranslator.load(QString::fromUtf8("qtbase_") + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
    qtTranslator.load(QString::fromUtf8("qt_") + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    a.installTranslator(&qtTranslator);

    //application translation
    QTranslator translator;
    QString sTranslationFile = QApplication::applicationDirPath() + "/language/qt_"
            + a.applicationName() + locale + ".qm";
    translator.load(sTranslationFile);
    a.installTranslator(&translator);

    MainWindow w;
    w.move((QApplication::desktop()->width() - w.width())/2,
            (QApplication::desktop()->height() - w.height())/2);
    w.show();

    return a.exec();
}
