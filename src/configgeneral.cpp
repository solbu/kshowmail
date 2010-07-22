/*
Copyright 2010  Ulrich Weigelt <ulrich.weigelt@gmx.de>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

  //group box for automatic refresh
  QGroupBox* gboxAutomaticRefresh = new QGroupBox( i18n( "&Automatic Refresh" ), this );
  layMain->addWidget( gboxAutomaticRefresh );

  //layout for automatic refresh
  QGridLayout* layAutoRefresh = new QGridLayout();
  gboxAutomaticRefresh->setLayout( layAutoRefresh );

  //group box for server connect
  QGroupBox* gBoxServerConnect = new QGroupBox( i18n( "Server Connect"), this  );
  layMain->addWidget( gBoxServerConnect );
  QGridLayout* layServerConnect = new QGridLayout();
  gBoxServerConnect->setLayout( layServerConnect );

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

  chkKeepNew = new QCheckBox( i18n( "&Keep mail as new" ), this );
  chkKeepNew->setToolTip( i18n( "Keep mail as new until termination" ) );
  layMainTop->addWidget( chkKeepNew, 2, 0 );



  chkAutomaticRefresh = new QCheckBox( i18n( "Automatic refresh" ) );
  chkAutomaticRefresh->setToolTip( i18n( "Run refreshes of the mail list automatically") );
  layAutoRefresh->addWidget( chkAutomaticRefresh, 0, 0 );
  connect( chkAutomaticRefresh, SIGNAL( toggled( bool) ), this, SLOT( slotAutomaticRefreshChanged( bool ) ) );

  lblTimerInitial = new QLabel( i18n( "Initial Timer:" ), gboxAutomaticRefresh );
  spbInitial = new QSpinBox( gboxAutomaticRefresh );
  spbInitial->setMinimum( 0 );
  spbInitial->setMaximum( 99999 );
  spbInitial->setSuffix( i18n( " Seconds") );
  spbInitial->setToolTip( i18n( "Seconds until first automatic refresh (0 = it refreshes immediately)" ) );
  lblTimerInitial->setToolTip( i18n( "Seconds until first automatic refresh (0 = it refreshes immediately)" ) );
  layAutoRefresh->addWidget( lblTimerInitial, 1, 0 );
  layAutoRefresh->addWidget( spbInitial, 1, 1 );

  lblTimerInterval = new QLabel( i18n( "Interval Timer:" ), gboxAutomaticRefresh );
  spbInterval = new QSpinBox( gboxAutomaticRefresh );
  spbInterval->setMinimum( 1 );
  spbInterval->setMaximum( 99999 );
  spbInterval->setSuffix( i18n( " Minutes") );
  spbInterval->setToolTip( i18n( "Minutes between automatic refresh (0 = no automatic)" ) );
  lblTimerInterval->setToolTip( i18n( "Minutes between automatic refresh (0 = no automatic)" ) );
  layAutoRefresh->addWidget( lblTimerInterval, 2, 0 );
  layAutoRefresh->addWidget( spbInterval, 2, 1 );


  chkShowConnectionErrors = new QCheckBox( i18n( "Show Connection Errors" ), gBoxServerConnect );
  chkShowConnectionErrors->setToolTip( i18n( "If a connection error occurs (e.g. unknown server), an error message will be shown." ) );
  layServerConnect->addWidget( chkShowConnectionErrors, 0, 0 );


  QLabel* lblTimerTimeout = new QLabel( i18n( "Timeout:" ), gBoxServerConnect );
  spbTimeout = new QSpinBox( gBoxServerConnect );
  spbTimeout->setMinimum( MINIMUM_TIMEOUT_TIME );
  spbTimeout->setMaximum( 99999 );
  spbTimeout->setSuffix( i18n( " Seconds" ) );
  spbTimeout->setToolTip( i18n( "Seconds until a server connect will be canceled" ) );
  lblTimerTimeout->setToolTip( i18n( "Seconds until a server connect will be canceled" ) );
  layServerConnect->addWidget( lblTimerTimeout, 1, 0 );
  layServerConnect->addWidget( spbTimeout, 1, 1 );


  //connect all configuration itmes with slot changed() to notify the dialog about changes
  connect( chkConfirmClose, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkConfirmDelete, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkStartMinimized, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkCloseToTray, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkShowConnectionErrors, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkKeepNew, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( spbInitial, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbInterval, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbTimeout, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( chkAutomaticRefresh, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );


  //get application config object (kshowmailrc)
  config = KGlobal::config();

  setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

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
  chkShowConnectionErrors->setChecked( configGeneral->readEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, DEFAULT_SHOW_CONNECTION_ERRORS ) );
  chkKeepNew->setChecked( configGeneral->readEntry( CONFIG_ENTRY_KEEP_NEW, DEFAULT_KEEP_NEW ) );


  chkAutomaticRefresh->setChecked( configGeneral->readEntry( CONFIG_ENTRY_AUTO_REFRESH, DEFAULT_AUTO_REFRESH ) );
  slotAutomaticRefreshChanged( chkAutomaticRefresh->isChecked() );
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
  chkShowConnectionErrors->setChecked( DEFAULT_SHOW_CONNECTION_ERRORS );
  chkKeepNew->setChecked( DEFAULT_KEEP_NEW );

  chkAutomaticRefresh->setChecked( DEFAULT_AUTO_REFRESH );
  slotAutomaticRefreshChanged( chkAutomaticRefresh->isChecked() );
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
  configGeneral->writeEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, chkShowConnectionErrors->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_KEEP_NEW, chkKeepNew->isChecked() );
  configGeneral->writeEntry( CONFIG_ENTRY_INITIAL_TIME, spbInitial->value() );
  configGeneral->writeEntry( CONFIG_ENTRY_INTERVAL_TIME, spbInterval->value() );
  configGeneral->writeEntry( CONFIG_ENTRY_TIMEOUT_TIME, spbTimeout->value() );
  configGeneral->writeEntry( CONFIG_ENTRY_AUTO_REFRESH, chkAutomaticRefresh->isChecked() );

  //write configuration to disk
  config->sync();
}

void ConfigGeneral::slotChanged( )
{
  KCModule::changed();
}

void ConfigGeneral::slotAutomaticRefreshChanged( bool checked ) {

  spbInitial->setEnabled( checked );
  spbInterval->setEnabled( checked );
  lblTimerInitial->setEnabled( checked );
  lblTimerInterval->setEnabled( checked );
}

#include "configgeneral.moc"
