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

K_PLUGIN_FACTORY( ConfigLogFactory, registerPlugin<ConfigLog>(); )
K_EXPORT_PLUGIN( ConfigLogFactory( "kcm_kshowmailconfiglog" ) )

ConfigLog::ConfigLog( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigLogFactory::componentData(), parent, args )
{
  //build GUI
  //---------
  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this );

  //widgets for deleted mails log
  QVBoxLayout* layDelMails = new QVBoxLayout();
  layMain->addLayout( layDelMails );

  chkLogDeletedMails = new QCheckBox( i18n( "Log mails deleted by filter" ), this );
  chkLogDeletedMails->setToolTip( i18n( "Check to activate the log of mails deleted by filter." ) );
  layDelMails->addWidget( chkLogDeletedMails );
  connect( chkLogDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkLogDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChangeItems() ) );

  QVBoxLayout* layDelMailsConfig = new QVBoxLayout();
  layDelMailsConfig->setMargin( 20 );
  layMain->addLayout( layDelMailsConfig );

  grpDelMailsRemove = new QButtonGroup( NULL );
  connect( grpDelMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChanged() ) );
  connect( grpDelMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChangeItems() ) );
  btnDelMailsRemoveExit = new QRadioButton( i18n( "Remove log entries at exit" ), this );
  grpDelMailsRemove->addButton( btnDelMailsRemoveExit, ID_BUTTON_REMOVE_AT_EXIT );
  layDelMailsConfig->addWidget( btnDelMailsRemoveExit );

  QHBoxLayout* layDelMailsConfigDays = new QHBoxLayout();
  layDelMailsConfig->addLayout( layDelMailsConfigDays );
  layDelMailsConfigDays->setAlignment( Qt::AlignLeft );
  btnDelMailsRemoveDays = new QRadioButton( i18n( "Remove log entries after" ), this );
  grpDelMailsRemove->addButton( btnDelMailsRemoveDays, ID_BUTTON_REMOVE_AFTER_DAYS );
  layDelMailsConfigDays->addWidget( btnDelMailsRemoveDays );
  spbDelDays = new QSpinBox( this );
  spbDelDays->setMinimum( 1 );
  spbDelDays->setMaximum( 999999 );
  connect( spbDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChangeItems() ) );
  spbDelDays->setSuffix( i18n( " Days" ) );
  spbDelDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
  layDelMailsConfigDays->addWidget( spbDelDays );


  //widgets for moved mails log
  QVBoxLayout* layMovMails = new QVBoxLayout();
  layMain->addLayout( layMovMails );

  chkLogMovedMails = new QCheckBox( i18n( "Log mails moved by filter" ), this );
  chkLogMovedMails->setToolTip( i18n( "Check to activate the log of mails moved by filter." ) );
  layMovMails->addWidget( chkLogMovedMails );
  connect( chkLogMovedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkLogMovedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChangeItems() ) );

  QVBoxLayout* layMovMailsConfig = new QVBoxLayout();
  layMovMails->addLayout( layMovMailsConfig );

  grpMovMailsRemove = new QButtonGroup( NULL );
  connect( grpMovMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChanged() ) );
  connect( grpMovMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChangeItems() ) );
  btnMovMailsRemoveExit = new QRadioButton( i18n( "Remove log entries at exit" ), this );
  grpMovMailsRemove->addButton( btnMovMailsRemoveExit, ID_BUTTON_REMOVE_AT_EXIT );
  layMovMailsConfig->addWidget( btnMovMailsRemoveExit );

  QHBoxLayout* layMovMailsConfigDays = new QHBoxLayout();
  layMovMailsConfig->addLayout( layMovMailsConfigDays );
  layMovMailsConfigDays->setAlignment( Qt::AlignLeft );
  btnMovMailsRemoveDays = new QRadioButton( i18n( "Remove log entries after" ), this );
  grpMovMailsRemove->addButton( btnMovMailsRemoveDays, ID_BUTTON_REMOVE_AFTER_DAYS );
  layMovMailsConfigDays->addWidget( btnMovMailsRemoveDays );
  spbMovDays = new QSpinBox( this );
  spbMovDays->setMinimum( 1 );
  spbMovDays->setMaximum( 999999 );
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
  config = KGlobal::config();

}

ConfigLog::~ConfigLog()
{
}

void ConfigLog::load()
{
  //set group
  KConfigGroup* configLog = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //load settings
  chkLogDeletedMails->setChecked( configLog->readEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, DEFAULT_LOG_LOG_DELETED_MAILS ) );
  chkLogMovedMails->setChecked( configLog->readEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, DEFAULT_LOG_LOG_MOVED_MAILS ) );

  QRadioButton* btnToCheck;
  if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, DEFAULT_LOG_REMOVE_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, DEFAULT_LOG_REMOVE_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    if( QString( DEFAULT_LOG_REMOVE_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
      btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
    else
      btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

  btnToCheck->setChecked( true );

  
  if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, DEFAULT_LOG_REMOVE_MOVED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, DEFAULT_LOG_REMOVE_MOVED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    if( QString( DEFAULT_LOG_REMOVE_MOVED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
      btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

  btnToCheck->setChecked( true );

  spbDelDays->setValue( configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_DELETED_MAILS ) );
  spbMovDays->setValue( configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, DEFAULT_LOG_HOLDDAYS_MOVED_MAILS ) );

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

  QRadioButton* btnToCheck;
  if( QString( DEFAULT_LOG_REMOVE_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
    btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( QString( DEFAULT_LOG_REMOVE_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ) )
    btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    btnToCheck = static_cast<QRadioButton*>( grpDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

  btnToCheck->setChecked( true );
  
  if( QString( DEFAULT_LOG_REMOVE_MOVED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( QString( DEFAULT_LOG_REMOVE_MOVED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ) )
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    btnToCheck = static_cast<QRadioButton*>( grpMovMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

  btnToCheck->setChecked( true );

  spbDelDays->setValue( DEFAULT_LOG_HOLDDAYS_DELETED_MAILS );
  spbMovDays->setValue( DEFAULT_LOG_HOLDDAYS_MOVED_MAILS );

  //enable or disable Items
  slotChangeItems();

  slotChanged();
}

void ConfigLog::save()
{
  //set group
  KConfigGroup* configLog = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //write settings
  configLog->writeEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, chkLogDeletedMails->isChecked() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, chkLogMovedMails->isChecked() );

  switch( grpDelMailsRemove->checkedId() )
  {
    case ID_BUTTON_REMOVE_AT_EXIT     : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
    case ID_BUTTON_REMOVE_AFTER_DAYS  : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
    default                           : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
  }

  switch( grpMovMailsRemove->checkedId() )
  {
    case ID_BUTTON_REMOVE_AT_EXIT     : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
    case ID_BUTTON_REMOVE_AFTER_DAYS  : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
    default                           : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
  }

  configLog->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, spbDelDays->value() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, spbMovDays->value() );
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
    switch( grpDelMailsRemove->checkedId() )
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
    switch( grpMovMailsRemove->checkedId() )
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
