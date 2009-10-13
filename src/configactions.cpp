//
// C++ Implementation: configactions
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configactions.h"

K_PLUGIN_FACTORY( ConfigActionsFactory, registerPlugin<ConfigActions>(); )
K_EXPORT_PLUGIN( ConfigActionsFactory( "kcm_kshowmailconfigactions" ) )

ConfigActions::ConfigActions( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigActionsFactory::componentData(), parent, args )
{
  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this );

  //two boxes to seperate the action if new mail or if no new mail
  QGroupBox* gboxNewMails = new QGroupBox( i18n( "Action if new &mail" ), this );
  QGroupBox* gboxNoNewMails = new QGroupBox( i18n( "Action if &no mail" ), this );
  layMain->addWidget( gboxNewMails );
  layMain->addWidget( gboxNoNewMails );

  //layouts for the upper group box
  QVBoxLayout* layMainNewMails = new QVBoxLayout();
  QGridLayout* layMainNewMailsTop = new QGridLayout();
  QGridLayout* layMainNewMailsDown = new QGridLayout();
  layMainNewMails->addLayout( layMainNewMailsTop );
  layMainNewMails->addLayout( layMainNewMailsDown );
  gboxNewMails->setLayout( layMainNewMails );


  //items for the upper group box
  chkNewMailsAlertWindow = new QCheckBox( i18n( "Show message box" ), gboxNewMails );
  chkNewMailsAlertWindow->setToolTip( i18n( "Show message if new mail arrives" ) );
  layMainNewMailsTop->addWidget( chkNewMailsAlertWindow, 0, 0 );

  chkNewMailsMainWindow = new QCheckBox( i18n( "Show main window" ), gboxNewMails );
  chkNewMailsMainWindow->setToolTip( i18n( "Show main window if new mail arrives" ) );
  layMainNewMailsTop->addWidget( chkNewMailsMainWindow, 0, 1 );

  chkNewMailsBeep = new QCheckBox( i18n( "&Beep" ), gboxNewMails );
  chkNewMailsBeep->setToolTip( i18n( "Beeps the internal speaker if new mail" ) );
  layMainNewMailsTop->addWidget( chkNewMailsBeep, 1, 0 );

  chkNewMailsSound = new QCheckBox( i18n( "Sound:" ), gboxNewMails );
  chkNewMailsSound->setToolTip( i18n( "Plays sound if new mail" ) );
  layMainNewMailsDown->addWidget( chkNewMailsSound, 0, 0 );

  btnNewMailsPlaySound = new KPushButton( KGuiItem( QString(), QString( "player_play" ), i18n( "Play the selected sound file" ), i18n( "Play the selected sound file" ) ), gboxNewMails );
  layMainNewMailsDown->addWidget( btnNewMailsPlaySound, 0, 1 );

  txtNewMailsSound = new KLineEdit( gboxNewMails );
  layMainNewMailsDown->addWidget( txtNewMailsSound, 0, 2 );

  btnNewMailsChooseSound = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Press to select sound file" ), i18n( "Press to select sound file" ) ), gboxNewMails );
  layMainNewMailsDown->addWidget( btnNewMailsChooseSound, 0, 3 );

  chkNewMailsCommand = new QCheckBox( i18n( "Command:" ), gboxNewMails );
  chkNewMailsCommand->setToolTip( i18n( "Starts external program if new mail" ) );
  layMainNewMailsDown->addWidget( chkNewMailsCommand, 1, 0 );

  btnNewMailsExecCommand = new KPushButton( KGuiItem( QString(), QString( "exec" ), i18n( "Start the selected program" ), i18n( "Start the selected program" ) ), gboxNewMails );
  layMainNewMailsDown->addWidget( btnNewMailsExecCommand, 1, 1 );

  txtNewMailsCommand = new KLineEdit( gboxNewMails );
  layMainNewMailsDown->addWidget( txtNewMailsCommand, 1, 2 );

  btnNewMailsChooseCommand = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Select external command" ), i18n( "Select external command" ) ), gboxNewMails );
  layMainNewMailsDown->addWidget( btnNewMailsChooseCommand, 1, 3 );

  //items for the lower group box
  chkNoNewMailsMinimize = new QCheckBox( i18n( "Minimi&ze" ), gboxNoNewMails );
  chkNoNewMailsMinimize->setToolTip( i18n( "Minimize window if no new mail" ) );

  chkNoNewMailsTerminate = new QCheckBox( i18n( "Terminate" ), gboxNoNewMails );
  chkNoNewMailsTerminate->setToolTip( i18n( "Terminate kshowmail if no new mail" ) );

  //connect file choose buttons
  connect( btnNewMailsChooseSound, SIGNAL( clicked() ), this, SLOT( slotChooseSound() ) );
  connect( btnNewMailsChooseCommand, SIGNAL( clicked() ), this, SLOT( slotChooseCommand() ) );

  //connect test buttons
  connect( btnNewMailsPlaySound, SIGNAL( clicked() ), this, SLOT( slotPlaySound() ) );
  connect( btnNewMailsExecCommand, SIGNAL( clicked() ), this, SLOT( slotExecuteCommand() ) );

  //connect sound and command check boxes to enable or disable the configuration items
  connect( chkNewMailsSound, SIGNAL( toggled( bool ) ), this, SLOT( slotSoundToggled( bool ) ) );
  connect( chkNewMailsCommand, SIGNAL( toggled( bool ) ), this, SLOT( slotCommandToggled( bool ) ) );

  //connect all configuration items with slot changed() to notify the dialog about changes
  connect( chkNewMailsAlertWindow, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkNewMailsMainWindow, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkNewMailsBeep, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkNewMailsSound, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkNewMailsCommand, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( txtNewMailsSound, SIGNAL( textChanged( const QString& ) ), this, SLOT( slotChanged() ) );
  connect( txtNewMailsCommand, SIGNAL( textChanged( const QString& ) ), this, SLOT( slotChanged() ) );
  connect( chkNoNewMailsMinimize, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkNoNewMailsTerminate, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );



  //get application config object (kshowmailrc)
  config = KGlobal::config();

  //load configured values
  load();

}


