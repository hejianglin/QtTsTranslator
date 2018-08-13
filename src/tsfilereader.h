#ifndef TSFILEREADER_H
#define TSFILEREADER_H

//Qt
#include <QFile>
#include <QXmlStreamReader>

//QtTsFileTranslator
#include "tsfileinfo.h"

class TsFileReader
{
public:
    TsFileReader();
    ~TsFileReader();
    bool read(const QString &sFile,TsFileInfo &);
    QString errorString() const;

private:
    QFile m_fileTs;
    QString m_sReadingName;
    QString m_sError;
    QXmlStreamReader m_xmlReader;

    bool isValidFile(const QString &sFile);

    void readVersion(Version &version);
    void readContext(ContextMap &context);
    void readName(ContextMap &context);
    void readMessage(ContextMap &context);
    void readLocation(Message *);
    void readSource(Message *);
    void readTranslation(Message *);
};


#endif // TSFILEREADER_H
