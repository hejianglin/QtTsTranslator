#include <QXmlStreamAttributes>

#include "tsfilereader.h"

TsFileReader::TsFileReader()
{

}

bool TsFileReader::isValidFile(const QString &sFile)
{
    if(sFile.isEmpty() ||  !QFile::exists(sFile))
    {
        sError = QObject::tr("file(%1) not exist").arg(sFile);
        return false;
    }

    m_fileTs.setFileName(sFile);
    if(!m_fileTs.open(QIODevice::ReadOnly))
    {
        sError =  QObject::tr("file(%1) open fail").arg(sFile);
        return false;
    }

    return true;
}

bool TsFileReader::read(const QString &sFile,ContextMap &context, QString &sError)
{
    if(!isValidFile(sFile,sError))
    {
        return false;
    }

    //read
    m_xmlReader.setDevice(&m_fileTs);
    while(m_xmlReader.readNextStartElement()){
        if(m_xmlReader.name() == "context"){
            m_sReadingName.clear();
            readContext(context);
        }
    }
    return true;
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
    context.insert(m_sReadingName,new Context(m_sReadingName));
}

void TsFileReader::readMessage(ContextMap &context)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "message");
    Message *item = new Message;
    while(m_xmlReader.readNextStartElement()){
        if(m_sReadingName.isEmpty() || !context.value(m_sReadingName)){
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
        context[m_sReadingName]->addMessage(item);
    }
}

void TsFileReader::readLocation(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "location");
    item->cLocation.sFileName = m_xmlReader.attributes().value("filename").toString();
    item->cLocation.iLine =  m_xmlReader.attributes().value("line").toInt();
    m_xmlReader.skipCurrentElement();
}

void TsFileReader::readSource(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "source");
    item->sSource = m_xmlReader.readElementText();
}

void TsFileReader::readTranslation(Message *item)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "translation");
    item->sTranslation = m_xmlReader.readElementText();
}

