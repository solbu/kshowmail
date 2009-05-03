//
// C++ Implementation: configlog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configlog.h"

typedef KGenericFactory<ConfigLog, QWidget> ConfigLogFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfiglog, ConfigLogFactory(
    "kcm_kshowmailconfiglog" ) );

ConfigLog::ConfigLog( QWidget * parent, const char * name, const QStringList & args )
  : KCModule( ConfigLogFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configlog" );

  //build GUI
  //---------
  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //widgets for deleted mails log
  QVBoxLayout* layDelMails = new QVBoxLayout( layMain, 10, "layDelMails" );

  chkLogDeletedMails = new QCheckBox( i18n( "Log mails deleted by filter" ), this, "chkLogDeletedMails" );
  layDelMails->addWidget( chkLogDeletedMails );
  QToolTip::add( chkLogDeletedMails, i18n( "Check to activate the log of mails deleted by filter." ) );
  connect( chkLogDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkLogDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChangeItems() ) );

  QVBoxLayout* layDelMailsConfig = new QVBoxLayout( layDelMails, 10, "layDelMailsConfig" );
  layDelMailsConfig->setMargin( 10 );

  grpDelMailsRemove = new QButtonGroup( NULL, "grpDelMailsRemove" );
  connect( grpDelMailsRemove, SIGNAL( clicked( int ) ), this, SLOT( slotChanged() ) );
  connect( grpDelMailsRemove, SIGNAL( clicked( int ) ), this, SLOT( slotChangeItems() ) );
  btnDelMailsRemoveExit = new QRadioButton( i18n( "Remove log entries at exit" ), this, "btnDelMailsRemoveExit" );
  grpDelMailsRemove->insert( btnDelMailsRemoveExit, ID_BUTTON_REMOVE_AT_EXIT );
  layDelMailsConfig->addWidget( btnDelMailsRemoveExit );

  QHBoxLayout* layDelMailsConfigDays = new QHBoxLayout( layDelMailsConfig, 0, "layDelMailsConfigDays" );
  layDelMailsConfigDays->setAlignment( Qt::AlignLeft );
  btnDelMailsRemoveDays = new QRadioButton( i18n( "Remove log entries after" ), this, "btnDelMailsRemoveDays" );
  grpDelMailsRemove->insert( btnDelMailsRemoveDays, ID_BUTTON_REMOVE_AFTER_DAYS );
  layDelMailsConfigDays->addWidget( btnDelMailsRemoveDays );
  spbDelDays = new QSpinBox( 1, 999999, 1, this, "spbDelDays" );
  connect( spbDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChangeItems() ) );
  spbDelDays->setSuffix( i18n( " Days" ) );
  spbDelDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
  layDelMailsConfigDays->addWidget( spbDelDays );


  //widgets for moved mails log
  QVBoxLayout* layMovMails = new QVBoxLayout( layMain, 10, "layMovMails" );

  chkLogMovedMails = new QCheckBox( i18n( "Log mails moved by filter" ), this, "chkLogMovedMails" );
  layMovMails->addWidget( chkLogMovedMails );
  QToolTip::add( chkLogMovedMails, i18n( "Check to activate the log of mails moved by filter." ) );
  connect( chkLogMovedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkLogMovedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChangeItems() ) );

  QVBoxLayout* layMovMailsConfig = new QVBoxLayout( layMovMails, 10, "layMovMailsConfig" );
  layMovMailsConfig->setMargin( 10 );

  grpMovMailsRemove = new QButtonGroup( NULL, "grpMovMailsRemove" );
  connect( grpMovMailsRemove, SIGNAL( clicked( int ) ), this, SLOT( slotChanged() ) );
  connect( grpMovMailsRemove, SIGNAL( clicked( int ) ), this, SLOT( slotChangeItems() ) );
  btnMovMailsRemoveExit = new QRadioButton( i18n( "Remove log entries at exit" ), this, "btnMovMailsRemoveExit" );
  grpMovMailsRemove->insert( btnMovMailsRemoveExit, ID_BUTTON_REMOVE_AT_EXIT );
  layMovMailsConfig->addWidget( btnMovMailsRemoveExit );

  QHBoxLayout* layMovMailsConfigDays = new QHBoxLayout( layMovMailsConfig, 0, "layMovMailsConfigDays" );
  layMovMailsConfigDays->setAlignment( Qt::AlignLeft );
  btnMovMailsRemoveDays = new QRadioButton( i18n( "Remove log entries after" ), this, "btnMovMailsRemoveDays" );
  grpMovMailsRemove->insert( btnMovMailsRemoveDays, ID_BUTTON_REMOVE_AFTER_DAYS );
  layMovMailsConfigDays->addWidget( btnMovMailsRemoveDays );
  spbMovDays = new QSpinBox( 1, 999999, 1, this, "spbMovDays" );
  connect( spbMovDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbMovDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChangeItems() ) );
  spbMovDays->setSuffix( i18n( " Days" ) );
  spbMovDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
  layMovMailsConfigDays->addWidget( spbMovDays );

  //the log of moved mails is not saved yet, maybe later...
  btnMovMailsRemoveExit->setHidden( true );
  btnMovMailsRemoveDays->setHidden( true );
  spbMovDays->setHidden( true );

  //get application config object (kshowmailrc)
  config = KApplication::kApplication()->config();

  //load configured values
  load();
}

