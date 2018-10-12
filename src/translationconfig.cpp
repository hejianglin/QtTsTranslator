//Qt
#include <QFile>
#include <QString>
#include <QSettings>
#include <QCoreApplication>

//QtTsTranslator
#include "translationconfig.h"

TranslationConfig::TranslationConfig()
    : m_bRetranslationFinishedMessage(false)
    , m_eTranslatorEngine(TranslationEngine_eNone)
    , m_eSourceLanguage(LanguageType_eAuto)
    , m_eTargetLanguage(LanguageType_eNone)
    , m_eHandleFileMode(HandleFileMode_eNew)
    , m_sNewFileSuffix(".tmp")
{
    loadFromFile();
}


TranslationConfig::TranslationConfig(const TranslationConfig &other)
{
    this->m_bRetranslationFinishedMessage = other.m_bRetranslationFinishedMessage;
    this->m_eTranslatorEngine = other.m_eTranslatorEngine;
    this->m_eSourceLanguage = other.m_eSourceLanguage;
    this->m_eTargetLanguage = other.m_eTargetLanguage;
    this->m_eHandleFileMode = other.m_eHandleFileMode;
    this->m_sAppID = other.m_sAppID;
    this->m_sAppKey = other.m_sAppKey;
    this->m_sNewFileSuffix = other.m_sNewFileSuffix;
}

TranslationConfig & TranslationConfig::operator =(const TranslationConfig &other)
{
    if(this != &other){
        this->m_bRetranslationFinishedMessage = other.m_bRetranslationFinishedMessage;
        this->m_eTranslatorEngine = other.m_eTranslatorEngine;
        this->m_eSourceLanguage = other.m_eSourceLanguage;
        this->m_eTargetLanguage = other.m_eTargetLanguage;
        this->m_eHandleFileMode = other.m_eHandleFileMode;
        this->m_sAppID = other.m_sAppID;
        this->m_sAppKey = other.m_sAppKey;
        this->m_sNewFileSuffix = other.m_sNewFileSuffix;
    }
    return *this;
}

void TranslationConfig::loadFromFile()
{
    //? hide this? or save to user config dir?
    QString configFile  = QCoreApplication::applicationDirPath() + "/config.ini";
    if(!QFile::exists(configFile)){
        return ;
    }

    QSettings configSettings(configFile,QSettings::IniFormat);

    //engine
    configSettings.beginGroup("engine");
    m_eTranslatorEngine = static_cast<TranslationEngine>(configSettings.value("engine").toInt());
    m_sAppID = configSettings.value("appid").toString();
    m_sAppKey = configSettings.value("appkey").toString();
    configSettings.endGroup();

    //language
    configSettings.beginGroup("language");
    m_eSourceLanguage = static_cast<LanguageType>(configSettings.value("source").toInt());
    m_eTargetLanguage = static_cast<LanguageType>(configSettings.value("target").toInt());
    configSettings.endGroup();

    //file
    configSettings.beginGroup("file");
    m_eHandleFileMode = static_cast<HandleFileMode>(configSettings.value("handleMode").toInt());
    m_sNewFileSuffix = configSettings.value("newFileSuffix").toString();
    configSettings.endGroup();

    //translation
    configSettings.beginGroup("translation");
    m_bRetranslationFinishedMessage = configSettings.value("retranslationFinishedMessage").toBool();
    configSettings.endGroup();
}

