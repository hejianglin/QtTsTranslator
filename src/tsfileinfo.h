#ifndef TSFILEINFO_H
#define TSFILEINFO_H

//Qt
#include <QMap>
#include <QHash>
#include <QList>
#include <QString>
#include <QHashIterator>

///////////////////////////////////////
/// \brief The VersionInfo class
/// Ts file version info
class Version
{
public:
    Version(const QString version = "",const QString language = "")
        : m_sVersion(version)
        , m_sLanguage(language)
    {
    }

    Version(const Version &other)
    {
        this->m_sVersion = other.m_sVersion;
        this->m_sLanguage = other.m_sLanguage;
    }

    Version &operator = (const Version &other)
    {
        if(this != &other){
            this->m_sVersion = other.m_sVersion;
            this->m_sLanguage = other.m_sLanguage;
        }
        return *this;
    }

    inline void setVersion(const QString &version)
    {
        m_sVersion = version;
    }

    inline QString version() const
    {
        return m_sVersion;
    }

    inline void setLanguage(const QString &language)
    {
        m_sLanguage = language;
    }

    inline QString language() const
    {
        return m_sLanguage;
    }

private:
    QString m_sVersion;
    QString m_sLanguage;
};

///////////////////////////////////////
/// \brief The Location class
/// message location
class Location
{
public:
    Location()
        :m_iLine(-1)
    {

    }

    Location(qint32 iLine,const QString filename)
        : m_iLine(iLine)
        , m_sFileName(filename)
    {

    }

    Location(const Location &other)
    {
        this->m_iLine = other.m_iLine;
        this->m_sFileName = other.m_sFileName;
    }

    Location &operator = (const Location &other)
    {
        if(this != &other){
            this->m_iLine = other.m_iLine;
            this->m_sFileName = other.m_sFileName;
        }
        return *this;
    }

    inline void setLine(qint32 line)
    {
        m_iLine = line;
    }

    inline qint32 line() const
    {
        return m_iLine;
    }

    inline void setFileName(const QString &fileName)
    {
        m_sFileName = fileName;
    }

    inline QString fileName() const
    {
        return m_sFileName;
    }

private:
    qint32 m_iLine;
    QString m_sFileName;
};

typedef QList<Location> LocationList;
class Message
{
public:
    Message()
    {

    }

    Message(const QString &source,QString translation = "",LocationList locationList= LocationList())
        : m_locationList(locationList)
        , m_sSource(source)
        , m_sTranslation(translation)
    {

    }

    Message(const Message &other)
    {
        this->m_locationList = other.m_locationList;
        this->m_sSource = other.m_sSource;
        this->m_sTranslation = other.m_sTranslation;
    }

    Message & operator = (const Message &other)
    {
        if(this != &other){
            this->m_locationList = other.m_locationList;
            this->m_sSource = other.m_sSource;
            this->m_sTranslation = other.m_sTranslation;
        }
        return *this;
    }

    inline void addLocation(qint32 line,const QString &filename)
    {
        m_locationList.append(Location(line,filename));
    }

    inline LocationList location() const
    {
        return m_locationList;
    }

    inline void setSource(const QString &source)
    {
        m_sSource = source;
    }

    inline QString source() const
    {
        return m_sSource;
    }

    inline void setTranslation(const QString &translation)
    {
        m_sTranslation = translation;
    }

    inline QString translation() const
    {
        return m_sTranslation;
    }

private:
    LocationList m_locationList;
    QString m_sSource;
    QString m_sTranslation;
};

typedef QList<Message *> MessageList;
class Context
{
public:
    Context(const QString sName = "");
    ~Context();

    Context(const Context &other);
    Context & operator =(const Context &other);

    void setName(const QString &);
    QString name() const;

    void addMessage(Message *message);
    MessageList messageList() const;

private:
    QString m_sName;
    QList<Message *> m_lMessage;
};

///////////////////////////////////////
/// \brief The TsFileInfo class
/// whole ts file
typedef QList<Context *> ContextList;
typedef QHash<QString,Context *> ContextMap;
typedef QHashIterator<QString,Context *> ContextMapIterator;
class TsFileInfo
{
public:    
    void clear()
    {
        fileName.clear();

        version = Version();

        ContextMapIterator it(contextMap);
        while(it.hasNext()){
            it.next();
            delete it.value();
        }

        contextMap.clear();
    }

    QString fileName;
    Version version;
    ContextMap contextMap;
};



#endif // TSFILEINFO_H
