//Qt
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

//QtTsTranslator
#include "translationconfig.h"
#include "translationconfigdialog.h"

//namespace
USE_NETWORKTRANSLATOR_NAMESPACE

TranslationConfigDialog::TranslationConfigDialog(TranslationConfig *config,QWidget *parent)
    : QDialog(parent)
    , m_cTranslationConfig(config)
{
    initGui();
}

void TranslationConfigDialog::initGui()
{
    initEngine();
    initLanguage();
    initFile();
    initTranslation();
    initTool();
    setMainLayout();
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

void TranslationConfigDialog::initEngine()
{
    m_lblEngine = new QLabel(this);
    m_lblEngine->setText(tr("Engine:"));
    m_cboxEngine = new QComboBox(this);

    m_lblAppID = new QLabel(this);
    m_lblAppID->setText(tr("AppID:"));
    m_edtAppID = new QLineEdit(this);

    m_lblAppKey = new QLabel(this);
    m_lblAppKey->setText(tr("AppKey:"));
    m_edtAppKey = new QLineEdit(this);

    m_gboxEngine = new QGroupBox(this);
    m_gboxEngine->setTitle(tr("Engine"));

    m_layoutEngine = new QGridLayout(m_gboxEngine);

    int row = 0;
    m_layoutEngine->addWidget(m_lblEngine,row,0,1,1);
    m_layoutEngine->addWidget(m_cboxEngine,row,1,1,2);

    row++;
    m_layoutEngine->addWidget(m_lblAppID,row,0,1,1);
    m_layoutEngine->addWidget(m_edtAppID,row,1,1,2);

    row++;
    m_layoutEngine->addWidget(m_lblAppKey,row,0,1,1);
    m_layoutEngine->addWidget(m_edtAppKey,row,1,1,2);

    m_gboxEngine->setLayout(m_layoutEngine);
}

void TranslationConfigDialog::initLanguage()
{
    m_lblSourceLanguage = new QLabel(this);
    m_lblSourceLanguage->setText(tr("Source:"));
    m_cboxSourceLanguage = new QComboBox(this);

    m_lblTargetLanguage = new QLabel(this);
    m_lblTargetLanguage->setText(tr("Target:"));
    m_cboxTargetLanguage = new QComboBox(this);

    m_gboxLanguage = new QGroupBox(this);
    m_gboxLanguage->setTitle("Language");

    m_layoutLanguage = new QGridLayout(m_gboxLanguage);

    int row = 0;
    m_layoutLanguage->addWidget(m_lblSourceLanguage,row,0,1,1);
    m_layoutLanguage->addWidget(m_cboxSourceLanguage,row,1,1,2);

    row++;
    m_layoutLanguage->addWidget(m_lblTargetLanguage,row,0,1,1);
    m_layoutLanguage->addWidget(m_cboxTargetLanguage,row,1,1,2);
    m_gboxLanguage->setLayout(m_layoutLanguage);
}

void TranslationConfigDialog::initFile()
{
    m_rboxNewFile = new QRadioButton(this);
    m_rboxNewFile->setText(tr("New same name file but different suffix:"));

    m_edtNewFileSuffix = new QLineEdit(this);
    m_edtNewFileSuffix->setMaxLength(10);

    connect(m_rboxNewFile,SIGNAL(toggled(bool)),m_edtNewFileSuffix,SLOT(setEnabled(bool)));

    m_rboxReplaceFile = new QRadioButton(this);
    m_rboxReplaceFile->setText(tr("Replace File"));

    m_gboxFile = new QGroupBox(this);
    m_gboxFile->setTitle(tr("File ( How to save translation result )"));

    m_layoutFile = new QGridLayout(m_gboxFile);
    int row = 0;
    m_layoutFile->addWidget(m_rboxNewFile,row,0,1,1);
    m_layoutFile->addWidget(m_edtNewFileSuffix,row,1,1,1);
    m_layoutFile->addWidget(m_rboxReplaceFile,++row,0,1,2);

    m_gboxFile->setLayout(m_layoutFile);
}

void TranslationConfigDialog::initTranslation()
{
    m_chboxRetranslationFinished = new QCheckBox(this);
    m_chboxRetranslationFinished->setText(tr("Retranslation finished message"));

    m_gboxTransaltion = new QGroupBox(this);
    m_gboxTransaltion->setTitle(tr("Translation"));

    m_layoutTranslation = new QGridLayout(m_gboxTransaltion);
    m_layoutTranslation->addWidget(m_chboxRetranslationFinished,0,0,1,1);

    m_gboxTransaltion->setLayout(m_layoutTranslation);
}

void TranslationConfigDialog::initTool()
{
    m_btnSave = new QPushButton(this);
    m_btnSave->setText(tr("Save"));
    connect(m_btnSave,SIGNAL(clicked()),this,SLOT(slotSave()));

    m_btnCancel = new QPushButton(this);
    m_btnCancel->setText(tr("Cancel"));
    connect(m_btnCancel,SIGNAL(clicked()),this,SLOT(reject()));


    m_layoutTool = new QHBoxLayout;
    m_layoutTool->setContentsMargins(0,0,0,0);
    m_layoutTool->addStretch();
    m_layoutTool->addWidget(m_btnSave);
    m_layoutTool->addWidget(m_btnCancel);
}

void TranslationConfigDialog::setMainLayout()
{
    m_layoutMain = new QVBoxLayout(this);
    m_layoutMain->setContentsMargins(5,5,5,5);
    m_layoutMain->addSpacing(5);
    m_layoutMain->addWidget(m_gboxEngine);
    m_layoutMain->addSpacing(5);
    m_layoutMain->addWidget(m_gboxLanguage);
    m_layoutMain->addSpacing(5);
    m_layoutMain->addWidget(m_gboxFile);
    m_layoutMain->addSpacing(5);
    m_layoutMain->addWidget(m_gboxTransaltion);
    m_layoutMain->addSpacing(5);
    m_layoutMain->addLayout(m_layoutTool);
    this->setLayout(m_layoutMain);
}

void TranslationConfigDialog::showEvent(QShowEvent *)
{
    //init engine list
    m_cboxEngine->addItems(m_cTranslationConfig->availableTranslationEngineList());

    //init language list
    QStringList slLanguageList = m_cTranslationConfig->availableTranslationLanguageList();
    m_cboxSourceLanguage->addItems(slLanguageList);

    slLanguageList.removeFirst();
    m_cboxTargetLanguage->addItems(slLanguageList);

    //load config
    loadConfig();

    this->move((QApplication::desktop()->width() - this->width())/2,
               (QApplication::desktop()->height() - this->height())/2);
}


void TranslationConfigDialog::loadConfig()
{
    //engine
    m_cboxEngine->setCurrentIndex(static_cast<int>(m_cTranslationConfig->translatorEngine()));
    m_edtAppID->setText(m_cTranslationConfig->appID());
    m_edtAppKey->setText(m_cTranslationConfig->appKey());

    //language
    m_cboxSourceLanguage->setCurrentIndex(static_cast<int>(m_cTranslationConfig->sourceLanguage()));

    // target list remove auto
    m_cboxTargetLanguage->setCurrentIndex(static_cast<int>(m_cTranslationConfig->targetLanguage()) - 1);

    //file
    m_rboxNewFile->setChecked(m_cTranslationConfig->handleFileMode() == TranslationConfig::HandleFileMode_eNew);
    m_rboxReplaceFile->setChecked(!m_rboxNewFile->isChecked());
    m_edtNewFileSuffix->setText(m_cTranslationConfig->newFileSuffix());

    //translation
    m_chboxRetranslationFinished->setCheckState(m_cTranslationConfig->retranslationFinishedMessage() ?
                                                    Qt::Checked : Qt::Unchecked);
}

void TranslationConfigDialog::slotSave()
{
    if(!isValid()){
        QMessageBox::critical(this,tr("Error"),m_sError);
        return ;
    }

    //engine
    m_cTranslationConfig->setTranslatorEngine(static_cast<TranslationEngine>(m_cboxEngine->currentIndex()));
    m_cTranslationConfig->setAppID(m_edtAppID->text());
    m_cTranslationConfig->setAppKey(m_edtAppKey->text());

    //language
    m_cTranslationConfig->setSourceLanguage(static_cast<LanguageType>(m_cboxSourceLanguage->currentIndex()));
    m_cTranslationConfig->setTargetLanguage(static_cast<LanguageType>(m_cboxTargetLanguage->currentIndex() + 1));

    //file
    m_cTranslationConfig->setHandleFileMode(m_rboxNewFile->isChecked() ? TranslationConfig::HandleFileMode_eNew \
                                                                       : TranslationConfig::HandleFileMode_eReplace);
    if(m_rboxNewFile->isChecked()){
        m_cTranslationConfig->setNewFileSuffix(m_edtNewFileSuffix->text());
    }

    //translation
    m_cTranslationConfig->setRetranslationFinishedMessage(m_chboxRetranslationFinished->isChecked());

    m_cTranslationConfig->saveToFile();
    QMessageBox::information(this,tr("Success"),tr("Save Translation Config Successful!"));
    accept();
}

bool TranslationConfigDialog::isValid()
{
    //engine
    if(m_edtAppID->text().isEmpty()){
        m_sError = tr("appid is not set");
        return false;
    }

    if(m_edtAppKey->text().isEmpty()){
        m_sError = tr("appkey is not set");
        return false;
    }

    //language
    if(m_cboxSourceLanguage->currentIndex() + 1 == m_cboxTargetLanguage->currentIndex()){
        m_sError = tr("the source language is the same as the target language");
        return false;
    }

    //file
    if(m_rboxNewFile->isChecked() && m_edtNewFileSuffix->text().isEmpty()){
        m_sError = tr("new file suffix is not set");
        return false;
    }

    return true;
}
