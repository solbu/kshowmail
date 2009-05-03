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

typedef KGenericFactory<ConfigActions, QWidget> ConfigActionsFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfigactions, ConfigActionsFactory(
    "kcm_kshowmailconfigactions" ) );

ConfigActions::ConfigActions( QWidget * parent, const char * name, const QStringList & args )
  : KCModule( ConfigActionsFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configactions" );

  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //two boxes to seperate the action if new mail or if no new mail
  QGroupBox* gboxNewMails = new QGroupBox( 0, Qt::Horizontal, i18n( "Action if new &mail" ), this, "gboxNewMails" );
  QGroupBox* gboxNoNewMails = new QGroupBox( 2, Qt::Vertical, i18n( "Action if &no mail" ), this, "gboxNoNewMails" );
  gboxNoNewMails->layout()->setSpacing( 20 );
  layMain->addWidget( gboxNewMails );
  layMain->addWidget( gboxNoNewMails );

  //layouts for the upper group box
  QVBoxLayout* layMainNewMails = new QVBoxLayout( gboxNewMails->layout(), 10 );
  QGridLayout* layMainNewMailsTop = new QGridLayout( layMainNewMails, 2, 2, 10 );
  QGridLayout* layMainNewMailsDown = new QGridLayout( layMainNewMails, 2, 4, 10 );

  //items for the upper group box
  chkNewMailsAlertWindow = new QCheckBox( i18n( "Show message box" ), gboxNewMails, "chkNewMailsAlertWindow" );
  QToolTip::add( chkNewMailsAlertWindow, i18n( "Show message if new mail arrives" ) );
  layMainNewMailsTop->addWidget( chkNewMailsAlertWindow, 0, 0 );

  chkNewMailsMainWindow = new QCheckBox( i18n( "Show main window" ), gboxNewMails, "chkNewMailsMainWindow" );
  QToolTip::add( chkNewMailsMainWindow, i18n( "Show main window if new mail arrives" ) );
  layMainNewMailsTop->addWidget( chkNewMailsMainWindow, 0, 1 );

  chkNewMailsBeep = new QCheckBox( i18n( "&Beep" ), gboxNewMails, "chkNewMailsBeep" );
  QToolTip::add( chkNewMailsBeep, i18n( "Beeps the internal speaker if new mail" ) );
  layMainNewMailsTop->addWidget( chkNewMailsBeep, 1, 0 );

  chkNewMailsSound = new QCheckBox( i18n( "Sound:" ), gboxNewMails, "chkNewMailsSound" );
  QToolTip::add( chkNewMailsSound, i18n( "Plays sound if new mail" ) );
  layMainNewMailsDown->addWidget( chkNewMailsSound, 0, 0 );

  btnNewMailsPlaySound = new KPushButton( KGuiItem( QString(), QString( "player_play" ), i18n( "Play the selected sound file" ), i18n( "Play the selected sound file" ) ), gboxNewMails, "btnNewMailsPlaySound" );
  layMainNewMailsDown->addWidget( btnNewMailsPlaySound, 0, 1 );

  txtNewMailsSound = new KLineEdit( gboxNewMails, "txtNewMailsSound" );
  layMainNewMailsDown->addWidget( txtNewMailsSound, 0, 2 );

  btnNewMailsChooseSound = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Press to select sound file" ), i18n( "Press to select sound file" ) ), gboxNewMails, "btnNewMailsChooseSound" );
  layMainNewMailsDown->addWidget( btnNewMailsChooseSound, 0, 3 );

  chkNewMailsCommand = new QCheckBox( i18n( "Command:" ), gboxNewMails, "chkNewMailsCommand" );
  QToolTip::add( chkNewMailsCommand, i18n( "Starts external program if new mail" ) );
  layMainNewMailsDown->addWidget( chkNewMailsCommand, 1, 0 );

  btnNewMailsExecCommand = new KPushButton( KGuiItem( QString(), QString( "exec" ), i18n( "Start the selected program" ), i18n( "Start the selected program" ) ), gboxNewMails, "btnNewMailsExecCommand" );
  layMainNewMailsDown->addWidget( btnNewMailsExecCommand, 1, 1 );

  txtNewMailsCommand = new KLineEdit( gboxNewMails, "txtNewMailsCommand" );
  layMainNewMailsDown->addWidget( txtNewMailsCommand, 1, 2 );

  btnNewMailsChooseCommand = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Select external command" ), i18n( "Select external command" ) ), gboxNewMails, "btnNewMailsChooseCommand" );
  layMainNewMailsDown->addWidget( btnNewMailsChooseCommand, 1, 3 );

  //items for the lower group box
  chkNoNewMailsMinimize = new QCheckBox( i18n( "Minimi&ze" ), gboxNoNewMails, "chkNoNewMailsMinimize" );
  QToolTip::add( chkNoNewMailsMinimize, i18n( "Minimize window if no new mail" ) );

  chkNoNewMailsTerminate = new QCheckBox( i18n( "Terminate" ), gboxNoNewMails, "chkNoNewMailsTerminate" );
  QToolTip::add( chkNoNewMailsTerminate, i18n( "Terminate kshowmail if no new mail" ) );

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
  config = KApplication::kApplication()->config();

  //load configured values
  load();

}


