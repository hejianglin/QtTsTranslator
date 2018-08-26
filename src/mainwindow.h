#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt
#include <QMainWindow>

//QtTsTranslator
#include "tsfileviewer.h"
#include "translationconfig.h"
#include "translationcontroller.h"

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //Data
    QAction *m_actOpenFile;
    QAction *m_actOpenDir;

    QLabel *m_lblTips;
    QLabel *m_lblProgress;
    QAction *m_actStart;
    QAction *m_actPause;

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

private slots:
    void slotOpenFile();
    void slotOpenDir();
    void slotSettings();
    void slotStart();
    void slotPause();
    void slotAbout();

    void slotTranslationFinished();
    void slotTranslationFinished(const QString &,bool);
    void slotTranslationProgress(qreal);
};

#endif // MAINWINDOW_H
