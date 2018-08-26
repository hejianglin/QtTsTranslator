//QtTsTranslator
#include "tsfilereader.h"
#include "tsfilewriter.h"
#include "tsfiletranslator.h"
#include "translationcontroller.h"


TranslationController::TranslationController(QObject *parent)
    : QObject(parent)
    , m_iIndex(0)
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
   m_iIndex = 0;
   m_sError.clear();
   foreach(ProgressInfo *info,m_hashProgressInfo){
       delete info;
   }
   m_hashProgressInfo.clear();
}

void TranslationController::translate_impl(int translationCount)
{
    int leftCount = m_slFile.count() - m_iIndex;
    int realCount =  leftCount > translationCount ? translationCount : leftCount;

    TsFileReader reader;
    for(int i = 0; i < realCount; i++)
    {
        //parse file
        TsFileInfo *tsFileInfo = new TsFileInfo;
        if(!reader.read(m_slFile.value(m_iIndex),*tsFileInfo)){
            emit sigAddLog(tr("read file(%1) error:%2").arg(m_slFile.value(m_iIndex))
                           .arg(reader.errorString()));
            delete tsFileInfo;
            m_iIndex++;
            continue;
        }

        TsFileTranslator *translator = new TsFileTranslator(this);
        translator->setTsFileInfo(tsFileInfo);
        translator->setTranslatorClient(m_cTranslationConfig.translatorClient());
        translator->setAppID(m_cTranslationConfig.appID());
        translator->setAppKey(m_cTranslationConfig.appKey());
        translator->setSourceLanguage(m_cTranslationConfig.sourceLanguage());
        translator->setTargetLanguage(m_cTranslationConfig.targetLanguage());
        connect(translator,SIGNAL(sigAddLog(QString,LogType)),
                this,SIGNAL(sigAddLog(QString,LogType)));
        connect(translator,SIGNAL(translationProgress(quint32,quint32)),
                this,SLOT(slotTranslationProgress(quint32,quint32)));
        connect(translator,SIGNAL(finished()),this,SLOT(slotFinished()));

        translator->translate();
        connect(translator,SIGNAL(finished()),translator,SLOT(deleteLater()));
        m_iIndex++;
    }
}

void TranslationController::slotTranslationProgress(quint32 finished,quint32 total)
{
    TsFileTranslator *translator = qobject_cast<TsFileTranslator *>(sender());
    if(translator == Q_NULLPTR){
        return ;
    }

    ProgressInfo *progressInfo = m_hashProgressInfo.value(translator->tsFileInfo()->fileName,0);
    if(!progressInfo){

        progressInfo = new ProgressInfo;
        m_hashProgressInfo.insert(translator->tsFileInfo()->fileName,progressInfo);
    }

    progressInfo->finished = finished;
    progressInfo->total = total;
    updateProgress();

}

void TranslationController::updateProgress()
{
    if(m_slFile.isEmpty()){
        emit translationProgress(1);
        emit finished();
        return ;
    }

    quint64 finishedMessage = 0,totalMessage = 0;
    foreach (ProgressInfo *info, m_hashProgressInfo) {
        finishedMessage += info->finished;
        totalMessage += info->total;
    }

    if(finishedMessage == totalMessage){
        emit translationProgress(1);
        emit finished();
        return ;
    }

    qreal progress = 0;
    if(totalMessage > 0){
        progress = finishedMessage * 1.0 / totalMessage;
    }

    emit translationProgress(progress);
}

void TranslationController::slotFinished()
{
    TsFileTranslator *translator = qobject_cast<TsFileTranslator *>(sender());
    if(translator == Q_NULLPTR){
        return ;
    }

    TsFileInfo *tsFileInfo = translator->tsFileInfo();
    TsFileWriter writer;
    bool bOk = writer.write(m_cTranslationConfig.handleFileMode() == TranslationConfig::HandleFileMode_eNew ? \
                                tsFileInfo->fileName + m_cTranslationConfig.newFileSuffix() :
                                tsFileInfo->fileName,*tsFileInfo);
    emit finished(tsFileInfo->fileName,bOk);
    delete tsFileInfo;
}



