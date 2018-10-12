#ifndef TRANSLATIONCONFIGDIALOG_H
#define TRANSLATIONCONFIGDIALOG_H

//Qt
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QShowEvent>
#include <QRadioButton>

class TranslationConfig;


class TranslationConfigDialog : public QDialog
{
    Q_OBJECT
public:
    TranslationConfigDialog(TranslationConfig *config,QWidget *parent  = 0);

protected:
    void showEvent(QShowEvent *);

private slots:
    void slotSave();

private:
    //engine
    QGroupBox *m_gboxEngine;
    QLabel *m_lblEngine;
    QComboBox *m_cboxEngine;
    QLabel *m_lblAppID;
    QLineEdit *m_edtAppID;
    QLabel *m_lblAppKey;
    QLineEdit *m_edtAppKey;
    QGridLayout *m_layoutEngine;

    //language
    QGroupBox *m_gboxLanguage;
    QLabel *m_lblSourceLanguage;
    QComboBox *m_cboxSourceLanguage;
    QLabel *m_lblTargetLanguage;
    QComboBox *m_cboxTargetLanguage;
    QGridLayout *m_layoutLanguage;

    //file
    QGroupBox *m_gboxFile;
    QRadioButton *m_rboxNewFile;
    QLineEdit *m_edtNewFileSuffix;
    QRadioButton *m_rboxReplaceFile;
    QGridLayout *m_layoutFile;

    //translation
    QGroupBox *m_gboxTransaltion;
    QCheckBox *m_chboxRetranslationFinished;
    QGridLayout *m_layoutTranslation;

    //toolbutton
    QPushButton *m_btnSave;
    QPushButton *m_btnCancel;
    QHBoxLayout *m_layoutTool;

    QVBoxLayout *m_layoutMain;

    QString m_sError;

    //settings
    TranslationConfig *m_cTranslationConfig;

    void initGui();
    void initEngine();
    void initLanguage();
    void initFile();
    void initTranslation();
    void initTool();
    void setMainLayout();

    void loadConfig();

    bool isValid();
};




#endif // TRANSLATIONCONFIGDIALOG_H
