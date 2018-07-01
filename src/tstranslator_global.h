#ifndef TSTRANSLATOR_GLOBAL_H
#define TSTRANSLATOR_GLOBAL_H

//Qt
#include <QMap>
#include <QHash>
#include <QList>
#include <QString>

//////////////////////////////////
/// user config
//////////////////////////////////
#define MAX_THREAD 5 //limit on the number of threads used at the same time
#define MAX_TRANSLATE_COUNT 10//translate quantity limit at the same time


//////////////////////////////////
/// base struct
//////////////////////////////////
/// message location ///
class Location
{
public:
    qint32 m_iLine;
    QString m_sFileName;

    Location() :iLine(-1)
    {

    }

    Location(qint32 iLine,const QString filename)
        : iLine(iLine)
        , sFileName(filename)
    {

    }
};

/// message ///
class Message
{
public:
    Message()
    {

    }

    Message(const QString &source,QString translation = "",qint32 line = -1,QString filename = "")
        : cLocation(line,filename)
        , sSource(source)
        , sTranslation(translation)
    {

    }

    Location cLocation;
    QString sSource;
    QString sTranslation;
};

/// context ///
class Context
{
public:
    Context(const QString sName = "")
    {
        m_sName = sName;
    }

    ~Context()
    {
        foreach(struct Message *message,m_lMessage){
            delete message;
        }
    }

    void addMessage(Message *message)
    {
        if(message == Q_NULLPTR){
            return ;
        }
        m_lMessage.append(message);
    }

    void addMessage(const QString &source,QString translation = "",qint32 line = -1,QString filename = "")
    {
        if(source.isEmpty()) {
            return ;
        }
        addMessage(new Message(source,translation,line,filename));
    }

    QString m_sName;
    QList<struct Message *> m_lMessage;
};

/// ts context list ///
typedef QHash<QString,Context *> ContextMap;

/// ts version info ///
class TsInfo
{
  public:
    TsInfo(const QString version = "",const QString language = "")
        : m_sVersion(version)
        , m_sLanguage(language)
    {

    }
    QString m_sVersion;
    QString m_sLanguage;
};

#endif // TSTRANSLATOR_GLOBAL_H
