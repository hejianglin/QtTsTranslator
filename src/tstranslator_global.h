#ifndef TSTRANSLATOR_GLOBAL_H
#define TSTRANSLATOR_GLOBAL_H

//Qt

//////////////////////////////////
/// user config
#define CONCURRENT_TRANSLATION_FILE_MAX 5
#define CONCURRENT_TRANSLATION_MESSAGE_MAX 15
#define TRANSLATION_MESSAGE_MAX_TRY_COUNT 3


//////////////////////////////////
///
enum LogType
{
    LogType_eDebug,
    LogType_eInfo,
    LogType_eWarning,
    LogType_eError
};

#if 1
#include <QDebug>
#include <QtGlobal>
#define DEBUG(x) do{qDebug()<<Q_FUNCTION_INFO<<" ["<<__LINE__<<"] "<<x;}while(0);
#else
#define DEBUG(x) do{}while(0);
#endif





#endif // TSTRANSLATOR_GLOBAL_H