ConfigActions::~ConfigActions()
{
}

void ConfigActions::load( )
{
  KConfigGroup* configAct = new KConfigGroup( config, CONFIG_GROUP_ACTIONS );


  chkNewMailsAlertWindow->setChecked( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW, DEFAULT_ACTION_NEW_MAIL_ALERTWINDOW ) );
  chkNewMailsMainWindow->setChecked( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_MAINWINDOW, DEFAULT_ACTION_NEW_MAIL_MAINWINDOW ) );
  chkNewMailsBeep->setChecked( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_BEEP, DEFAULT_ACTION_NEW_MAIL_BEEP ) );
  chkNewMailsSound->setChecked( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_SOUND, DEFAULT_ACTION_NEW_MAIL_SOUND ) );
  chkNewMailsCommand->setChecked( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_COMMAND, DEFAULT_ACTION_NEW_MAIL_COMMAND ) );
  chkNoNewMailsTerminate->setChecked( configAct->readEntry( CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE, DEFAULT_ACTION_NO_NEW_MAIL_TERMINATE ) );
  chkNoNewMailsMinimize->setChecked( configAct->readEntry( CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE, DEFAULT_ACTION_NO_NEW_MAIL_MINIMIZE ) );

  txtNewMailsSound->setText( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_SOUNDPATH ) );
  txtNewMailsCommand->setText( configAct->readEntry( CONFIG_ENTRY_NEW_MAIL_COMMANDPATH ) );

  //enable or disable configuration items of sound and command
  slotSoundToggled( chkNewMailsSound->isChecked() );
  slotCommandToggled( chkNewMailsCommand->isChecked() );
}

