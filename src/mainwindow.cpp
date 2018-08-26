//Qt
#include <QLabel>
#include <QAction>
#include <QDialog>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileSelector>

#include <QMenu>
#include <QMenuBar>

#include <QFile>
#include <QCoreApplication>

//QtTsTranslator
#include "mainwindow.h"
#include "translationconfigdialog.h"

USE_NETWORKTRANSLATOR_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_cTranslationController(new TranslationController(this))
{

    connect(m_cTranslationController,SIGNAL(translationProgress(qreal))
            ,this,SLOT(slotTranslationProgress(qreal)));
    connect(m_cTranslationController,SIGNAL(finished(QString,bool))
            ,this,SLOT(slotTranslationFinished(QString,bool)));
    connect(m_cTranslationController,SIGNAL(finished()),
            this,SLOT(slotTranslationFinished()));

    initGui();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initGui()
{
    initStatusBar();
    initMenuBar();
    initToolBar();
    initFileViewer();
    this->setWindowIcon(QIcon("://image/logo.png"));
    resize(800,600);
}

void MainWindow::initStatusBar()
{
    m_lblTips = new QLabel(this);
    m_lblTips->setText(tr("Ready"));
    m_lblProgress = new QLabel(this);

    QHBoxLayout *layoutStatusBar = new QHBoxLayout;
    layoutStatusBar->setContentsMargins(3,0,3,0);

    layoutStatusBar->addWidget(m_lblTips);
    layoutStatusBar->addStretch();
    layoutStatusBar->addWidget(m_lblProgress);

    QWidget *statusBarWidget = new QWidget(this);
    statusBarWidget->setLayout(layoutStatusBar);

    QStatusBar *statusBar = this->statusBar();
    statusBar->addWidget(statusBarWidget,1);
}

void MainWindow::initMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(0);

    //file
    QMenu *menuFile = menuBar->addMenu(tr("File"));
    m_actOpenFile = menuFile->addAction(QIcon("://image/file.png"),tr("Open File"),
                                        this,SLOT(slotOpenFile()));
    m_actOpenFile->setStatusTip(tr("Open Qt translation file"));

    m_actOpenDir = menuFile->addAction(QIcon("://image/dir.png"),tr("Open Dir"),
                                       this,SLOT(slotOpenDir()));
    m_actOpenDir->setStatusTip(tr("Open include Qt translation file dir"));
    m_actOpenDir->setEnabled(false);

    menuFile->addSeparator();
    menuFile->addAction(tr("Quit(&Q)"),this,SLOT(close()));

    //tool
    QMenu *menuTool = menuBar->addMenu(tr("Tool"));
    menuTool->addAction(tr("Settings"),this,SLOT(slotSettings()));


    //help
    QMenu *menuHelp = menuBar->addMenu(tr("Help"));
    menuHelp->addAction(tr("About"),this,SLOT(slotAbout()));

    this->setMenuBar(menuBar);
}

void MainWindow::initToolBar()
{
    //progress toolbar
    QToolBar *toolBar_Progress = new QToolBar(this);
    toolBar_Progress->setIconSize(QSize(32,32));
    toolBar_Progress->setMovable(false);

    //start
    m_actStart = toolBar_Progress->addAction(QIcon("://image/start.png"),
                                             tr("Start"),this,SLOT(slotStart()));
    m_actStart->setEnabled(false);
    m_actStart->setStatusTip(tr("Start translation"));

    //file toolbar
    QToolBar *toolBar_File = new QToolBar(this);
    toolBar_File->setIconSize(QSize(32,32));
    toolBar_File->setMovable(false);

    toolBar_File->addAction(m_actOpenFile);
    toolBar_File->addAction(m_actOpenDir);

    this->addToolBar(Qt::TopToolBarArea,toolBar_Progress);
    this->addToolBar(Qt::TopToolBarArea,toolBar_File);
    this->insertToolBarBreak(toolBar_Progress);
}

