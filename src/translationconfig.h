#ifndef TRANSLATIONCONFIG_H
#define TRANSLATIONCONFIG_H

//Qt
#include <QString>

//QtTsTranslator
#include "networktranslator_global.h"

//namespace
USE_NETWORKTRANSLATOR_NAMESPACE

class TranslationConfig
{
public:
    enum HandleFileMode
    {
        HandleFileMode_eNew,
        HandleFileMode_eReplace
    };


    TranslationConfig();
    TranslationConfig(const TranslationConfig &other);
    TranslationConfig &operator =(const TranslationConfig &other);

    void loadFromFile();
    void saveToFile();

    bool isValid();
    QString errorString() const;

    void setTranslatorEngine(TranslationEngine engine);
    TranslationEngine translatorEngine() const;

    void setAppID(const QString &appid);
    QString appID() const;
    void setAppKey(const QString &appkey);
    QString appKey() const;

    void setSourceLanguage(LanguageType language);
    LanguageType sourceLanguage() const;

    void setTargetLanguage(LanguageType language);
    LanguageType targetLanguage() const;

    void setHandleFileMode(HandleFileMode mode);
    HandleFileMode handleFileMode() const;

    void setNewFileSuffix(const QString &);
    QString newFileSuffix() const;

    void setRetranslationFinishedMessage(bool b);
    bool retranslationFinishedMessage() const;

    static QStringList availableTranslationEngineList();
    static QStringList availableTranslationLanguageList();

private:
    bool m_bRetranslationFinishedMessage;
    TranslationEngine m_eTranslatorEngine;
    LanguageType m_eSourceLanguage;
    LanguageType m_eTargetLanguage;
    HandleFileMode m_eHandleFileMode;
    QString m_sAppID;
    QString m_sAppKey;
    QString m_sError;
    QString m_sNewFileSuffix;
};



#endif // TRANSLATIONCONFIG_H
