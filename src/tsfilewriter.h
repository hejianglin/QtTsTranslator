#ifndef TSFILEWRITER_H
#define TSFILEWRITER_H

//Qt
#include <QXmlStreamWriter>

//QtTsTranslator
#include "tsfileinfo.h"

class TsFileWriter
{
public:
    TsFileWriter();
    bool write(const QString &file,TsFileInfo &,bool overwrite = true);
    QString errorString() const;

private:
    QString m_sError;
    QXmlStreamWriter m_cXmlWriter;

    void writeContext(const ContextMap &);
    void writeMessage(const MessageList &);
    void writeLocation(const LocationList &);
};

#endif // TSFILEWRITER_H
