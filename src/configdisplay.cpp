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

#include "configdisplay.h"

K_PLUGIN_FACTORY( ConfigDisplayFactory, registerPlugin<ConfigDisplay>(); )
K_EXPORT_PLUGIN( ConfigDisplayFactory( "kcm_kshowmailconfigdisplay" ) )

ConfigDisplay::ConfigDisplay( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigDisplayFactory::componentData(), parent, args )
{
  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this );

  //group boxes
  QGroupBox* gboxAccountlist = new QGroupBox( i18nc( "@title:group appearance options of the account view", "Account list" ), this );
  layMain->addWidget( gboxAccountlist );
  QGroupBox* gboxMessagelist = new QGroupBox( i18nc( "@title:group appearance options of the mail view", "Mail list" ), this );
  layMain->addWidget( gboxMessagelist );
  QGroupBox* gboxMessageContent = new QGroupBox( i18nc( "@title:group appearance options of the mail content view", "Mail content" ), this );
  layMain->addWidget( gboxMessageContent );

  //group box layouts
  QGridLayout* layAccountlist = new QGridLayout();
  QGridLayout* layMessagelist = new QGridLayout();
  QGridLayout* layMessageContent = new QGridLayout();
  gboxAccountlist->setLayout( layAccountlist );
  gboxMessagelist->setLayout( layMessagelist );
  gboxMessageContent->setLayout( layMessageContent );

  //create items
  chkAccountlistActive = new QCheckBox( i18nc( "@option:check show column 'active' in the account view", "Active" ), gboxAccountlist );
  chkAccountlistActive->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Active' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistActive, 0, 0 );

  chkAccountlistAccount = new QCheckBox( i18nc( "@option:check show account column in the account view", "Account" ), gboxAccountlist );
  chkAccountlistAccount->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Account' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistAccount, 0, 1 );

  chkAccountlistServer = new QCheckBox( i18nc( "@option:check show server column in the account view", "Server" ), gboxAccountlist );
  chkAccountlistServer->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Server' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistServer, 1, 0 );

  chkAccountlistUser = new QCheckBox( i18nc( "@option:check show user column in the account view", "User" ), gboxAccountlist );
  chkAccountlistUser->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'User' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistUser, 1, 1 );

  chkAccountlistMessages = new QCheckBox( i18nc( "@option:check show number of messages column in the account view", "Messages" ), gboxAccountlist );
  chkAccountlistMessages->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Messages' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistMessages, 2, 0 );

  chkAccountlistSize = new QCheckBox( i18nc( "@option:check show column about the entire size of mails in the account view", "Size" ), gboxAccountlist );
  chkAccountlistSize->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Size' column in the account list" ) );
  layAccountlist->addWidget( chkAccountlistSize, 2, 1 );

  chkMessagelistNumber = new QCheckBox( i18nc( "@option:check show mail number column in the mail view", "Number" ), gboxMessagelist );
  chkMessagelistNumber->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Number' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistNumber, 0, 0 );

  chkMessagelistAccount = new QCheckBox( i18nc( "@option:check show account name column in the mail view", "Account" ), gboxMessagelist );
  chkMessagelistAccount->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Account' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistAccount, 0, 1 );

  chkMessagelistFrom = new QCheckBox( i18nc( "@option:check show sender column in the mail view", "From" ), gboxMessagelist );
  chkMessagelistFrom->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'From' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistFrom, 1, 0 );

  chkMessagelistTo = new QCheckBox( i18nc( "@option:check show addressee column in mail view", "To" ), gboxMessagelist );
  chkMessagelistTo->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'To' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistTo, 1, 1 );

  chkMessagelistSubject = new QCheckBox( i18nc( "@option:check show subject column in mail view", "Subject" ), gboxMessagelist );
  chkMessagelistSubject->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Subject' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistSubject, 2, 0 );

  chkMessagelistDate = new QCheckBox( i18nc( "@option:check show send date column in the mail view", "Date" ), gboxMessagelist );
  chkMessagelistDate->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Date' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistDate, 2, 1 );

  chkMessagelistSize = new QCheckBox( i18nc( "@option:check show mail size column in the mail view", "Size" ), gboxMessagelist );
  chkMessagelistSize->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Size' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistSize, 3, 0 );

  chkMessagelistContent = new QCheckBox( i18nc( "@option:check show content type column in mail view", "Content" ), gboxMessagelist );
  chkMessagelistContent->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'Content' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistContent, 3, 1 );

  chkMessagelistState = new QCheckBox( i18nc( "@option:check show state column in mail view", "State" ), gboxMessagelist );
  chkMessagelistState->setToolTip( i18nc( "@info:tooltip", "To switch on/off the 'State' column in the message list" ) );
  layMessagelist->addWidget( chkMessagelistState, 4, 0 );

  chkAllowHTML = new QCheckBox( i18nc( "@option:check allow the mail content view to process HTML", "Allow HTML" ), gboxMessageContent );
  chkAllowHTML->setToolTip( i18nc( "@info:tooltip", "To switch on/off HTML in the message view") );
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
  config = KGlobal::config();

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
  KConfigGroup* configAccList = new KConfigGroup( config, CONFIG_GROUP_ACCOUNT_LIST );

  chkAccountlistActive->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE, DEFAULT_DISPLAY_ACCOUNT_ACTIVE ) );
  chkAccountlistAccount->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT, DEFAULT_DISPLAY_ACCOUNT_ACCOUNT ) );
  chkAccountlistServer->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER, DEFAULT_DISPLAY_ACCOUNT_SERVER ) );
  chkAccountlistUser->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_USER, DEFAULT_DISPLAY_ACCOUNT_USER ) );
  chkAccountlistMessages->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES, DEFAULT_DISPLAY_ACCOUNT_MESSAGES ) );
  chkAccountlistSize->setChecked( configAccList->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE, DEFAULT_DISPLAY_ACCOUNT_SIZE ) );

  KConfigGroup* configMessList = new KConfigGroup( config, CONFIG_GROUP_MESSAGE_LIST );

  chkMessagelistNumber->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER, DEFAULT_DISPLAY_MESSAGE_NUMBER ) );
  chkMessagelistAccount->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT, DEFAULT_DISPLAY_MESSAGE_ACCOUNT ) );
  chkMessagelistFrom->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_FROM, DEFAULT_DISPLAY_MESSAGE_FROM ) );
  chkMessagelistTo->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_TO, DEFAULT_DISPLAY_MESSAGE_TO ) );
  chkMessagelistSubject->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT, DEFAULT_DISPLAY_MESSAGE_SUBJECT ) );
  chkMessagelistDate->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_DATE, DEFAULT_DISPLAY_MESSAGE_DATE ) );
  chkMessagelistSize->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE, DEFAULT_DISPLAY_MESSAGE_SIZE ) );
  chkMessagelistContent->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT, DEFAULT_DISPLAY_MESSAGE_CONTENT ) );
  chkMessagelistState->setChecked( configMessList->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_STATE, DEFAULT_DISPLAY_MESSAGE_STATE ) );

  KConfigGroup* configView = new KConfigGroup( config, CONFIG_GROUP_VIEW );

  chkAllowHTML->setChecked( configView->readEntry( CONFIG_ENTRY_VIEW_USE_HTML, DEFAULT_VIEW_USE_HTML ) );

  delete configAccList;
  delete configMessList;
  delete configView;

}

