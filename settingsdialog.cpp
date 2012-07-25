#include <QtGui>

#include "settingsdialog.h"
//
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QTabWidget *tabWidget = new QTabWidget;
    generalSettingTab = new GeneralSettingTab;
    textSettingTab = new TextSettingTab;
    tabWidget->addTab(generalSettingTab, tr("General"));
    tabWidget->addTab(textSettingTab, tr("Text"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Settings Dialog"));
}

void SettingsDialog::accept()
{
    generalSettingTab->accept();
    textSettingTab->accept();
    QDialog::accept();
}


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



GeneralSettingTab::GeneralSettingTab(QWidget *parent)
    : QWidget(parent)
{


    //->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QGroupBox *generalBox = new QGroupBox(tr("General :"));
    QVBoxLayout *generalBoxLayout = new QVBoxLayout;

    QHBoxLayout *langLayout = new QHBoxLayout;
    QLabel *langLabel = new QLabel(tr("Change your language :"));
    langComboBox = new QComboBox();
    langs << "Français" << "English";
    langCodes << "fr_FR" << "en_US";
    langComboBox->addItems(langs);
    langLayout->addWidget(langLabel);
    langLayout->addWidget(langComboBox);


    QHBoxLayout *styleLayout = new QHBoxLayout;
    QLabel *styleLabel = new QLabel(tr("Change Plume style :"));
    styleComboBox = new QComboBox();
    styles <<  tr("System default") << tr("Plastique (KDE)") << tr("Cleanlooks");
#ifdef Q_OS_LINUX
    styles << tr("Gtk (Gnome/XFCE)");
#endif
#ifdef Q_OS_WIN32
    styles  << tr("Windows Vista") << tr("Windows XP");
#endif
#ifdef Q_OS_MAC
    styles << tr("Macintosh (OSX)");
#endif
    styleCodes << "default" << "plastique" << "cleanlooks";
#ifdef Q_OS_LINUX
    styleCodes << "gtk";
#endif
#ifdef Q_OS_WIN32
    styleCodes << "vista" << "xp";
#endif
#ifdef Q_OS_MAC
    styleCodes << "osx";
#endif
    styleComboBox->addItems(styles);
    styleLayout->addWidget(styleLabel);
    styleLayout->addWidget(styleComboBox);

    menuBarOnTopCheckBox = new QCheckBox(tr("Set the menu bar on top"));
    menuBarOnTopCheckBox->setToolTip(tr("Menu is set horizontaly on top or verticaly on the right side"));

    QHBoxLayout *autosaveLayout = new QHBoxLayout;
    QLabel *autosaveLabel = new QLabel(tr("Save project every :"));
    autosaveTimeSpin = new QSpinBox;
    autosaveTimeSpin->setRange(10,3600);
    autosaveTimeSpin->setSingleStep(10);
    autosaveTimeSpin->setAccelerated(true);
    autosaveTimeSpin->setSuffix(tr(" s"));
    autosaveLayout->addWidget(autosaveLabel);
    autosaveLayout->addWidget(autosaveTimeSpin);

    checkUpdateAtStartupCheckBox = new QCheckBox(tr("Check update at startup"));

    preventDoubleSpaceCheckBox = new QCheckBox(tr("Prevent multiple space characters between words"));


    generalBoxLayout->addLayout(langLayout);
    generalBoxLayout->addLayout(styleLayout);
    generalBoxLayout->addLayout(autosaveLayout);
    generalBoxLayout->addWidget(menuBarOnTopCheckBox);
    generalBoxLayout->addWidget(checkUpdateAtStartupCheckBox);
    generalBoxLayout->addWidget(preventDoubleSpaceCheckBox);
    generalBoxLayout->addStretch();
    generalBox->setLayout(generalBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(generalBox);
    setLayout(mainLayout);

    readSettings();

    connect(langComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(langChanged()), Qt::UniqueConnection);
    connect(styleComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(styleChanged()), Qt::UniqueConnection);
    connect(menuBarOnTopCheckBox,SIGNAL(toggled(bool)), this, SLOT(menuPositionChanged(bool)), Qt::UniqueConnection);
}

//---------------------------------------------------------------------------------

void GeneralSettingTab::langChanged()
{
    QMessageBox msgBox;
    msgBox.setText(tr("A different language has been selected.<br>The change will be effective after restarting the program."));
    msgBox.exec();
}

//---------------------------------------------------------------------------------

void GeneralSettingTab::styleChanged()
{
    QMessageBox msgBox;
    msgBox.setText(tr("A different style has been selected.<br>The change will be effective after restarting the program."));
    msgBox.exec();
}

//---------------------------------------------------------------------------------
void GeneralSettingTab::menuPositionChanged(bool menuOnTopBool)
{
    if(menuOnTopBool == false){
    QMessageBox msgBox;
    msgBox.setText(tr("The menu will be set on the side.<br>The change will be effective after restarting the program."));
    msgBox.exec();
    }
}

//---------------------------------------------------------------------------------

void GeneralSettingTab::readSettings()
{
    settings.beginGroup("MainWindow");
    langComboBox->setCurrentIndex(langCodes.indexOf(settings.value("lang", "en_US").toString()));
    styleComboBox->setCurrentIndex(styleCodes.indexOf(settings.value("style", "default").toString()));
    menuBarOnTopCheckBox->setChecked(settings.value("menuBarOnTop", true).toBool());
         settings.endGroup();
    settings.beginGroup( "Settings" );
    autosaveTime = settings.value("autosaveTime", 20000).toInt();
    autosaveTimeSpin->setValue(autosaveTime / 1000);
    preventDoubleSpaceCheckBox->setChecked(settings.value("preventDoubleSpace", false).toBool());
    settings.endGroup();
    settings.beginGroup("Updater");
    checkUpdateAtStartupCheckBox->setChecked(settings.value("checkAtStartup", true).toBool());
    settings.endGroup();

}

//---------------------------------------------------------------------------------

void GeneralSettingTab::accept()
{
    settings.beginGroup("MainWindow");
    settings.setValue("lang", langCodes.at(langs.indexOf(langComboBox->currentText())));
    settings.setValue("style", styleCodes.at(styles.indexOf(styleComboBox->currentText())));
    settings.setValue("menuBarOnTop", menuBarOnTopCheckBox->isChecked());
          settings.endGroup();
    settings.beginGroup( "Settings" );
    settings.setValue("autosaveTime", autosaveTimeSpin->value() * 1000);
    settings.setValue("preventDoubleSpace", preventDoubleSpaceCheckBox->isChecked());
    settings.endGroup();
    settings.beginGroup("Updater");
    settings.setValue("checkAtStartup", checkUpdateAtStartupCheckBox->isChecked());
    settings.endGroup();



}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



TextSettingTab::TextSettingTab(QWidget *parent)
    : QWidget(parent)
{

    QGroupBox *textBox = new QGroupBox(tr("Main Text Area :"));
    QGridLayout *gridTextLayout = new QGridLayout;
    QLabel *textFontLabel = new QLabel(tr("Default Text Font :"));
    textFontCombo = new QFontComboBox;
    textFontCombo->setCurrentFont(textFont);
    textSpin = new QSpinBox;
    textSpin->setRange(6,30);
    QCheckBox *textApplyWideBox = new QCheckBox(tr("Apply project wide"));
    showScrollbarBox = new QCheckBox(tr("Show scrollbar"));
    QLabel *textIndentLabel = new QLabel(tr("Paragraph Indent :"));
    textIndentSpin = new QSpinBox;
    textIndentSpin->setRange(0,500);
    QLabel *textMarginLabel = new QLabel(tr("Margin :"));
    textMarginSpin = new QSpinBox;
    textMarginSpin->setRange(0,50);




    gridTextLayout->addWidget(textFontLabel,0,0);
    gridTextLayout->addWidget(textFontCombo,0,1);
    gridTextLayout->addWidget(textSpin,0,2);
    //    gridTextLayout->addWidget(textApplyWideBox,0,3);
    gridTextLayout->addWidget(showScrollbarBox,1,0);
    gridTextLayout->addWidget(textIndentLabel,2,0);
    gridTextLayout->addWidget(textIndentSpin,2,1);
    gridTextLayout->addWidget(textMarginLabel,3,0);
    gridTextLayout->addWidget(textMarginSpin,3,1);
    textBox->setLayout(gridTextLayout);



    // Fullscreen text :

    QGroupBox *fullTextBox = new QGroupBox(tr("FullScreen Text Area :"));

    QGridLayout *gridFullTextLayout = new QGridLayout;

    showFullScrollbarBox = new QCheckBox(tr("Show scrollbar"));

    gridFullTextLayout->addWidget(showFullScrollbarBox,0,0);
    fullTextBox->setLayout(gridFullTextLayout);



    // Synopsis :



    QGroupBox *synBox = new QGroupBox(tr("Synopsis Area :"));
    QGridLayout *gridSynLayout = new QGridLayout;
    QLabel *synFontLabel = new QLabel(tr("Default Text Font :"));
    synFontCombo = new QFontComboBox;
    synFontCombo->setCurrentFont(synFont);
    synSpin = new QSpinBox;
    synSpin->setRange(6,30);
    QCheckBox *synApplyWideBox = new QCheckBox(tr("Apply project wide"));
    synShowScrollbarBox = new QCheckBox(tr("Show scrollbar"));
    QLabel *synIndentLabel = new QLabel(tr("Paragraph Indent :"));
    synIndentSpin = new QSpinBox;
    synIndentSpin->setRange(0,500);
    QLabel *synMarginLabel = new QLabel(tr("Margin :"));
    synMarginSpin = new QSpinBox;
    synMarginSpin->setRange(0,50);




    gridSynLayout->addWidget(synFontLabel,0,0);
    gridSynLayout->addWidget(synFontCombo,0,1);
    gridSynLayout->addWidget(synSpin,0,2);
    //    gridSynLayout->addWidget(synApplyWideBox,0,3);
    gridSynLayout->addWidget(synShowScrollbarBox,1,0);
    gridSynLayout->addWidget(synIndentLabel,2,0);
    gridSynLayout->addWidget(synIndentSpin,2,1);
    gridSynLayout->addWidget(synMarginLabel,3,0);
    gridSynLayout->addWidget(synMarginSpin,3,1);
    synBox->setLayout(gridSynLayout);



    // Note :

    QGroupBox *noteBox = new QGroupBox(tr("Note Area :"));
    QGridLayout *gridNoteLayout = new QGridLayout;
    QLabel *noteFontLabel = new QLabel(tr("Default Text Font :"));
    noteFontCombo = new QFontComboBox;
    noteFontCombo->setCurrentFont(noteFont);
    noteSpin = new QSpinBox;
    noteSpin->setRange(6,30);
    QCheckBox *noteApplyWideBox = new QCheckBox(tr("Apply project wide"));
    noteShowScrollbarBox = new QCheckBox(tr("Show scrollbar"));
    QLabel *noteIndentLabel = new QLabel(tr("Paragraph Indent :"));
    noteIndentSpin = new QSpinBox;
    noteIndentSpin->setRange(0,500);
    QLabel *noteMarginLabel = new QLabel(tr("Margin :"));
    noteMarginSpin = new QSpinBox;
    noteMarginSpin->setRange(0,50);



    gridNoteLayout->addWidget(noteFontLabel,0,0);
    gridNoteLayout->addWidget(noteFontCombo,0,1);
    gridNoteLayout->addWidget(noteSpin,0,2);
    //    gridNoteLayout->addWidget(noteApplyWideBox,0,3);
    gridNoteLayout->addWidget(noteShowScrollbarBox,1,0);
    gridNoteLayout->addWidget(noteIndentLabel,2,0);
    gridNoteLayout->addWidget(noteIndentSpin,2,1);
    gridNoteLayout->addWidget(noteMarginLabel,3,0);
    gridNoteLayout->addWidget(noteMarginSpin,3,1);
    noteBox->setLayout(gridNoteLayout);










    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    mainLayout->addWidget(fullTextBox);
    mainLayout->addWidget(synBox);
    mainLayout->addWidget(noteBox);
    setLayout(mainLayout);

    readSettings();

}

//---------------------------------------------------------------------------------

void TextSettingTab::readSettings()
{
    settings.beginGroup( "Settings" );
    textSpinValue = settings.value("TextArea/textHeight", 12).toInt();
    textFont.setFamily(settings.value("TextArea/textFontFamily", "Liberation Serif").toString());
    showScrollbarBox->setChecked(settings.value("TextArea/showScrollbar", true).toBool());
    textIndentValue = settings.value("TextArea/textIndent", 20).toInt();
    textMarginValue = settings.value("TextArea/bottomMargin", 10).toInt();

    textSpin->setValue(textSpinValue);
    textFontCombo->setCurrentFont(textFont);
    textIndentSpin->setValue(textIndentValue);
    textMarginSpin->setValue(textMarginValue);

    // Fullscreen text :

    showFullScrollbarBox->setChecked(settings.value("FullTextArea/showScrollbar", false).toBool());

    // Synopsis :

    synSpinValue = settings.value("SynArea/textHeight", 12).toInt();
    synFont.setFamily(settings.value("SynArea/textFontFamily", "Liberation Serif").toString());
    synShowScrollbarBox->setChecked(settings.value("SynArea/showScrollbar", true).toBool());
    synIndentValue = settings.value("SynArea/textIndent", 20).toInt();
    synMarginValue = settings.value("SynArea/bottomMargin", 10).toInt();

    synSpin->setValue(synSpinValue);
    synFontCombo->setCurrentFont(synFont);
    synIndentSpin->setValue(synIndentValue);
    synMarginSpin->setValue(synMarginValue);

    // Note :

    noteSpinValue = settings.value("NoteArea/textHeight", 12).toInt();
    noteFont.setFamily(settings.value("NoteArea/textFontFamily", "Liberation Serif").toString());
    noteShowScrollbarBox->setChecked(settings.value("NoteArea/showScrollbar", true).toBool());
    noteIndentValue = settings.value("NoteArea/textIndent", 20).toInt();
    noteMarginValue = settings.value("NoteArea/bottomMargin", 10).toInt();

    noteSpin->setValue(noteSpinValue);
    noteFontCombo->setCurrentFont(noteFont);
    noteIndentSpin->setValue(noteIndentValue);
    noteMarginSpin->setValue(noteMarginValue);

    settings.endGroup();

}

//---------------------------------------------------------------------------------

void TextSettingTab::accept()
{
    settings.beginGroup( "Settings" );
    settings.setValue("TextArea/textHeight", textSpin->value());
    settings.setValue("TextArea/textFontFamily", textFontCombo->currentFont());
    settings.setValue("TextArea/showScrollbar", showScrollbarBox->isChecked());
    settings.setValue("TextArea/textIndent", textIndentSpin->value());
    settings.setValue("TextArea/bottomMargin", textMarginSpin->value());

    // Fullscreen text :


    settings.setValue("FullTextArea/showScrollbar", showFullScrollbarBox->isChecked());


    // Synopsis :

    settings.setValue("SynArea/textHeight", synSpin->value());
    settings.setValue("SynArea/textFontFamily", synFontCombo->currentFont());
    settings.setValue("SynArea/showScrollbar", synShowScrollbarBox->isChecked());
    settings.setValue("SynArea/textIndent", synIndentSpin->value());
    settings.setValue("SynArea/bottomMargin", synMarginSpin->value());

    // Note :

    settings.setValue("NoteArea/textHeight", noteSpin->value());
    settings.setValue("NoteArea/textFontFamily", noteFontCombo->currentFont());
    settings.setValue("NoteArea/showScrollbar", noteShowScrollbarBox->isChecked());
    settings.setValue("NoteArea/textIndent", noteIndentSpin->value());
    settings.setValue("NoteArea/bottomMargin", noteMarginSpin->value());

    settings.endGroup();

    //   qDebug()<< "textFont" << textFont.family();







}