void MainWindow::initFileViewer()
{
    m_cTsFileViewer = new TsFileViewer(this);
    m_cTsFileViewer->hide();

    QFrame *frmMain = new QFrame(this);
    QGridLayout *layoutMain = new QGridLayout(frmMain);
    layoutMain->setContentsMargins(0,0,0,0);
    layoutMain->addWidget(m_cTsFileViewer,0,0,1,1);
    this->setCentralWidget(frmMain);
}


void MainWindow::slotOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Ts File"),
                                                    QDir::homePath(),
                                                    QString("Qt Ts File (*.ts)"));

    if(fileName.simplified().isEmpty()){
        return ;
    }

    m_actStart->setEnabled(true);
    m_slSourceFile.clear();
    m_slSourceFile.append(fileName);

    updateTargetFileList();
    updateFileViewer();
}

void MainWindow::slotOpenDir()
{

}

void MainWindow::updateTargetFileList()
{
    m_slTargetFile.clear();
    foreach(QString sFile,m_slSourceFile){
        m_slTargetFile.append(sFile.append(m_cTranslationConfig.handleFileMode() == \
                                           TranslationConfig::HandleFileMode_eNew ? \
                                               m_cTranslationConfig.newFileSuffix() : QString("")));
    }
}

void MainWindow::updateFileViewer()
{
    if(m_slSourceFile.isEmpty()){
        return ;
    }

    if(m_slSourceFile.count() == 1){
        m_cTsFileViewer->setSourceFile(m_slSourceFile.value(0));
        m_cTsFileViewer->setTargetFile(m_slTargetFile.value(0));
        m_cTsFileViewer->setVisible(true);
        m_cTsFileViewer->updateViewer();
    }
}

void MainWindow::slotAbout()
{
    QDialog *dialogAbout = new QDialog(this);
    dialogAbout->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *lblAboutIcon = new QLabel(dialogAbout);
    lblAboutIcon->setPixmap(QPixmap("://image/logo.png"));
    lblAboutIcon->setAlignment(Qt::AlignCenter);

    QLabel *lblDebugText = new QLabel(dialogAbout);
    lblDebugText->setText(tr("bug report: he.jianglin@foxmail.com"));
    lblDebugText->setAlignment(Qt::AlignCenter);

    QLabel *lblAboutText = new QLabel(dialogAbout);
    lblAboutText->setText(tr("QtTsFileTranslator is used to help you translate Qt ts file."));
    lblAboutText->setAlignment(Qt::AlignCenter);
    QVBoxLayout *layoutAboutDialog = new QVBoxLayout(dialogAbout);
    layoutAboutDialog->addWidget(lblAboutIcon);
    layoutAboutDialog->addWidget(lblDebugText);
    layoutAboutDialog->addWidget(lblAboutText);
    dialogAbout->setLayout(layoutAboutDialog);
    dialogAbout->exec();
}

void MainWindow::slotSettings()
{
    TranslationConfigDialog *dialog = new TranslationConfigDialog(&m_cTranslationConfig,this);
    dialog->exec();
    delete dialog;
}

void MainWindow::slotStart()
{
    m_cTranslationController->setTranslationConfig(m_cTranslationConfig);
    m_cTranslationController->setFile(m_slSourceFile);
    if(!m_cTranslationController->translate()){
        QMessageBox::critical(this,tr("Translation Error"),m_cTranslationController->errorString());
        return ;
    }
}

void MainWindow::slotPause()
{

}

void MainWindow::slotTranslationFinished()
{
    m_lblTips->setText(tr("Ready"));
}

void MainWindow::slotTranslationProgress(qreal progress)
{
    if(progress >= 1){
         DEBUG(QString::number(progress));
        m_lblTips->setText(tr("Translation finished"));
        m_lblProgress->setText("");
    }else{
        DEBUG(QString::number(progress));
        m_lblTips->setText(tr("Translating..."));
        m_lblProgress->setText(QString::number(progress * 100,'f',2) + QString("%"));
    }
}

void MainWindow::slotTranslationFinished(const QString &,bool suc)
{
    if(m_slSourceFile.count() == 1 && suc){
        m_cTsFileViewer->updateViewer();
    }
}
