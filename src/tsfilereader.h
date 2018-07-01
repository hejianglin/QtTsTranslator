#ifndef TSFILEREADER_H
#define TSFILEREADER_H

//Qt
#include <QFile>
#include <QXmlStreamReader>

//QtTsFileTranslator
#include "tstranslator_global.h"

class TsFileReader
{
public:
    TsFileReader(const QString file = "");

    bool setFile(const QString &file);

    bool read(TsInfo &info,ContextMap &Context);
    bool read(const QString &sFile,TsInfo &info,ContextMap &Context);

    QString errorString() const;

private:
    QFile m_fileTs;
    QString m_sReadingName;
    QXmlStreamReader m_xmlReader;

    bool isValidFile(const QString &sFile);

    void readTsInfo(TsInfo &info);
    void readContext(ContextMap &context);
    void readName(ContextMap &context);
    void readMessage(ContextMap &context);
    void readLocation(Message *);
    void readSource(Message *);
    void readTranslation(Message *);
};


#endif // TSFILEREADER_H
