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
  spbDelDays->setSuffix( i18nc( "@label:spinbox number of days till deletion", " Days" ) );
  spbDelDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
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
  spbMovDays->setSuffix( i18nc( "@label:spinbox number of days till deletion", " Days" ) );
  spbMovDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
  layMovMailsConfigDays->addWidget( spbMovDays );

  //widgets for manual deleted mails log
  QVBoxLayout* layManualDelMails = new QVBoxLayout();
  layMain->addLayout( layManualDelMails );

  chkLogManualDeletedMails = new QCheckBox( i18n( "Log manual deleted mails" ), this );
  chkLogManualDeletedMails->setToolTip( i18n( "Check to activate the log of manual deleted mails." ) );
  layManualDelMails->addWidget( chkLogManualDeletedMails );
  connect( chkLogManualDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkLogManualDeletedMails, SIGNAL( toggled( bool ) ), this, SLOT( slotChangeItems() ) );

  QVBoxLayout* layManualDelMailsConfig = new QVBoxLayout();
  layManualDelMailsConfig->setMargin( 20 );
  layMain->addLayout( layManualDelMailsConfig );

  grpManualDelMailsRemove = new QButtonGroup( NULL );
  connect( grpManualDelMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChanged() ) );
  connect( grpManualDelMailsRemove, SIGNAL( buttonClicked( int ) ), this, SLOT( slotChangeItems() ) );
  btnManualDelMailsRemoveExit = new QRadioButton( i18n( "Remove log entries at exit" ), this );
  grpManualDelMailsRemove->addButton( btnManualDelMailsRemoveExit, ID_BUTTON_REMOVE_AT_EXIT );
  layManualDelMailsConfig->addWidget( btnManualDelMailsRemoveExit );

  QHBoxLayout* layManualDelMailsConfigDays = new QHBoxLayout();
  layManualDelMailsConfig->addLayout( layManualDelMailsConfigDays );
  layManualDelMailsConfigDays->setAlignment( Qt::AlignLeft );
  btnManualDelMailsRemoveDays = new QRadioButton( i18n( "Remove log entries after" ), this );
  grpManualDelMailsRemove->addButton( btnManualDelMailsRemoveDays, ID_BUTTON_REMOVE_AFTER_DAYS );
  layManualDelMailsConfigDays->addWidget( btnManualDelMailsRemoveDays );
  spbManualDelDays = new QSpinBox( this );
  spbManualDelDays->setMinimum( 1 );
  spbManualDelDays->setMaximum( 999999 );
  connect( spbManualDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChanged() ) );
  connect( spbManualDelDays, SIGNAL( valueChanged( int ) ), this, SLOT( slotChangeItems() ) );
  spbManualDelDays->setSuffix( i18nc( "@label:spinbox number of days till deletion", " Days" ) );
  spbManualDelDays->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layManualDelMailsConfigDays->addWidget( spbManualDelDays );


  //the log of moved mails is not saved yet, maybe later...
  btnMovMailsRemoveExit->setHidden( true );
  btnMovMailsRemoveDays->setHidden( true );
  spbMovDays->setHidden( true );

  setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

  //get application config object (kshowmailrc)
  config = KGlobal::config();

}

ConfigLog::~ConfigLog()
{
}

void ConfigLog::load()
{
  //set group
  KConfigGroup* configLog = new KConfigGroup( config, CONFIG_GROUP_LOG );

  //load settings
  chkLogDeletedMails->setChecked( configLog->readEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, DEFAULT_LOG_LOG_DELETED_MAILS ) );
  chkLogMovedMails->setChecked( configLog->readEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, DEFAULT_LOG_LOG_MOVED_MAILS ) );
  chkLogManualDeletedMails->setChecked( configLog->readEntry( CONFIG_ENTRY_LOG_LOG_MANUAL_DELETED_MAILS, DEFAULT_LOG_LOG_MANUAL_DELETED_MAILS ) );

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

  if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
    btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( configLog->readEntry( CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS ) == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
    btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    if( QString( DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
      btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
    else
      btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

    btnToCheck->setChecked( true );

  spbDelDays->setValue( configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_DELETED_MAILS ) );
  spbMovDays->setValue( configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, DEFAULT_LOG_HOLDDAYS_MOVED_MAILS ) );
  spbManualDelDays->setValue( configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MANUAL_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_MANUAL_DELETED_MAILS ) );

  //enable or disable Items
  slotChangeItems();
}

void ConfigLog::defaults()
{
  if( DEFAULT_LOG_LOG_DELETED_MAILS )
    chkLogDeletedMails->setChecked( true );
  else
    chkLogDeletedMails->setChecked( false );

  if( DEFAULT_LOG_LOG_MOVED_MAILS )
    chkLogMovedMails->setChecked( true );
  else
    chkLogMovedMails->setChecked( false );

  if( DEFAULT_LOG_LOG_MANUAL_DELETED_MAILS )
    chkLogManualDeletedMails->setChecked( true );
  else
    chkLogManualDeletedMails->setChecked( false );

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

  if( QString( DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ) )
    btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AT_EXIT ) );
  else if( QString( DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS ) == QString( CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ) )
    btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );
  else
    btnToCheck = static_cast<QRadioButton*>( grpManualDelMailsRemove->button( ID_BUTTON_REMOVE_AFTER_DAYS ) );

  btnToCheck->setChecked( true );

  spbDelDays->setValue( DEFAULT_LOG_HOLDDAYS_DELETED_MAILS );
  spbMovDays->setValue( DEFAULT_LOG_HOLDDAYS_MOVED_MAILS );
  spbManualDelDays->setValue( DEFAULT_LOG_HOLDDAYS_MANUAL_DELETED_MAILS );

  //enable or disable Items
  slotChangeItems();

  slotChanged();
}

void ConfigLog::save()
{
  //set group
  KConfigGroup* configLog = new KConfigGroup( config, CONFIG_GROUP_LOG );

  //write settings
  configLog->writeEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, chkLogDeletedMails->isChecked() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, chkLogMovedMails->isChecked() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_LOG_MANUAL_DELETED_MAILS, chkLogManualDeletedMails->isChecked() );

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

  switch( grpManualDelMailsRemove->checkedId() )
  {
    case ID_BUTTON_REMOVE_AT_EXIT     : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT ); break;
    case ID_BUTTON_REMOVE_AFTER_DAYS  : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
    default                           : configLog->writeEntry( CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS ); break;
  }

  configLog->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, spbDelDays->value() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS, spbMovDays->value() );
  configLog->writeEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MANUAL_DELETED_MAILS, spbManualDelDays->value() );
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

  if( chkLogManualDeletedMails->isChecked() )
  {
    btnManualDelMailsRemoveExit->setEnabled( true );
    btnManualDelMailsRemoveDays->setEnabled( true );
    spbManualDelDays->setEnabled( true );
    switch( grpManualDelMailsRemove->checkedId() )
    {
      case ID_BUTTON_REMOVE_AFTER_DAYS      : spbManualDelDays->setEnabled( true ); break;
      case ID_BUTTON_REMOVE_AT_EXIT         : spbManualDelDays->setEnabled( false ); break;
      default                               : spbManualDelDays->setEnabled( true ); break;
    }
  }
  else
  {
    btnManualDelMailsRemoveExit->setEnabled( false );
    btnManualDelMailsRemoveDays->setEnabled( false );
    spbManualDelDays->setEnabled( false );
  }


}


#include "configlog.moc"
