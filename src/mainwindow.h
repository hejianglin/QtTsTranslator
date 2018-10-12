#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt
#include <QMainWindow>
#include <QProgressDialog>

//QtTsTranslator
#include "tsfileviewer.h"
#include "translationconfig.h"
#include "translationcontroller.h"
#include "tstranslator_global.h"

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //Data
    QString m_sLogDir;

    QAction *m_actOpenFile;
    QAction *m_actOpenDir;
    QAction *m_actSettings;

    QLabel *m_lblTips;
    QLabel *m_lblProgress;
    QAction *m_actStart;
    QAction *m_actPause;

    QProgressDialog *m_proDlgWaiting;

    QStringList m_slSourceFile;
    QStringList m_slTargetFile;
    TranslationController *m_cTranslationController;
    TranslationConfig m_cTranslationConfig;
    TsFileViewer *m_cTsFileViewer;

    //Function
    void initGui();
    void initMenuBar();
    void initToolBar();
    void initStatusBar();
    void initFileViewer();

    void updateTargetFileList();
    void updateFileViewer();

    void enterBusyMode();
    void quitBusyMode();

private slots:
    void slotOpenFile();
    void slotOpenDir();
    void slotSettings();
    void slotStart();
    void slotPause();
    void slotAbout();

    void slotTranslationFinished();
    void slotTranslationAFileFinished(const QString &,bool);
    void slotTranslationProgress(qreal);

    void slotAddLog(const QString &log,LogType type);
};

#endif // MAINWINDOW_H
