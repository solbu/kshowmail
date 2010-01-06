//
// C++ Implementation: configgeneral
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configgeneral.h"

K_PLUGIN_FACTORY( ConfigGeneralFactory, registerPlugin<ConfigGeneral>(); )
K_EXPORT_PLUGIN( ConfigGeneralFactory( "kcm_kshowmailconfiggeneral" ) )

ConfigGeneral::ConfigGeneral( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigGeneralFactory::componentData(), parent, args )
{
  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this );

  //layout for check buttons (upper half)
  QGridLayout* layMainTop = new QGridLayout();
  layMainTop->setMargin( 20 );
  layMain->addLayout( layMainTop );

  //group box for timers
  QGroupBox* gboxTimers = new QGroupBox( i18n( "&Timers" ), this );
  layMain->addWidget( gboxTimers );

  //layouts for timers
  QGridLayout* layTimers = new QGridLayout();
  gboxTimers->setLayout( layTimers );

  //create items
  chkConfirmClose = new QCheckBox( i18n( "Confirm Close" ), this );
  chkConfirmClose->setToolTip( i18n( "If checked, window close must be confirmed" ) );
  layMainTop->addWidget( chkConfirmClose, 0, 0 );

  chkConfirmDelete = new QCheckBox( i18n( "Confirm delete" ), this );
  chkConfirmDelete->setToolTip( i18n( "If checked, message delete must be confirmed" ) );
  layMainTop->addWidget( chkConfirmDelete, 0, 1 );

  chkStartMinimized = new QCheckBox( i18n( "Start Minimi&zed" ), this );
  chkStartMinimized->setToolTip( i18n( "Application is started as icon" ) );
  layMainTop->addWidget( chkStartMinimized, 1, 0 );

  chkCloseToTray = new QCheckBox( i18n( "Close to tray" ), this );
  chkCloseToTray->setToolTip( i18n( "Close button leaves the application running in tray" ) );
  layMainTop->addWidget( chkCloseToTray, 1, 1 );

  chkMinimizeToTray = new QCheckBox( i18n( "Minimize to tray" ), this );
  chkMinimizeToTray->setToolTip( i18n( "Minimizes to the tray rather than to the taskbar" ) );
  layMainTop->addWidget( chkMinimizeToTray, 2, 0 );

  chkShowConnectionErrors = new QCheckBox( i18n( "Show Connection Errors during refresh" ), this );
  chkShowConnectionErrors->setToolTip( i18n( "If a connection error occurs during refresh (e.g. unknown server), an error message will be shown. During other actions, this error always will be shown" ) );
  layMainTop->addWidget( chkShowConnectionErrors, 2, 1 );

  chkKeepNew = new QCheckBox( i18n( "&Keep mail as new" ), this );
  chkKeepNew->setToolTip( i18n( "Keep mail as new until termination" ) );
  layMainTop->addWidget( chkKeepNew, 3, 0 );

  QLabel* lblTimerInitial = new QLabel( i18n( "Initial Timer:" ), gboxTimers );
  spbInitial = new QSpinBox( gboxTimers );
  spbInitial->setMinimum( 0 );
  spbInitial->setMaximum( 99999 );
  spbInitial->setSuffix( i18n( " Seconds") );
  spbInitial->setToolTip( i18n( "Seconds until first automatic logon (0 = no automatic)" ) );
  lblTimerInitial->setToolTip( i18n( "Seconds until first automatic logon (0 = no automatic)" ) );
  layTimers->addWidget( lblTimerInitial, 0, 0 );
  layTimers->addWidget( spbInitial, 0, 1 );

  QLabel* lblTimerInterval = new QLabel( i18n( "Interval Timer:" ), gboxTimers );
  spbInterval = new QSpinBox( gboxTimers );
  spbInterval->setMinimum( 0 );
  spbInterval->setMaximum( 99999 );
  spbInterval->setSuffix( i18n( " Minutes") );
  spbInterval->setToolTip( i18n( "Minutes between automatic logon (0 = no automatic)" ) );
  lblTimerInterval->setToolTip( i18n( "Minutes between automatic logon (0 = no automatic)" ) );
  layTimers->addWidget( lblTimerInterval, 1, 0 );
  layTimers->addWidget( spbInterval, 1, 1 );

  QLabel* lblTimerTimeout = new QLabel( i18n( "Timeout:" ), gboxTimers );
  spbTimeout = new QSpinBox( gboxTimers );
  spbTimeout->setMinimum( MINIMUM_TIMEOUT_TIME );
  spbTimeout->setMaximum( 99999 );
  spbTimeout->setSuffix( i18n( " Seconds" ) );
  spbTimeout->setToolTip( i18n( "Seconds until a server connect will be canceled" ) );
  lblTimerTimeout->setToolTip( i18n( "Seconds until a server connect will be canceled" ) );
  layTimers->addWidget( lblTimerTimeout, 2, 0 );
  layTimers->addWidget( spbTimeout, 2, 1 );


  //connect all configuration itmes with slot changed() to notify the dialog about changes
  connect( chkConfirmClose, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkConfirmDelete, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkStartMinimized, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkCloseToTray, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMinimizeToTray, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkShowConnectionErrors, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkKeepNew, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( spbInitial, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbInterval, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbTimeout, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );


  //get application config object (kshowmailrc)
  config = KGlobal::config();

  //load configurated values
  load();

}

