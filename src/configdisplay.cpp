//
// C++ Implementation: configdisplay
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configdisplay.h"

typedef KGenericFactory<ConfigDisplay, QWidget> ConfigDisplayFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfigdisplay, ConfigDisplayFactory(
    "kcm_kshowmailconfigdisplay" ) );

ConfigDisplay::ConfigDisplay( QWidget * parent, const char * name, const QStringList & args )
  : KCModule( ConfigDisplayFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configdisplay" );

  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //group boxes
  QGroupBox* gboxAccountlist = new QGroupBox( 0, Qt::Horizontal, i18n( "Account list" ), this, "gboxAccountlist" );
  layMain->addWidget( gboxAccountlist );
  QGroupBox* gboxMessagelist = new QGroupBox( 0, Qt::Horizontal, i18n( "Mail list" ), this, "gboxMessagelist" );
  layMain->addWidget( gboxMessagelist );
  QGroupBox* gboxMessageContent = new QGroupBox( 0, Qt::Horizontal, i18n( "Mail content" ), this, "gboxMessageContent" );
  layMain->addWidget( gboxMessageContent );

  //group box layouts
  QGridLayout* layAccountlist = new QGridLayout( gboxAccountlist->layout(), 3, 2, 10 );
  QGridLayout* layMessagelist = new QGridLayout( gboxMessagelist->layout(), 5, 2, 10 );
  QGridLayout* layMessageContent = new QGridLayout( gboxMessageContent->layout(), 1, 1, 10 );

  //create items
  chkAccountlistActive = new QCheckBox( i18n( "Active" ), gboxAccountlist, "chkAccountlistActive" );
  QToolTip::add( chkAccountlistActive, i18n( "To switch on/off the 'Active' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistActive, 0, 0 );

  chkAccountlistAccount = new QCheckBox( i18n( "Account" ), gboxAccountlist, "chkAccountlistAccount" );
  QToolTip::add( chkAccountlistAccount, i18n( "To switch on/off the 'Account' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistAccount, 0, 1 );

  chkAccountlistServer = new QCheckBox( i18n( "Server" ), gboxAccountlist, "chkAccountlistServer" );
  QToolTip::add( chkAccountlistServer, i18n( "To switch on/off the 'Server' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistServer, 1, 0 );

  chkAccountlistUser = new QCheckBox( i18n( "User" ), gboxAccountlist, "chkAccountlistUser" );
  QToolTip::add( chkAccountlistUser, i18n( "To switch on/off the 'User' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistUser, 1, 1 );

  chkAccountlistMessages = new QCheckBox( i18n( "Messages" ), gboxAccountlist, "chkAccountlistMessages" );
  QToolTip::add( chkAccountlistMessages, i18n( "To switch on/off the 'Messages' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistMessages, 2, 0 );

  chkAccountlistSize = new QCheckBox( i18n( "Si&ze" ), gboxAccountlist, "chkAccountlistSize" );
  QToolTip::add( chkAccountlistSize, i18n( "To switch on/off the 'Size' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistSize, 2, 1 );

  chkMessagelistNumber = new QCheckBox( i18n( "Number" ), gboxMessagelist, "chkMessagelistNumber" );
  QToolTip::add( chkMessagelistNumber, i18n( "To switch on/off the 'Number' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistNumber, 0, 0 );

  chkMessagelistAccount = new QCheckBox( i18n( "Account" ), gboxMessagelist, "chkMessagelistAccount" );
  QToolTip::add( chkMessagelistAccount, i18n( "To switch on/off the 'Account' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistAccount, 0, 1 );

  chkMessagelistFrom = new QCheckBox( i18n( "From" ), gboxMessagelist, "chkMessagelistFrom" );
  QToolTip::add( chkMessagelistFrom, i18n( "To switch on/off the 'From' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistFrom, 1, 0 );

  chkMessagelistTo = new QCheckBox( i18n( "To" ), gboxMessagelist, "chkMessagelistTo" );
  QToolTip::add( chkMessagelistTo, i18n( "To switch on/off the 'To' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistTo, 1, 1 );

  chkMessagelistSubject = new QCheckBox( i18n( "Su&bject" ), gboxMessagelist, "chkMessagelistSubject" );
  QToolTip::add( chkMessagelistSubject, i18n( "To switch on/off the 'Subject' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistSubject, 2, 0 );

  chkMessagelistDate = new QCheckBox( i18n( "Date" ), gboxMessagelist, "chkMessagelistDate" );
  QToolTip::add( chkMessagelistDate, i18n( "To switch on/off the 'Date' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistDate, 2, 1 );

  chkMessagelistSize = new QCheckBox( i18n( "Size" ), gboxMessagelist, "chkMessagelistSize" );
  QToolTip::add( chkMessagelistSize, i18n( "To switch on/off the 'Size' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistSize, 3, 0 );

  chkMessagelistContent = new QCheckBox( i18n( "Content" ), gboxMessagelist, "chkMessagelistContent" );
  QToolTip::add( chkMessagelistContent, i18n( "To switch on/off the 'Content' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistContent, 3, 1 );

  chkMessagelistState = new QCheckBox( i18n( "State" ), gboxMessagelist, "chkMessagelistState" );
  QToolTip::add( chkMessagelistState, i18n( "To switch on/off the 'State' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistState, 4, 0 );

  chkAllowHTML = new QCheckBox( i18n( "Allow HTML" ), gboxMessageContent, "chkAllowHTML" );
  QToolTip::add( chkAllowHTML, i18n( "To switch on/off HTML in the message view") );
  layMessageContent->addWidget( chkAllowHTML, 0, 0 );

  //connect all configuration itmes with slot changed() to notify the dialog about changes
  connect( chkAccountlistActive, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAccountlistAccount, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAccountlistServer, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAccountlistUser, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAccountlistMessages, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAccountlistSize, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistNumber, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistAccount, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistFrom, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistTo, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistSubject, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistDate, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistSize, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistContent, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkMessagelistState, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );
  connect( chkAllowHTML, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );


  //get application config object (kshowmailrc)
  config = KApplication::kApplication()->config();

  //load configurated values
  load();
}

ConfigDisplay::~ConfigDisplay()
{
}

void ConfigDisplay::slotChanged( )
{
  KCModule::changed();
}

void ConfigDisplay::load( )
{
  config->setGroup( CONFIG_GROUP_ACCOUNT_LIST );
  chkAccountlistActive->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE, DEFAULT_DISPLAY_ACCOUNT_ACTIVE ) );
  chkAccountlistAccount->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT, DEFAULT_DISPLAY_ACCOUNT_ACCOUNT ) );
  chkAccountlistServer->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER, DEFAULT_DISPLAY_ACCOUNT_SERVER ) );
  chkAccountlistUser->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_USER, DEFAULT_DISPLAY_ACCOUNT_USER ) );
  chkAccountlistMessages->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES, DEFAULT_DISPLAY_ACCOUNT_MESSAGES ) );
  chkAccountlistSize->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE, DEFAULT_DISPLAY_ACCOUNT_SIZE ) );

  config->setGroup( CONFIG_GROUP_MESSAGE_LIST );
  chkMessagelistNumber->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER, DEFAULT_DISPLAY_MESSAGE_NUMBER ) );
  chkMessagelistAccount->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT, DEFAULT_DISPLAY_MESSAGE_ACCOUNT ) );
  chkMessagelistFrom->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_FROM, DEFAULT_DISPLAY_MESSAGE_FROM ) );
  chkMessagelistTo->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_TO, DEFAULT_DISPLAY_MESSAGE_TO ) );
  chkMessagelistSubject->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT, DEFAULT_DISPLAY_MESSAGE_SUBJECT ) );
  chkMessagelistDate->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_DATE, DEFAULT_DISPLAY_MESSAGE_DATE ) );
  chkMessagelistSize->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE, DEFAULT_DISPLAY_MESSAGE_SIZE ) );
  chkMessagelistContent->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT, DEFAULT_DISPLAY_MESSAGE_CONTENT ) );
  chkMessagelistState->setChecked( config->readBoolEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_STATE, DEFAULT_DISPLAY_MESSAGE_STATE ) );

  config->setGroup( CONFIG_GROUP_VIEW );
  chkAllowHTML->setChecked( config->readBoolEntry( CONFIG_ENTRY_VIEW_USE_HTML, DEFAULT_VIEW_USE_HTML ) );

}

void ConfigDisplay::save()
{
  config->setGroup( CONFIG_GROUP_ACCOUNT_LIST );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE, chkAccountlistActive->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT, chkAccountlistAccount->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER, chkAccountlistServer->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_USER, chkAccountlistUser->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES, chkAccountlistMessages->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE, chkAccountlistSize->isChecked() );

  config->setGroup( CONFIG_GROUP_MESSAGE_LIST );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER, chkMessagelistNumber->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT, chkMessagelistAccount->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_FROM, chkMessagelistFrom->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_TO, chkMessagelistTo->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT, chkMessagelistSubject->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_DATE, chkMessagelistDate->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE, chkMessagelistSize->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT, chkMessagelistContent->isChecked() );
  config->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_STATE, chkMessagelistState->isChecked() );

  config->setGroup( CONFIG_GROUP_VIEW );
  config->writeEntry( CONFIG_ENTRY_VIEW_USE_HTML, chkAllowHTML->isChecked() );

  //write configuration to disk
  config->sync();
}