ConfigLog::~ConfigLog()
{
}

void ConfigLog::load()
{
  //set group
  config->setGroup( CONFIG_GROUP_LOG );

  //load settings
  chkLogDeletedMails->setChecked( config->readBoolEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, DEFAULT_LOG_LOG_DELETED_MAILS ) );
  chkLogMovedMails->setChecked( config->readBoolEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, DEFAULT_LOG_LOG_MOVED_MAILS ) );

  if( config->readEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, DEFAULT_LOG_REMOVE_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
  else if( config->readEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, DEFAULT_LOG_REMOVE_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );
  else
    if( DEFAULT_LOG_REMOVE_DELETED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
      grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
    else
      grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );

  if( config->readEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, DEFAULT_LOG_REMOVE_MOVED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
  else if( config->readEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, DEFAULT_LOG_REMOVE_MOVED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );
  else
    if( DEFAULT_LOG_REMOVE_MOVED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
      grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
  else
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );

  spbDelDays->setValue( config->readNumEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_DELETED_MAILS ) );
  spbMovDays->setValue( config->readNumEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, DEFAULT_LOG_HOLDDAYS_MOVED_MAILS ) );

  //enable or disable Items
  slotChangeItems();
}

void ConfigLog::defaults()
{
  if( DEFAULT_LOG_LOG_DELETED_MAILS )
    chkLogDeletedMails->setChecked( true );
  else
    chkLogMovedMails->setChecked( false );

  if( DEFAULT_LOG_LOG_MOVED_MAILS )
    chkLogMovedMails->setChecked( true );
  else
    chkLogMovedMails->setChecked( false );

  if( DEFAULT_LOG_REMOVE_DELETED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
  else if( DEFAULT_LOG_REMOVE_DELETED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );
  else
    grpDelMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );

  if( DEFAULT_LOG_REMOVE_MOVED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AT_EXIT );
  else if( DEFAULT_LOG_REMOVE_MOVED_MAILS == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );
  else
    grpMovMailsRemove->setButton( ID_BUTTON_REMOVE_AFTER_DAYS );

  spbDelDays->setValue( DEFAULT_LOG_HOLDDAYS_DELETED_MAILS );
  spbMovDays->setValue( DEFAULT_LOG_HOLDDAYS_MOVED_MAILS );

  //enable or disable Items
  slotChangeItems();

  slotChanged();
}

void ConfigLog::save()
{
  //set group
  config->setGroup( CONFIG_GROUP_LOG );

  //write settings
  config->writeEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, chkLogDeletedMails->isOn() );
  config->writeEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, chkLogMovedMails->isOn() );

  switch( grpDelMailsRemove->selectedId() )
  {
    case ID_BUTTON_REMOVE_AT_EXIT     : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
    case ID_BUTTON_REMOVE_AFTER_DAYS  : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
    default                           : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
  }

  switch( grpMovMailsRemove->selectedId() )
  {
    case ID_BUTTON_REMOVE_AT_EXIT     : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
    case ID_BUTTON_REMOVE_AFTER_DAYS  : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
    default                           : config->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
  }

  config->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, spbDelDays->value() );
  config->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, spbMovDays->value() );
}

void ConfigLog::slotChanged()
{
  KCModule::changed();
}

void ConfigLog::slotChangeItems()
{
  if( chkLogDeletedMails->isChecked() )
  {
    btnDelMailsRemoveExit->setEnabled( true );
    btnDelMailsRemoveDays->setEnabled( true );
    spbDelDays->setEnabled( true );
    switch( grpDelMailsRemove->selectedId() )
    {
      case ID_BUTTON_REMOVE_AFTER_DAYS      : spbDelDays->setEnabled( true ); break;
      case ID_BUTTON_REMOVE_AT_EXIT         : spbDelDays->setEnabled( false ); break;
      default                               : spbDelDays->setEnabled( true ); break;
    }
  }
  else
  {
    btnDelMailsRemoveExit->setEnabled( false );
    btnDelMailsRemoveDays->setEnabled( false );
    spbDelDays->setEnabled( false );
  }

  if( chkLogMovedMails->isChecked() )
  {
    btnMovMailsRemoveExit->setEnabled( true );
    btnMovMailsRemoveDays->setEnabled( true );
    spbMovDays->setEnabled( true );
    switch( grpMovMailsRemove->selectedId() )
    {
      case ID_BUTTON_REMOVE_AFTER_DAYS      : spbMovDays->setEnabled( true ); break;
      case ID_BUTTON_REMOVE_AT_EXIT         : spbMovDays->setEnabled( false ); break;
      default                               : spbMovDays->setEnabled( true ); break;
    }
  }
  else
  {
    btnMovMailsRemoveExit->setEnabled( false );
    btnMovMailsRemoveDays->setEnabled( false );
    spbMovDays->setEnabled( false );
  }
}


#include "configlog.moc"
