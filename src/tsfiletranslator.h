#ifndef TSFILETRANSLATOR_H
#define TSFILETRANSLATOR_H

//Qt
#include <QObject>

//QtTranslator
#include "tsfileinfo.h"
#include "translationconfig.h"
#include "tstranslator_global.h"

//QNetworkTranslator
#include "networktranslator_global.h"
#include "networktranslator.h"

USE_NETWORKTRANSLATOR_NAMESPACE

class TsFileTranslator : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TsFileTranslator)

public:
    explicit TsFileTranslator(QObject *parent = 0);
    ~TsFileTranslator();

    void setTranslatorEngine(TranslationEngine);
    void setAppID(const QString &appid);
    void setAppKey(const QString &appkey);
    void setSourceLanguage(LanguageType);
    void setTargetLanguage(LanguageType);
    void setRetranslationFininshed(bool);

    void setFile(const QString &file);
    QString file() const;

    bool isValid();
    bool isError();
    QString errorString() const;

public slots:
    bool translate();

signals:
    void addLog(const QString &log,LogType logType = LogType_eInfo);
    void translationProgress(quint32 finishedMessage, quint32 totalMessage);
    void finished();

private:
    void translate_impl(int translateCount = 1);
    void updateProgress();
    bool saveFile();

    bool m_bRetranslationFinished;
    QString m_sError;
    QString m_sFile;
    quint32 m_iTotolMessage;
    MessageList m_listMessage;
    MessageList m_listMessage_Requesting;
    QHash<Message *,qint8> m_hashFailRecord;
    TsFileInfo m_cFileInfo;
    NetworkTranslator *m_cTranslator;

private slots:
    void slotTranslateFinished(const NetworkTranslatorReply &);
};

#endif // TSFILETRANSLATOR_H