void ConfigDisplay::save()
{
  KConfigGroup* configAccList = new KConfigGroup( config, CONFIG_GROUP_ACCOUNT_LIST );

  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE, chkAccountlistActive->isChecked() );
  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT, chkAccountlistAccount->isChecked() );
  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER, chkAccountlistServer->isChecked() );
  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_USER, chkAccountlistUser->isChecked() );
  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES, chkAccountlistMessages->isChecked() );
  configAccList->writeEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE, chkAccountlistSize->isChecked() );

  KConfigGroup* configMessList = new KConfigGroup( config, CONFIG_GROUP_MESSAGE_LIST );

  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER, chkMessagelistNumber->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT, chkMessagelistAccount->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_FROM, chkMessagelistFrom->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_TO, chkMessagelistTo->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT, chkMessagelistSubject->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_DATE, chkMessagelistDate->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE, chkMessagelistSize->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT, chkMessagelistContent->isChecked() );
  configMessList->writeEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_STATE, chkMessagelistState->isChecked() );

  KConfigGroup* configView = new KConfigGroup( config, CONFIG_GROUP_VIEW );

  configView->writeEntry( CONFIG_ENTRY_VIEW_USE_HTML, chkAllowHTML->isChecked() );

  //write configuration to disk
  config->sync();

  delete configAccList;
  delete configMessList;
  delete configView;
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
