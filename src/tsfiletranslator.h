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

    void setTranslatorClient(TranslationClient);
    void setAppID(const QString &appid);
    void setAppKey(const QString &appkey);
    void setSourceLanguage(LanguageType);
    void setTargetLanguage(LanguageType);

    void setTsFileInfo(TsFileInfo *);
    TsFileInfo *tsFileInfo() const;

    bool isValid();
    QString errorString() const;

public slots:
    bool translate();
    bool translate(TsFileInfo *);

signals:
    void translationProgress(quint32 finishedMessage, quint32 totalMessage);
    void finished();

    void sigAddLog(const QString &log,LogType logType = LogType_eInfo);

private:
    void translate_impl(int translateCount = 1);
    void updateTranslationConfig();
    void updateProgress();

    QString m_sError;
    quint32 m_iTotolMessage;
    MessageList m_listMessage;
    MessageList m_listMessage_Requesting;
    QHash<Message *,qint8> m_hashFailRecord;
    NetworkTranslator *m_cTranslator;
    TsFileInfo *m_cTsFileInfoPoint;

private slots:
    void slotTranslateFinished(const NetworkTranslatorReply &);
};

#endif // TSFILETRANSLATOR_H
