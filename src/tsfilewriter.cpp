//Qt
#include <QHash>
#include <QFile>
#include <QXmlStreamWriter>

//QtTsTranslator
#include "tsfilewriter.h"


TsFileWriter::TsFileWriter()
{
    m_cXmlWriter.setAutoFormatting(true);
}

QString TsFileWriter::errorString() const
{
    return m_sError;
}
bool TsFileWriter::write(const QString &file, TsFileInfo &info,bool overwrite)
{
    QFile tsFile(file);
    if(tsFile.exists() && !overwrite){
        m_sError = QObject::tr("file existed");
        return  false;
    }

    if(!tsFile.open(QFile::WriteOnly)){
        m_sError = QObject::tr("file open error:%1").arg(tsFile.errorString());
        return false;
    }

    m_cXmlWriter.setDevice(&tsFile);
    m_cXmlWriter.writeStartDocument();
    m_cXmlWriter.writeDTD("<!DOCTYPE TS>");
    m_cXmlWriter.writeStartElement("TS");
    m_cXmlWriter.writeAttribute("version",info.version.version());
    m_cXmlWriter.writeAttribute("language",info.version.language());

    writeContext(info.contextMap);

    m_cXmlWriter.writeEndElement();
    m_cXmlWriter.writeEndDocument();
    tsFile.close();
    return true;
}

void TsFileWriter::writeContext(const ContextMap &contextMap)
{
    ContextMapIterator it(contextMap);
    while(it.hasNext()){
        it.next();
        m_cXmlWriter.writeStartElement("context");
        m_cXmlWriter.writeTextElement("name",it.key());
        writeMessage(it.value()->messageList());
        m_cXmlWriter.writeEndElement();
    }
}

void TsFileWriter::writeMessage(const MessageList &messageList)
{
    foreach(Message *message,messageList){
        m_cXmlWriter.writeStartElement("message");
        writeLocation(message->location());
        m_cXmlWriter.writeTextElement("source",message->source());
        m_cXmlWriter.writeTextElement("translation",message->translation());
        m_cXmlWriter.writeEndElement();
    }
}

void TsFileWriter::writeLocation(const LocationList &locationList)
{
    foreach(Location location,locationList){
        m_cXmlWriter.writeStartElement("location");
        m_cXmlWriter.writeAttribute("filename",location.fileName());
        m_cXmlWriter.writeAttribute("line",QString::number(location.line()));
        m_cXmlWriter.writeEndElement();
    }
}
