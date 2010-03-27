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

#include "configaccounts.h"

K_PLUGIN_FACTORY( ConfigAccountsFactory, registerPlugin<ConfigAccounts>(); )
K_EXPORT_PLUGIN( ConfigAccountsFactory( "kcm_kshowmailconfigaccounts" ) )
 
ConfigAccounts::ConfigAccounts( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigAccountsFactory::componentData(), parent, args )
{
  //build GUI
  //---------

  //main layout
  QHBoxLayout* layMain = new QHBoxLayout( this );

  //account list view
  accountListView = new QTreeWidget( this );
	accountListView->setColumnCount( 1 );
	accountListView->setHeaderLabels( QStringList( i18n( "Name" ) ) );
  accountListView->setIndentation( 0 );
	
  layMain->addWidget( accountListView );

  //button layout
  QVBoxLayout* layButtons = new QVBoxLayout();
	layMain->addLayout( layButtons );

  //Buttons
  btnAdd = new KPushButton( KStandardGuiItem::add(), this );
  layButtons->addWidget( btnAdd );
  btnAdd->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  connect( btnAdd, SIGNAL( clicked() ), this, SLOT( slotAdd() ) );

  btnEdit = new KPushButton( KStandardGuiItem::configure(), this );
  layButtons->addWidget( btnEdit );
  btnEdit->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  connect( btnEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );

  btnRemove = new KPushButton( KStandardGuiItem::remove(), this );
  layButtons->addWidget( btnRemove );
  btnRemove->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  connect( btnRemove, SIGNAL( clicked() ), this, SLOT( slotRemove() ) );

  layButtons->addItem( new QSpacerItem( 1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding ) );


  //get application config object (kshowmailrc)
  config = KGlobal::config();

}

ConfigAccounts::~ConfigAccounts()
{
}

void ConfigAccounts::load()
{
  //get list of account names
  KConfigGroup* configAcc = new KConfigGroup( config, CONFIG_GROUP_ACCOUNTS );
  QStringList accounts = configAcc->readEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

  //create list view items and order accounts to load their config
  for( QStringList::Iterator it = accounts.begin(); it != accounts.end(); ++it )
  {
    //create item
    AccountSetupItem* item = new AccountSetupItem( accountListView, *it );

    //load item config
    item->load();

  }
}

void ConfigAccounts::save()
{
  KConfigGroup grpAccounts =  config->group( CONFIG_GROUP_ACCOUNTS );

  //get old account list from config file to remove old account entries
  QStringList oldList = grpAccounts.readEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

  //remove all account entries
  for( QStringList::Iterator it = oldList.begin(); it != oldList.end(); ++it )
  {
    config->deleteGroup( *it );
  }


  //write a list with all account names into the config
  QStringList accounts;       //list of all account names
  AccountSetupItem* item = NULL;
  int index = 0;

  do  //get all account names
  {
    item = (AccountSetupItem*)( accountListView->topLevelItem( index ) );
    if( item != NULL )
    {
      index++;
      accounts.append( item->getAccountName() );
    }
  } while( item != NULL );

  grpAccounts.writeEntry( CONFIG_ENTRY_ACCOUNTS_LIST, accounts ); //write list of account names

  //order the items to save their configuration
  index = 0;
  item = NULL;
  do
  {
    item = (AccountSetupItem*)( accountListView->topLevelItem( index ) );
    if( item != NULL )
    {
      index++;
      item->save();
    }
  } while( item != NULL );

  //write configuration to disk
  config->sync();
}

void ConfigAccounts::defaults()
{
}

void ConfigAccounts::slotChanged( )
{
   KCModule::changed();
}

void ConfigAccounts::slotAdd( )
{
  //open setup dialog
  QPointer<AccountSetupDialog> dlg = new AccountSetupDialog( this, accountListView, NULL );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialog::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;
}

void ConfigAccounts::slotEdit( )
{
  //get selected item
  AccountSetupItem* account = (AccountSetupItem*)( accountListView->currentItem() );

  //test item
  if( account == NULL )
    return;

  //open dialog
  AccountSetupDialog* dlg = new AccountSetupDialog( this, accountListView, account );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialog::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;
}

void ConfigAccounts::slotRemove( )
{
  //get selected item
  AccountSetupItem* account = (AccountSetupItem*)( accountListView->currentItem() );

  //test item
  if( account == NULL )
    return;

  //remove item
  int result = KMessageBox::questionYesNo( this, i18n( "Do you really want to remove account %1?").arg( account->getAccountName() ) );
  if( result == KMessageBox::Yes )
  {
    delete account;
    slotChanged();
  }
}