ConfigGeneral::~ConfigGeneral()
{
}

void ConfigGeneral::load( )
{
  KConfigGroup* configGeneral = new KConfigGroup( config, CONFIG_GROUP_GENERAL );

  chkConfirmClose->setChecked( configGeneral->readEntry( CONFIG_ENTRY_CONFIRM_CLOSE, DEFAULT_CONFIRM_CLOSE ) );
  chkConfirmDelete->setChecked( configGeneral->readEntry( CONFIG_ENTRY_CONFIRM_DELETE, DEFAULT_CONFIRM_DELETE ) );
  chkStartMinimized->setChecked( configGeneral->readEntry( CONFIG_ENTRY_START_MINIMIZED, DEFAULT_START_MINIMIZED ) );
  chkCloseToTray->setChecked( configGeneral->readEntry( CONFIG_ENTRY_CLOSE_TO_TRAY, DEFAULT_CLOSE_TO_TRAY ) );
  chkMinimizeToTray->setChecked( configGeneral->readEntry( CONFIG_ENTRY_MINIMIZE_TO_TRAY, DEFAULT_MINIMIZE_TO_TRAY ) );
  chkShowConnectionErrors->setChecked( configGeneral->readEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, DEFAULT_SHOW_CONNECTION_ERRORS ) );
  chkKeepNew->setChecked( configGeneral->readEntry( CONFIG_ENTRY_KEEP_NEW, DEFAULT_KEEP_NEW ) );

  spbInitial->setValue( configGeneral->readEntry( CONFIG_ENTRY_INITIAL_TIME, DEFAULT_INITIAL_TIME ) );
  spbInterval->setValue( configGeneral->readEntry( CONFIG_ENTRY_INTERVAL_TIME, DEFAULT_INTERVAL_TIME) );
  spbTimeout->setValue( configGeneral->readEntry( CONFIG_ENTRY_TIMEOUT_TIME, DEFAULT_TIMEOUT_TIME) );
}

void ConfigGeneral::defaults( )
{
  chkConfirmClose->setChecked( DEFAULT_CONFIRM_CLOSE );
  chkConfirmDelete->setChecked( DEFAULT_CONFIRM_DELETE );
  chkStartMinimized->setChecked( DEFAULT_START_MINIMIZED );
  chkCloseToTray->setChecked( DEFAULT_CLOSE_TO_TRAY );
  chkMinimizeToTray->setChecked( DEFAULT_MINIMIZE_TO_TRAY );
  chkShowConnectionErrors->setChecked( DEFAULT_SHOW_CONNECTION_ERRORS );
  chkKeepNew->setChecked( DEFAULT_KEEP_NEW );

  spbInitial->setValue( DEFAULT_INITIAL_TIME );
  spbInterval->setValue( DEFAULT_INTERVAL_TIME );
  spbTimeout->setValue( DEFAULT_TIMEOUT_TIME );
}

void ConfigGeneral::save( )
{
  KConfigGroup* configGeneral = new KConfigGroup( config, CONFIG_GROUP_GENERAL );

  configGeneral->writeEntry( CONFIG_ENTRY_CONFIRM_CLOSE, chkConfirmClose->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_CONFIRM_DELETE, chkConfirmDelete->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_START_MINIMIZED, chkStartMinimized->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_CLOSE_TO_TRAY, chkCloseToTray->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_MINIMIZE_TO_TRAY, chkMinimizeToTray->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, chkShowConnectionErrors->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_KEEP_NEW, chkKeepNew->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_INITIAL_TIME, spbInitial->value() );
  configGeneral->writeEntry( CONFIG_ENTRY_INTERVAL_TIME, spbInterval->value() );
  configGeneral->writeEntry( CONFIG_ENTRY_TIMEOUT_TIME, spbTimeout->value() );

  //write configuration to disk
  config->sync();
}

void ConfigGeneral::slotChanged( )
{
  KCModule::changed();
}

#include "configgeneral.moc"
