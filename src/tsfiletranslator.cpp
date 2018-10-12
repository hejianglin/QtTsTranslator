#include "tsfilereader.h"
#include "tsfilewriter.h"
#include "tsfiletranslator.h"

TsFileTranslator::TsFileTranslator(QObject *parent)
    : QObject(parent)
    , m_bRetranslationFinished(false)
    , m_iTotolMessage(0)
    , m_cTranslator(new NetworkTranslator(this))
{

    connect(m_cTranslator,SIGNAL(finished(NetworkTranslatorReply)),
            this,SLOT(slotTranslateFinished(NetworkTranslatorReply)));
}

TsFileTranslator::~TsFileTranslator()
{
    m_cFileInfo.clear();
    DEBUG(Q_FUNC_INFO)
}

void TsFileTranslator::setTranslatorEngine(TranslationEngine engine)
{
    m_cTranslator->setTranslatorEngine(engine);
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

void TsFileTranslator::setRetranslationFininshed(bool b)
{
    m_bRetranslationFinished = b;
}

void TsFileTranslator::setFile(const QString &file)
{
    m_sFile = file;
}

QString TsFileTranslator::file() const
{
   return m_sFile;
}

QString TsFileTranslator::errorString() const
{
    return m_sError;
}

bool TsFileTranslator::isValid()
{
    m_sError.clear();

    //配置参数
    if(!m_cTranslator->isValid()){
        m_sError = m_cTranslator->errorString();
        return false;
    }

    //文件检测
    if(m_sFile.isEmpty()){
        m_sError = tr("ts file is not set");
        return false;
    }

    if(!QFile::exists(m_sFile)){
        m_sError = tr("ts file is not exist");
        return false;
    }

    //读取文件
    TsFileReader fileReader;
    if(!fileReader.read(m_sFile,m_cFileInfo)){
        m_sError = tr("parse file error %1").arg(fileReader.errorString());
        return false;
    }

    //文件内容检查
    if(m_cFileInfo.contextMap.isEmpty()){
        m_sError = tr("file translation context is empty");
        return false;
    }

    return true;
}

bool TsFileTranslator::isError()
{
    return !(m_sError.isEmpty());
}

bool TsFileTranslator::translate()
{
    //参数检查
    if(!isValid()){
        emit addLog(tr("file(%1) translation error:%2").arg(m_sFile).arg(m_sError));
        emit finished();
        return false;
    }

    //初始化任务列表
    m_listMessage.clear();
    m_listMessage_Requesting.clear();
    foreach(Context *item,m_cFileInfo.contextMap.context()){
        if(m_bRetranslationFinished){
            m_listMessage.append(item->messageList());
        } else {
            m_listMessage.append(item->unfinishedMessageList());
        }
    }

    //检查是否存在需要翻译的内容
    if(m_listMessage.isEmpty()){
        emit addLog(tr("file(%1) is no need to translate information.").arg(m_sFile));
        m_sError = tr("no content needs translation");
        emit finished();
        return false;
    }

    m_iTotolMessage = static_cast<quint32>(m_listMessage.count());

    //开始翻译
    translate_impl(CONCURRENT_TRANSLATION_MESSAGE_MAX);
    return true;
}

void TsFileTranslator::translate_impl(int translateCount)
{
    if(m_listMessage.isEmpty() || translateCount < 1)
        return ;

    int realCount = translateCount > m_listMessage.count() ? \
                m_listMessage.count() : translateCount;

    for(int i = 0; i < realCount; ++i){
        Message *requestMessage = m_listMessage.takeFirst();//移除
        if(requestMessage->source().simplified().isEmpty()){
            continue;
        }
        m_listMessage_Requesting.append(requestMessage);
        m_cTranslator->translator(requestMessage->source());
    }
}

void TsFileTranslator::slotTranslateFinished(const NetworkTranslatorReply &reply)
{
    //查询相应信息
    Message *message = Q_NULLPTR;
    foreach(Message *item,m_listMessage_Requesting){
        if(item->source().simplified() == reply.source().simplified()){
            message = item;
            break;
        }
    }

    if(message == Q_NULLPTR){
        return;
    }

    //移除缓存信息
    m_listMessage_Requesting.removeOne(message);

    //解析结果
    if(reply.error() != NetworkTranslatorReply::TranslationError_eNone){
        qint8 failCount = m_hashFailRecord.value(message,0) + 1;
        if(failCount > TRANSLATION_MESSAGE_MAX_TRY_COUNT){
            //当前信息超出可尝试的最大次数,移除相应的记录,并不在尝试
            m_hashFailRecord.remove(message);
        }else{
            //重新添加到翻译列表,并设置失败信息
            m_listMessage.append(message);
            m_hashFailRecord.insert(message,failCount);
        }

        emit addLog(tr("translation (%1) fail:%2,failcount:%3 %4")\
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
        emit addLog(tr("translation (%1) success!").arg(reply.source()));
    }

    //如果还有则继续翻译,否则翻译结束
    if(!m_listMessage.isEmpty()){
        //保存??
        translate_impl(1);
    }else{
        if(m_listMessage_Requesting.isEmpty()){
            saveFile();
            emit finished();
        }
    }
}

void TsFileTranslator::updateProgress()
{
    quint32 iFinished =  m_iTotolMessage - static_cast<unsigned int>(m_listMessage.count()) \
            - static_cast<unsigned int>(m_listMessage_Requesting.count());
    emit translationProgress(iFinished,m_iTotolMessage);
}

bool TsFileTranslator::saveFile()
{
    TsFileWriter writer;
    return writer.write(m_sFile + ".tmp",m_cFileInfo);
}