void ConfigActions::defaults( )
{
  chkNewMailsAlertWindow->setChecked( DEFAULT_ACTION_NEW_MAIL_ALERTWINDOW );
  chkNewMailsMainWindow->setChecked( DEFAULT_ACTION_NEW_MAIL_MAINWINDOW );
  chkNewMailsBeep->setChecked( DEFAULT_ACTION_NEW_MAIL_BEEP );
  chkNewMailsSound->setChecked( DEFAULT_ACTION_NEW_MAIL_SOUND );
  chkNewMailsCommand->setChecked( DEFAULT_ACTION_NEW_MAIL_COMMAND );
  chkNoNewMailsTerminate->setChecked( DEFAULT_ACTION_NO_NEW_MAIL_TERMINATE );
  chkNoNewMailsMinimize->setChecked( DEFAULT_ACTION_NO_NEW_MAIL_MINIMIZE );

  //enable or disable configuration items of sound and command
  slotSoundToggled( chkNewMailsSound->isChecked() );
  slotCommandToggled( chkNewMailsCommand->isChecked() );
}

void ConfigActions::save( )
{
  KConfigGroup* configAct = new KConfigGroup( config, CONFIG_GROUP_ACTIONS );

  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW, chkNewMailsAlertWindow->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_MAINWINDOW, chkNewMailsMainWindow->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_BEEP, chkNewMailsBeep->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_SOUND, chkNewMailsSound->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_SOUNDPATH, txtNewMailsSound->text() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_COMMAND, chkNewMailsCommand->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NEW_MAIL_COMMANDPATH, txtNewMailsCommand->text() );
  configAct->writeEntry( CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE, chkNoNewMailsTerminate->isChecked() );
  configAct->writeEntry( CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE, chkNoNewMailsMinimize->isChecked() );

  //write configuration to disk
  config->sync();
}

void ConfigActions::slotChooseSound( )
{
  QString startPath;    //start path of the file dialog

  //get sounds directory of kshowmail
  KStandardDirs stdDirs;
  QString soundPath = stdDirs.findResource( "data", "kshowmail/sounds/" );

  //get start path of the file dialog
  if( txtNewMailsSound->text() != "" )
  {
    startPath = txtNewMailsSound->text();
  }
  else if( soundPath != QString::null )
  {
    startPath = soundPath;
  }
  else
  {
    startPath = QString::null;
  }

  //open file dialog
  QString path = KFileDialog::getOpenFileName( startPath, "*.wav *.ogg|" + i18n( "Sound files (*.wav, *.ogg)" ) + "\n*.*|" + i18n( "All files (*)" ), this, i18n("Select Sound File") );

  //write selected path into the edit line
  if( path != QString::null )
    txtNewMailsSound->setText( path );
}

void ConfigActions::slotChooseCommand( )
{
  //open file dialog
  QString path = KFileDialog::getOpenFileName( KUrl(), "", this, i18n("Select external command") );

  //write selected path into the edit line
  if( path != QString::null )
    txtNewMailsCommand->setText( path );
}

void ConfigActions::slotPlaySound( )
{
  QString path = txtNewMailsSound->text();

  if( path != QString::null )
  {
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject( this );
    mediaObject->setCurrentSource( Phonon::MediaSource( path ) );
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory, this );
    Phonon::createPath( mediaObject, audioOutput );
    mediaObject->play();

    //delete mediaObject;
    //delete audioOutput;
  }
}

void ConfigActions::slotExecuteCommand( )
{
  QString path = txtNewMailsCommand->text();

  if( path != QString::null )
  {
    KProcess::execute( path );
  }
}

void ConfigActions::slotSoundToggled( bool on )
{
  btnNewMailsPlaySound->setEnabled( on );
  txtNewMailsSound->setEnabled( on );
  btnNewMailsChooseSound->setEnabled( on );
}

void ConfigActions::slotCommandToggled( bool on )
{
  btnNewMailsExecCommand->setEnabled( on );
  txtNewMailsCommand->setEnabled( on );
  btnNewMailsChooseCommand->setEnabled( on );
}

void ConfigActions::slotChanged( )
{
  KCModule::changed();
}


#include "configactions.moc"