void TranslationConfig::saveToFile()
{
    //? hide this? or save to user config dir?
    QString configFile  = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings configSettings(configFile,QSettings::IniFormat);

    //engine
    configSettings.beginGroup("engine");
    configSettings.setValue("engine",static_cast<int>(m_eTranslatorEngine));
    configSettings.setValue("appid",m_sAppID);
    configSettings.setValue("appkey",m_sAppKey);
    configSettings.endGroup();

    //language
    configSettings.beginGroup("language");
    configSettings.setValue("source",static_cast<int>(m_eSourceLanguage));
    configSettings.setValue("target",static_cast<int>(m_eTargetLanguage));
    configSettings.endGroup();

    //file
    configSettings.beginGroup("file");
    configSettings.setValue("handleMode",static_cast<int>(m_eHandleFileMode));
    configSettings.setValue("newFileSuffix",m_sNewFileSuffix);
    configSettings.endGroup();

    //translation
    configSettings.beginGroup("translation");
    configSettings.setValue("retranslationFinishedMessage",false);
    configSettings.endGroup();

    configSettings.sync();
}

void TranslationConfig::setTranslatorEngine(TranslationEngine engine)
{
    m_eTranslatorEngine = engine;
}

TranslationEngine TranslationConfig::translatorEngine() const
{
    return m_eTranslatorEngine;
}

void TranslationConfig::setAppID(const QString &appid)
{
    m_sAppID = appid;
}

QString TranslationConfig::appID() const
{
    return m_sAppID;
}

void TranslationConfig::setAppKey(const QString &appkey)
{
    m_sAppKey = appkey;
}

QString TranslationConfig::appKey() const
{
    return m_sAppKey;
}

void TranslationConfig::setSourceLanguage(LanguageType language)
{
    m_eSourceLanguage = language;
}

LanguageType TranslationConfig::sourceLanguage() const
{
    return m_eSourceLanguage;
}

void TranslationConfig::setTargetLanguage(LanguageType language)
{
    m_eTargetLanguage = language;
}

LanguageType TranslationConfig::targetLanguage() const
{
    return m_eTargetLanguage;
}

bool TranslationConfig::isValid()
{
    if(m_eTranslatorEngine <= TranslationEngine_eNone)
    {
        m_sError = QObject::tr("translator clinet is not set");
        return false;
    }

    if(m_eSourceLanguage < LanguageType_eAuto)
    {
        m_sError = QObject::tr("source language is not set");
        return false;
    }

    if(m_eTargetLanguage <= LanguageType_eAuto)
    {
        m_sError = QObject::tr("target language is not set");
        return false;
    }

    if(m_sAppID.isEmpty())
    {
        m_sError = QObject::tr("appid is not set");
        return false;
    }

    if(m_sAppKey.isEmpty())
    {
        m_sError =  QObject::tr("appkey  is not set");
        return false;
    }
    return true;
}

void TranslationConfig::setHandleFileMode(HandleFileMode mode)
{
    m_eHandleFileMode = mode;
}

TranslationConfig::HandleFileMode TranslationConfig::handleFileMode() const
{
    return m_eHandleFileMode;
}

void TranslationConfig::setNewFileSuffix(const QString &suffix)
{
    m_sNewFileSuffix = suffix;
}

QString TranslationConfig::newFileSuffix() const
{
    return m_sNewFileSuffix;
}

void TranslationConfig::setRetranslationFinishedMessage(bool b)
{
    m_bRetranslationFinishedMessage = b;
}

bool TranslationConfig::retranslationFinishedMessage() const
{
    return m_bRetranslationFinishedMessage;
}

QStringList TranslationConfig::availableTranslationEngineList()
{
    //TODO：
    QStringList engineList;
    engineList.append(QObject::tr("Baidu"));
    engineList.append(QObject::tr("Sougou"));
    engineList.append(QObject::tr("Tengxun"));
    return engineList;
}

QStringList TranslationConfig::availableTranslationLanguageList()
{
    //TODO:
    QStringList languageList;
    languageList.append(QObject::tr("Auto"));
    languageList.append(QObject::tr("Chinese_Simplified"));
    languageList.append(QObject::tr("Chinese_Traditional"));
    languageList.append(QObject::tr("English"));
    languageList.append(QObject::tr("Japanese"));
    return languageList;
}


QString TranslationConfig::errorString() const
{
    return m_sError;
}

