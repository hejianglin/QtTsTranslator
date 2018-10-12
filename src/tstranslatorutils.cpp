//Qt
#include <QtCore/QTime>
#include <QtCore/QCoreApplication>

//QtTsTranslator
#include "tstranslatorutils.h"


void TsTranslatorUtils::delay(quint32 msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
