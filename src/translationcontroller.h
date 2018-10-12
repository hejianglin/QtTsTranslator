#ifndef TRANSLATIONCONTROLLER_H
#define TRANSLATIONCONTROLLER_H

//Qt
#include <QObject>

//QtTsTranslator
#include "translationconfig.h"
#include "tstranslator_global.h"

class TranslationController : public QObject
{
    Q_OBJECT
public:
    explicit TranslationController(QObject *parent = 0);
    ~TranslationController();

    void setTranslationConfig(const TranslationConfig &);
    TranslationConfig translationConfig() const;

    void setFile(const QString &);
    void setFile(const QStringList &);

    bool isValid();
    QString errorString() const;

public slots:
    bool translate();
    bool translate(const QString &);
    bool translate(const QStringList &);

signals:
    void translationProgress(qreal);
    void finished();
    void finishedAFile(const QString &file,bool bSuc);

    void addLog(const QString &log,LogType logType = LogType_eInfo);

private slots:
    void slotTranslationProgress(quint32,quint32);
    void slotFinishedAFile();
private:
    //data
    struct ProgressInfo
    {
        quint32 finished;
        quint32 total;

        ProgressInfo()
            : finished(0)
            , total(0)
        {}

        bool isFinished()
        {
            return finished == total;
        }
    };
    TranslationConfig m_cTranslationConfig;

    QString m_sError;
    QStringList m_slFile;
    QStringList m_slFile_Translating;
    QHash<QString,ProgressInfo *>m_hashProgressInfo;

    //function
    void clear();
    void updateProgress();
    void translate_impl(int translationCount = 1);
};

#endif // TRANSLATIONCONTROLLER_H
