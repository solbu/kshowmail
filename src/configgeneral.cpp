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

typedef KGenericFactory<ConfigGeneral, QWidget> ConfigGeneralFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfiggeneral, ConfigGeneralFactory(
    "kcm_kshowmailconfiggeneral" ) );

ConfigGeneral::ConfigGeneral( QWidget * parent, const char * name, const QStringList & args )
  : KCModule( ConfigGeneralFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configgeneral" );

  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //layout for check buttons (upper half)
  QGridLayout* layMainTop = new QGridLayout( layMain, 4, 2, 15 );
  layMainTop->setMargin( 20 );

  //group box for timers
  QGroupBox* gboxTimers = new QGroupBox( 0, Qt::Horizontal, i18n( "&Timers" ), this, "gboxTimers" );
  layMain->addWidget( gboxTimers );

  //layouts for timers
  QGridLayout* layTimers = new QGridLayout( gboxTimers->layout(),3, 3, 20 );

  //create items
  chkConfirmClose = new QCheckBox( i18n( "Confirm Close" ), this, "chkConfirmClose" );
  QToolTip::add( chkConfirmClose, i18n( "If checked, window close must be confirmed" ) );
  layMainTop->addWidget( chkConfirmClose, 0, 0 );

  chkConfirmDelete = new QCheckBox( i18n( "Confirm delete" ), this, "chkConfirmDelete" );
  QToolTip::add( chkConfirmDelete, i18n( "If checked, message delete must be confirmed" ) );
  layMainTop->addWidget( chkConfirmDelete, 0, 1 );

  chkStartMinimized = new QCheckBox( i18n( "Start Minimi&zed" ), this, "chkStartMinimized" );
  QToolTip::add( chkStartMinimized, i18n( "Application is started as icon" ) );
  layMainTop->addWidget( chkStartMinimized, 1, 0 );

  chkCloseToTray = new QCheckBox( i18n( "Close to tray" ), this, "chkCloseToTray" );
  QToolTip::add( chkCloseToTray, i18n( "Close button leaves the application running in tray" ) );
  layMainTop->addWidget( chkCloseToTray, 1, 1 );

  chkMinimizeToTray = new QCheckBox( i18n( "Minimize to tray" ), this, "chkMinimizeToTray" );
  QToolTip::add( chkMinimizeToTray, i18n( "Minimizes to the tray rather than to the taskbar" ) );
  layMainTop->addWidget( chkMinimizeToTray, 2, 0 );

  chkShowConnectionErrors = new QCheckBox( i18n( "Show Connection Errors during refresh" ), this, "chkShowConnectionErrors" );
  QToolTip::add( chkShowConnectionErrors, i18n( "If a connection error occurs during refresh (e.g. unknown server), an error message will be shown. During other actions, this error always will be shown" ) );
  layMainTop->addWidget( chkShowConnectionErrors, 2, 1 );

  chkKeepNew = new QCheckBox( i18n( "&Keep mail as new" ), this, "chkKeepNew" );
  QToolTip::add( chkKeepNew, i18n( "Keep mail as new until termination" ) );
  layMainTop->addWidget( chkKeepNew, 3, 0 );

  QLabel* lblTimerInitial = new QLabel( i18n( "Initial Timer:" ), gboxTimers, "lblTimerInitial" );
  QLabel* lblInitialUnit = new QLabel( i18n( "[Seconds]" ), gboxTimers, "lblInitialUnit" );
  spbInitial = new QSpinBox( 0, 99999, 1, gboxTimers, "spbInitial" );
  QToolTip::add( spbInitial, i18n( "Seconds until first automatic logon (0 = no automatic)" ) );
  QToolTip::add( lblTimerInitial, i18n( "Seconds until first automatic logon (0 = no automatic)" ) );
  layTimers->addWidget( lblTimerInitial, 0, 0 );
  layTimers->addWidget( spbInitial, 0, 1 );
  layTimers->addWidget( lblInitialUnit, 0, 2 );

  QLabel* lblTimerInterval = new QLabel( i18n( "Interval Timer:" ), gboxTimers, "lblTimerInterval" );
  QLabel* lblIntervalUnit = new QLabel( i18n( "[Minutes]" ), gboxTimers, "lblIntervalUnit" );
  spbInterval = new QSpinBox( 0, 99999, 1, gboxTimers, "spbInterval" );
  QToolTip::add( spbInterval, i18n( "Minutes between automatic logon (0 = no automatic)" ) );
  QToolTip::add( lblTimerInterval, i18n( "Minutes between automatic logon (0 = no automatic)" ) );
  layTimers->addWidget( lblTimerInterval, 1, 0 );
  layTimers->addWidget( spbInterval, 1, 1 );
  layTimers->addWidget( lblIntervalUnit, 1, 2 );

  QLabel* lblTimerTimeout = new QLabel( i18n( "Timeout:" ), gboxTimers, "lblTimerTimeout" );
  QLabel* lblTimeoutUnit = new QLabel( i18n( "[Seconds]" ), gboxTimers, "lblTimeoutUnit" );
  spbTimeout = new QSpinBox( MINIMUM_TIMEOUT_TIME, 99999, 1, gboxTimers, "spbTimeout" );
  QToolTip::add( spbTimeout, i18n( "Seconds until a server connect will be canceled" ) );
  QToolTip::add( lblTimerTimeout, i18n( "Seconds until a server connect will be canceled" ) );
  layTimers->addWidget( lblTimerTimeout, 2, 0 );
  layTimers->addWidget( spbTimeout, 2, 1 );
  layTimers->addWidget( lblTimeoutUnit, 2, 2 );


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
  config = KApplication::kApplication()->config();

  //load configurated values
  load();

}

