//Qt
#include <QXmlStreamAttributes>

//QtTsTranslator
#include "tsfilereader.h"

TsFileReader::TsFileReader()
{

}

TsFileReader::~TsFileReader()
{
    if(m_fileTs.isOpen()){
        m_fileTs.close();
    }
}

QString TsFileReader::errorString() const
{
    return m_sError;
}

bool TsFileReader::isValidFile(const QString &sFile)
{
    if(sFile.isEmpty() ||  !QFile::exists(sFile))
    {
        m_sError = QObject::tr("file(%1) not exist").arg(sFile);
        return false;
    }

    m_fileTs.setFileName(sFile);
    if(!m_fileTs.open(QIODevice::ReadOnly))
    {
        m_sError =  QObject::tr("file(%1) open fail").arg(sFile);
        return false;
    }
    return true;
}

bool TsFileReader::read(const QString &sFile, TsFileInfo &info)
{
    if(!isValidFile(sFile))
    {
        return false;
    }

    //read
    info.clear();
    info.fileName = sFile;
    m_xmlReader.setDevice(&m_fileTs);
    while(m_xmlReader.readNextStartElement()){
        if(m_xmlReader.name() == "TS"){
            readVersion(info.version);
        }
        if(m_xmlReader.name() == "context"){
            m_sReadingName.clear();
            readContext(info.contextMap);
        }
    }
    return true;
}

void TsFileReader::readVersion(Version &versionInfo)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "TS");
    versionInfo.setVersion(m_xmlReader.attributes().value("version").toString());
    versionInfo.setLanguage(m_xmlReader.attributes().value("language").toString());
}

void TsFileReader::readContext(ContextMap &context)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "context");
    while(m_xmlReader.readNextStartElement()){
        if(m_xmlReader.name() == "name"){
            readName(context);
        }else if(m_xmlReader.name() == "message"){
            readMessage(context);
        }else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void TsFileReader::readName(ContextMap &context)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "name");
    m_sReadingName = m_xmlReader.readElementText();
    context.append(m_sReadingName,new Context(m_sReadingName));
}

void TsFileReader::readMessage(ContextMap &context)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "message");
    Message *item = new Message;
    while(m_xmlReader.readNextStartElement()){
        if(m_sReadingName.isEmpty() || !context.isExists(m_sReadingName)){
            delete item;
            m_xmlReader.raiseError(QObject::tr("file format error!"));
            break;
        }
        if(m_xmlReader.name() == "location"){
            readLocation(item);
        }else if(m_xmlReader.name() == "source"){
            readSource(item);
        }else if(m_xmlReader.name() == "translation"){
            readTranslation(item);
        }else {
            m_xmlReader.skipCurrentElement();
        }
    }
    context[m_sReadingName]->addMessage(item);
}

void TsFileReader::readLocation(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "location");
    item->addLocation(m_xmlReader.attributes().value("line").toInt()\
                      ,m_xmlReader.attributes().value("filename").toString());
    m_xmlReader.skipCurrentElement();
}

void TsFileReader::readSource(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "source");
    item->setSource(m_xmlReader.readElementText());
}

void TsFileReader::readTranslation(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "translation");
    item->setTranslation(m_xmlReader.readElementText());
}