void ConfigDisplay::defaults()
{
  chkAccountlistActive->setChecked( DEFAULT_DISPLAY_ACCOUNT_ACTIVE );
  chkAccountlistAccount->setChecked( DEFAULT_DISPLAY_ACCOUNT_ACCOUNT );
  chkAccountlistServer->setChecked( DEFAULT_DISPLAY_ACCOUNT_SERVER );
  chkAccountlistUser->setChecked( DEFAULT_DISPLAY_ACCOUNT_USER );
  chkAccountlistMessages->setChecked( DEFAULT_DISPLAY_ACCOUNT_MESSAGES );
  chkAccountlistSize->setChecked( DEFAULT_DISPLAY_ACCOUNT_SIZE );

  chkMessagelistNumber->setChecked( DEFAULT_DISPLAY_MESSAGE_NUMBER );
  chkMessagelistAccount->setChecked( DEFAULT_DISPLAY_MESSAGE_ACCOUNT );
  chkMessagelistFrom->setChecked( DEFAULT_DISPLAY_MESSAGE_FROM );
  chkMessagelistTo->setChecked( DEFAULT_DISPLAY_MESSAGE_TO );
  chkMessagelistSubject->setChecked( DEFAULT_DISPLAY_MESSAGE_SUBJECT );
  chkMessagelistDate->setChecked( DEFAULT_DISPLAY_MESSAGE_DATE );
  chkMessagelistSize->setChecked( DEFAULT_DISPLAY_MESSAGE_SIZE );
  chkMessagelistContent->setChecked( DEFAULT_DISPLAY_MESSAGE_CONTENT );
  chkMessagelistState->setChecked( DEFAULT_DISPLAY_MESSAGE_STATE );

  chkAllowHTML->setChecked( DEFAULT_VIEW_USE_HTML );

}
#include "configdisplay.moc"