ConfigGeneral::~ConfigGeneral()
{
}

void ConfigGeneral::load( )
{
  config->setGroup( CONFIG_GROUP_GENERAL );

  chkConfirmClose->setChecked( config->readBoolEntry( CONFIG_ENTRY_CONFIRM_CLOSE, DEFAULT_CONFIRM_CLOSE ) );
  chkConfirmDelete->setChecked( config->readBoolEntry( CONFIG_ENTRY_CONFIRM_DELETE, DEFAULT_CONFIRM_DELETE ) );
  chkStartMinimized->setChecked( config->readBoolEntry( CONFIG_ENTRY_START_MINIMIZED, DEFAULT_START_MINIMIZED ) );
  chkCloseToTray->setChecked( config->readBoolEntry( CONFIG_ENTRY_CLOSE_TO_TRAY, DEFAULT_CLOSE_TO_TRAY ) );
  chkMinimizeToTray->setChecked( config->readBoolEntry( CONFIG_ENTRY_MINIMIZE_TO_TRAY, DEFAULT_MINIMIZE_TO_TRAY ) );
  chkShowConnectionErrors->setChecked( config->readBoolEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, DEFAULT_SHOW_CONNECTION_ERRORS ) );
  chkKeepNew->setChecked( config->readBoolEntry( CONFIG_ENTRY_KEEP_NEW, DEFAULT_KEEP_NEW ) );

  spbInitial->setValue( config->readNumEntry( CONFIG_ENTRY_INITIAL_TIME, DEFAULT_INITIAL_TIME ) );
  spbInterval->setValue( config->readNumEntry( CONFIG_ENTRY_INTERVAL_TIME, DEFAULT_INTERVAL_TIME) );
  spbTimeout->setValue( config->readNumEntry( CONFIG_ENTRY_TIMEOUT_TIME, DEFAULT_TIMEOUT_TIME) );
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
  config->setGroup( CONFIG_GROUP_GENERAL );

  config->writeEntry( CONFIG_ENTRY_CONFIRM_CLOSE, chkConfirmClose->isChecked() );
  config->writeEntry( CONFIG_ENTRY_CONFIRM_DELETE, chkConfirmDelete->isChecked() );
  config->writeEntry( CONFIG_ENTRY_START_MINIMIZED, chkStartMinimized->isChecked() );
  config->writeEntry( CONFIG_ENTRY_CLOSE_TO_TRAY, chkCloseToTray->isChecked() );
  config->writeEntry( CONFIG_ENTRY_MINIMIZE_TO_TRAY, chkMinimizeToTray->isChecked() );
  config->writeEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, chkShowConnectionErrors->isChecked() );
  config->writeEntry( CONFIG_ENTRY_KEEP_NEW, chkKeepNew->isChecked() );
  config->writeEntry( CONFIG_ENTRY_INITIAL_TIME, spbInitial->value() );
  config->writeEntry( CONFIG_ENTRY_INTERVAL_TIME, spbInterval->value() );
  config->writeEntry( CONFIG_ENTRY_TIMEOUT_TIME, spbTimeout->value() );

  //write configuration to disk
  config->sync();
}

void ConfigGeneral::slotChanged( )
{
  KCModule::changed();
}

#include "configgeneral.moc"
