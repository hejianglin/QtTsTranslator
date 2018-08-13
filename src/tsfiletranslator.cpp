#include "tsfiletranslator.h"

#include <QDebug>

TsFileTranslator::TsFileTranslator(QObject *parent)
    : QObject(parent)
    , m_iTotolMessage(0)
    , m_cTranslator(new NetworkTranslator(this))
    , m_cTsFileInfoPoint(Q_NULLPTR)
{
    connect(m_cTranslator,SIGNAL(finished(NetworkTranslatorReply)),
            this,SLOT(slotTranslateFinished(NetworkTranslatorReply)));
}

TsFileTranslator::~TsFileTranslator()
{
//    qDebug()<<Q_FUNC_INFO;
}

void TsFileTranslator::setTranslatorClient(TranslationClient client)
{
    m_cTranslator->setTranslatorClient(client);
}

void TsFileTranslator::setAppID(const QString &appid)
{
    m_cTranslator->setAppID(appid);
}

void TsFileTranslator::setAppKey(const QString &appkey)
{
    m_cTranslator->setAppKey(appkey);
}

void TsFileTranslator::setSourceLanguage(LanguageType source)
{
    m_cTranslator->setDefaultSourceLanguage(source);
}

void TsFileTranslator::setTargetLanguage(LanguageType target)
{
    m_cTranslator->setDefaultTargetLanguage(target);
}

void TsFileTranslator::setTsFileInfo(TsFileInfo *fileInfo)
{
    m_cTsFileInfoPoint = fileInfo;
}

TsFileInfo * TsFileTranslator::tsFileInfo() const
{
    return m_cTsFileInfoPoint;
}

QString TsFileTranslator::errorString() const
{
    return m_sError;
}

bool TsFileTranslator::translate(TsFileInfo *fileInfo)
{
    m_cTsFileInfoPoint = fileInfo;
    return translate();
}

bool TsFileTranslator::isValid()
{
    m_sError.clear();

    if(m_cTsFileInfoPoint == Q_NULLPTR){
        m_sError = tr("ts file info unset");
        return false;
    }

    if(!m_cTranslator->isValid()){
        m_sError = m_cTranslator->errorString();
        return false;
    }

    return true;
}

bool TsFileTranslator::translate()
{
    if(!isValid()){
        emit finished();
        return false;
    }

    if(m_cTsFileInfoPoint->contextMap.isEmpty()){
        emit finished();;
        return true;
    }

    //init task list
    m_listMessage.clear();
    m_listMessage_Requesting.clear();
    ContextMapIterator it(m_cTsFileInfoPoint->contextMap);
    while(it.hasNext()){
        it.next();
        m_listMessage.append(it.value()->messageList());
    }
    m_iTotolMessage = m_listMessage.count();

    //translate
    translate_impl(CONCURRENT_TRANSLATION_MESSAGE_MAX);
    return true;
}

void TsFileTranslator::translate_impl(int translateCount)
{
    if(m_listMessage.isEmpty())
        return ;

    int realCount = translateCount > m_listMessage.count() ? \
                m_listMessage.count() : translateCount;

    for(int i = 0; i < realCount; ++i){
        Message *requestMessage = m_listMessage.takeFirst();
        if(requestMessage->source().simplified().isEmpty()){
            continue;
        }
        m_listMessage_Requesting.append(requestMessage);
        m_cTranslator->translator(requestMessage->source());
    }
}

void TsFileTranslator::slotTranslateFinished(const NetworkTranslatorReply &reply)
{
    //find message
    Message *message = Q_NULLPTR;
    foreach(Message *item,m_listMessage_Requesting){
        if(item->source().simplified() == reply.source().simplified()){
            message = item;
            break;
        }
    }

    //check
    if(message == Q_NULLPTR){
        return;
    }

    //remove the message
    m_listMessage_Requesting.removeOne(message);


    if(reply.error() != NetworkTranslatorReply::TranslationError_eNone){

        qint8 failCount = m_hashFailRecord.value(message,0) + 1;
        if(failCount > TRANSLATION_MESSAGE_MAX_TRY_COUNT){
            //remove the failure record and do not attempt translation
            m_hashFailRecord.remove(message);
        }else{
            //append to task,retry
            m_listMessage.append(message);
            m_hashFailRecord.insert(message,failCount);
        }

        emit sigAddLog(tr("translate (%1) fail:%2,fail count:%3 %4")\
                       .arg(reply.source())\
                       .arg(reply.errorString())\
                       .arg(failCount)
                       .arg(failCount > TRANSLATION_MESSAGE_MAX_TRY_COUNT ? \
                                tr("translation task will be cast away") \
                              : tr("translation task will be retried")));
    }else{

        message->setTranslation(reply.target());

        m_hashFailRecord.remove(message);

        updateProgress();

        emit sigAddLog(tr("translate (%1) success!").arg(reply.source()));
    }

    //continue translation
    if(!m_listMessage.isEmpty()){
        translate_impl(1);
    }
}

void TsFileTranslator::updateProgress()
{
    quint32 iFinished =  m_iTotolMessage - static_cast<unsigned int>(m_listMessage.count()) \
            - static_cast<unsigned int>(m_listMessage_Requesting.count());
    emit translationProgress(iFinished,m_iTotolMessage);
    if(iFinished == m_iTotolMessage){
        emit finished();
    }
}


