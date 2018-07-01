#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>

#include "tstranslator_global.h"

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = 0);

    void setContext(Context *);
    Context *context() const;

    bool translate();
    bool translate(Context *);

    QString errorString() const;

signals:
    void translatorProgress(qint64 finishedMessage, qint64 totalMessage);
    void finished();

private:
    Context *m_cContext;

    bool isValid();
    void genertorTask();
};

#endif // TRANSLATOR_H
