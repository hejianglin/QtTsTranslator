//Qt
#include <QFile>

#include <QScrollBar>
#include <QMessageBox>

//QtTsTranslator
#include "tsfileviewer.h"


TsFileViewer::TsFileViewer(QWidget *parent) : QWidget(parent)
{
    initGui();
}

void TsFileViewer::initGui()
{
    //source
    m_lblSourceFile = new QLabel(this);
    m_txtEditSource = new CodeEditor(this);
    m_txtEditSource->setReadOnly(true);
    new XmlSyntaxHighlighter(m_txtEditSource->document());

    m_frmSource = new QFrame(this);
    m_layoutSource = new QVBoxLayout;
    m_layoutSource->setContentsMargins(0,0,0,0);
    m_layoutSource->addWidget(m_lblSourceFile);
    m_layoutSource->addWidget(m_txtEditSource);
    m_frmSource->setLayout(m_layoutSource);

    //target
    m_lblTargetFile = new QLabel(this);
    m_txtEditTarget = new CodeEditor(this);
    new XmlSyntaxHighlighter(m_txtEditTarget->document());

    m_frmTarget = new QFrame(this);
    m_layoutTarget = new QVBoxLayout;
    m_layoutTarget->setContentsMargins(0,0,0,0);
    m_layoutTarget->addWidget(m_lblTargetFile);
    m_layoutTarget->addWidget(m_txtEditTarget);
    m_frmTarget->setLayout(m_layoutTarget);

    m_splitterSourceTarget = new QSplitter(Qt::Horizontal,this);
    m_splitterSourceTarget->addWidget(m_frmSource);
    m_splitterSourceTarget->addWidget(m_frmTarget);


    //tool
    m_chbSyncReading = new QCheckBox(tr("Cursor Synchronization"),this);
    connect(m_chbSyncReading,SIGNAL(stateChanged(int)),this,SLOT(slotSyncReadStateChange(int)));
    m_chbSyncReading->setChecked(false);

    m_btnReplace = new QPushButton(tr("Replace Source File"),this);
    m_btnReplace->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(m_btnReplace,SIGNAL(clicked()),this,SLOT(slotReplace()));

    m_btnSave = new QPushButton(tr("Save"),this);
    m_btnSave->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(m_btnSave,SIGNAL(clicked()),this,SLOT(slotSave()));

    m_layoutTool = new QHBoxLayout;
    m_layoutTool->setContentsMargins(0,0,0,0);
    m_layoutTool->addWidget(m_chbSyncReading);
    m_layoutTool->addStretch();
    m_layoutTool->addWidget(m_btnReplace);
    m_layoutTool->addWidget(m_btnSave);

    m_layoutMain = new QVBoxLayout;
    m_layoutMain->setContentsMargins(8,8,8,8);
    m_layoutMain->addWidget(m_splitterSourceTarget);
    m_layoutMain->addLayout(m_layoutTool);
    this->setLayout(m_layoutMain);
}

void TsFileViewer::setSourceFile(const QString &file)
{
    m_sSourceFile = file;
    //m_lblSourceFile->setText(file);
}

void TsFileViewer::setTargetFile(const QString &file)
{
    m_sTargetFile = file;
    //m_lblTargetFile->setText(file);
}

void TsFileViewer::showEvent(QShowEvent *)
{
    updateViewer();
}

void TsFileViewer::slotSyncReadStateChange(int)
{
    QScrollBar *sourceBar = m_txtEditSource->verticalScrollBar();
    QScrollBar *targetBar = m_txtEditTarget->verticalScrollBar();
    if(!sourceBar || !targetBar){
        return;
    }

    if(m_chbSyncReading->checkState() == Qt::Checked){
        connect(sourceBar,SIGNAL(valueChanged(int)),this,SLOT(slotScrollBarValueChange(int)));
        connect(targetBar,SIGNAL(valueChanged(int)),this,SLOT(slotScrollBarValueChange(int)));
    }else{
        disconnect(sourceBar,SIGNAL(valueChanged(int)),this,SLOT(slotScrollBarValueChange(int)));
        disconnect(targetBar,SIGNAL(valueChanged(int)),this,SLOT(slotScrollBarValueChange(int)));
    }
}

void TsFileViewer::slotScrollBarValueChange(int value)
{
    QScrollBar *bar = qobject_cast<QScrollBar *>(sender());
    if(bar == Q_NULLPTR){
        return ;
    }

    ///!TODO: Slow and inaccurate
    //source scroll bar value change need change target scroll bar
    if(bar == m_txtEditSource->verticalScrollBar()){
        m_txtEditTarget->verticalScrollBar()->setValue(value);
    }

    //other ...
    if(bar == m_txtEditTarget->verticalScrollBar()){
        m_txtEditSource->verticalScrollBar()->setValue(value);
    }
}

void TsFileViewer::slotSave()
{
    return saveTargetToFile(m_sTargetFile);
}

void TsFileViewer::slotReplace()
{
    return saveTargetToFile(m_sSourceFile);
}

void TsFileViewer::saveTargetToFile(const QString &filepath)
{
    QFile file(filepath);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::critical(this,tr("Error"),tr("open file(%1) error:%2!")
                              .arg(filepath).arg(file.errorString()));
        return ;
    }

    file.write(m_txtEditTarget->toPlainText().toUtf8());
    file.close();
    QMessageBox::information(this,tr("Success"),tr("save to file(%1) success!").arg(filepath));
}

void TsFileViewer::updateViewer()
{
    //same file , hide source file widget
    bool hideSourceWidget = false;
    if(m_sSourceFile == m_sTargetFile){
        hideSourceWidget = true;
    }

    m_splitterSourceTarget->widget(SplitterType_eSource)->setVisible(!hideSourceWidget);
    m_chbSyncReading->setVisible(!hideSourceWidget);
    m_btnReplace->setVisible(!hideSourceWidget);
    m_lblSourceFile->setText(m_sSourceFile);
    m_lblTargetFile->setText(m_sTargetFile);

    //clear context
    m_txtEditSource->clear();
    m_txtEditTarget->clear();

    //load file
    if(!hideSourceWidget){
        QFile sourceFile(m_sSourceFile);
        if(sourceFile.open(QFile::ReadOnly)){
            m_txtEditSource->setPlainText(QString(sourceFile.readAll()));
            sourceFile.close();
        }
    }

    QFile targetFile(m_sTargetFile);
    if(targetFile.open(QFile::ReadOnly)){
        m_txtEditTarget->setPlainText(QString(targetFile.readAll()));
        targetFile.close();
    }

}
