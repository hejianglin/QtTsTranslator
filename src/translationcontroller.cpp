//QtTsTranslator
#include "tsfilereader.h"
#include "tsfilewriter.h"
#include "tsfiletranslator.h"
#include "translationcontroller.h"


TranslationController::TranslationController(QObject *parent)
    : QObject(parent)
{

}

TranslationController::~TranslationController()
{
    foreach(ProgressInfo *item,m_hashProgressInfo){
        delete item;
    }

    m_hashProgressInfo.clear();
}

void TranslationController::setTranslationConfig(const TranslationConfig &config)
{
    m_cTranslationConfig = config;
}

TranslationConfig TranslationController::translationConfig() const
{
    return m_cTranslationConfig;
}

QString TranslationController::errorString() const
{
    return m_sError;
}

void TranslationController::setFile(const QString &file)
{
    m_slFile = QStringList()<<file;
}

void TranslationController::setFile(const QStringList &file)
{
    m_slFile = file;
}

bool TranslationController::translate(const QString &file)
{
    setFile(file);
    return translate();
}

bool TranslationController::translate(const QStringList &file)
{
    setFile(file);
    return translate();
}

bool TranslationController::translate()
{
    if(!isValid()){
        return false;
    }

    clear();
    translate_impl(CONCURRENT_TRANSLATION_FILE_MAX);
    return true;
}

bool TranslationController::isValid()
{
    if(!m_cTranslationConfig.isValid()){
        m_sError = m_cTranslationConfig.errorString();
        return false;
    }

    return true;
}

void TranslationController::clear()
{
    m_sError.clear();
    foreach(ProgressInfo *info,m_hashProgressInfo){
        delete info;
    }
    m_hashProgressInfo.clear();
    m_slFile_Translating.clear();
}

void TranslationController::translate_impl(int translationCount)
{
    int leftCount = m_slFile.count();
    int realCount =  leftCount > translationCount ? translationCount : leftCount;

    for(int i = 0; i < realCount; i++)
    {
        TsFileTranslator *translator = new TsFileTranslator(this);
        QString sFile = m_slFile.takeFirst();
        m_slFile_Translating.append(sFile);
        translator->setFile(sFile);
        translator->setTranslatorEngine(m_cTranslationConfig.translatorEngine());
        translator->setAppID(m_cTranslationConfig.appID());
        translator->setAppKey(m_cTranslationConfig.appKey());
        translator->setSourceLanguage(m_cTranslationConfig.sourceLanguage());
        translator->setTargetLanguage(m_cTranslationConfig.targetLanguage());
        connect(translator,SIGNAL(addLog(QString,LogType)),
                this,SIGNAL(addLog(QString,LogType)));
        connect(translator,SIGNAL(translationProgress(quint32,quint32)),
                this,SLOT(slotTranslationProgress(quint32,quint32)));
        connect(translator,SIGNAL(finished()),this,SLOT(slotFinishedAFile()));
        connect(translator,SIGNAL(finished()),translator,SLOT(deleteLater()));
        emit addLog(tr("%1 start translate...").arg(translator->file()));
        translator->translate();
    }
}

void TranslationController::slotTranslationProgress(quint32 finished,quint32 total)
{
    TsFileTranslator *translator = qobject_cast<TsFileTranslator *>(sender());
    if(translator == Q_NULLPTR){
        return ;
    }

    ProgressInfo *progressInfo = m_hashProgressInfo.value(translator->file(),0);
    if(!progressInfo){

        progressInfo = new ProgressInfo;
        m_hashProgressInfo.insert(translator->file(),progressInfo);
    }

    progressInfo->finished = finished;
    progressInfo->total = total;
    updateProgress();
}

void TranslationController::updateProgress()
{
    quint64 finishedMessage = 0,totalMessage = 0;
    foreach (ProgressInfo *info, m_hashProgressInfo) {
        finishedMessage += info->finished;
        totalMessage += info->total;
    }

    if(finishedMessage == totalMessage){
        emit translationProgress(1);
        return ;
    }

    qreal progress = 0;
    if(totalMessage > 0){
        progress = finishedMessage * 1.0 / totalMessage;
    }

    emit translationProgress(progress);
}

void TranslationController::slotFinishedAFile()
{
    DEBUG(Q_FUNC_INFO)

    TsFileTranslator *translator = qobject_cast<TsFileTranslator *>(sender());
    if(translator == Q_NULLPTR){
        return ;
    }

    //删除缓存记录
    m_slFile_Translating.removeOne(translator->file());

    //翻译成功,根据配置保存文件
    if(!translator->isError()){
        QString sNewFile = translator->file();
        if(m_cTranslationConfig.handleFileMode() == TranslationConfig::HandleFileMode_eNew){
            sNewFile += m_cTranslationConfig.newFileSuffix();
        }
        QFile::rename(translator->file() + ".tmp",sNewFile);
    }

    emit finishedAFile(translator->file(),translator->isError());

    //检查翻译是否完成
    if(!m_slFile.isEmpty()){
        translate_impl(1);
    } else {
        if(m_slFile_Translating.isEmpty()){
            emit finished();
        }
    }
}



