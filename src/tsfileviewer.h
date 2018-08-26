#ifndef TSFILEVIEWER_H
#define TSFILEVIEWER_H

//Qt
#include <QLabel>
#include <QWidget>
#include <QSplitter>
#include <QCheckBox>
#include <QShowEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPlainTextEdit>

#include "codeeditor.h"
#include "xmlsyntaxhighlighter.h"

class TsFileViewer : public QWidget
{
    Q_OBJECT
public:
    explicit TsFileViewer(QWidget *parent = 0);

    void setSourceFile(const QString &);
    QString sourceFile() const;

    void setTargetFile(const QString &);
    QString targetFile() const;

    void updateViewer();

protected:
    void showEvent( QShowEvent *);

private slots:
    void slotSave();
    void slotReplace();
    void slotSyncReadStateChange(int);
    void slotScrollBarValueChange(int);

private:
    //Data
    enum SplitterType
    {
        SplitterType_eSource,
        SplitterType_eTarget
    };

    QString m_sSourceFile;
    QString m_sTargetFile;

    QFrame *m_frmSource;
    QLabel *m_lblSourceFile;
    QPlainTextEdit *m_txtEditSource;
    QVBoxLayout *m_layoutSource;

    QFrame *m_frmTarget;
    QLabel *m_lblTargetFile;
    QPlainTextEdit *m_txtEditTarget;
    QVBoxLayout *m_layoutTarget;

    QSplitter *m_splitterSourceTarget;

    QCheckBox *m_chbSyncReading;
    QPushButton *m_btnReplace;
    QPushButton *m_btnSave;
    QHBoxLayout *m_layoutTool;

    QVBoxLayout *m_layoutMain;

    //Function
    void initGui();

    bool saveTargetToFile(const QString &file,QString &error);
};

#endif // TSFILEVIEWER_H