ConfigActions::~ConfigActions()
{
}

void ConfigActions::load( )
{
  config->setGroup( CONFIG_GROUP_ACTIONS );

  chkNewMailsAlertWindow->setChecked( config->readBoolEntry( CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW, DEFAULT_ACTION_NEW_MAIL_ALERTWINDOW ) );
  chkNewMailsMainWindow->setChecked( config->readBoolEntry( CONFIG_ENTRY_NEW_MAIL_MAINWINDOW, DEFAULT_ACTION_NEW_MAIL_MAINWINDOW ) );
  chkNewMailsBeep->setChecked( config->readBoolEntry( CONFIG_ENTRY_NEW_MAIL_BEEP, DEFAULT_ACTION_NEW_MAIL_BEEP ) );
  chkNewMailsSound->setChecked( config->readBoolEntry( CONFIG_ENTRY_NEW_MAIL_SOUND, DEFAULT_ACTION_NEW_MAIL_SOUND ) );
  chkNewMailsCommand->setChecked( config->readBoolEntry( CONFIG_ENTRY_NEW_MAIL_COMMAND, DEFAULT_ACTION_NEW_MAIL_COMMAND ) );
  chkNoNewMailsTerminate->setChecked( config->readBoolEntry( CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE, DEFAULT_ACTION_NO_NEW_MAIL_TERMINATE ) );
  chkNoNewMailsMinimize->setChecked( config->readBoolEntry( CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE, DEFAULT_ACTION_NO_NEW_MAIL_MINIMIZE ) );

  txtNewMailsSound->setText( config->readEntry( CONFIG_ENTRY_NEW_MAIL_SOUNDPATH ) );
  txtNewMailsCommand->setText( config->readEntry( CONFIG_ENTRY_NEW_MAIL_COMMANDPATH ) );

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
  config->setGroup( CONFIG_GROUP_ACTIONS );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW, chkNewMailsAlertWindow->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_MAINWINDOW, chkNewMailsMainWindow->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_BEEP, chkNewMailsBeep->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_SOUND, chkNewMailsSound->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_SOUNDPATH, txtNewMailsSound->text() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_COMMAND, chkNewMailsCommand->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NEW_MAIL_COMMANDPATH, txtNewMailsCommand->text() );
  config->writeEntry( CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE, chkNoNewMailsTerminate->isChecked() );
  config->writeEntry( CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE, chkNoNewMailsMinimize->isChecked() );

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
  QString path = KFileDialog::getOpenFileName( QString::null, "", this, i18n("Select external command") );

  //write selected path into the edit line
  if( path != QString::null )
    txtNewMailsCommand->setText( path );
}

void ConfigActions::slotPlaySound( )
{
  QString path = txtNewMailsSound->text();

  if( path != QString::null )
    KAudioPlayer::play( path );
}

void ConfigActions::slotExecuteCommand( )
{
  QString path = txtNewMailsCommand->text();

  if( path != QString::null )
  {
    KShellProcess proc;    //process handler to execute the binary

    proc << path;

    proc.start( KShellProcess::DontCare );
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





