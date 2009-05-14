//
// C++ Implementation: configaccounts
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
  accountListView = new QTableWidget( this );
	accountListView->setColumnCount( 1 );
	accountListView->setHorizontalHeaderLabels( QStringList( i18n( "Name" ) ) );
	//accountListView->setRowCount(1);
	//accountListView->setItem(0, 0, )
	
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
  config = KApplication::kApplication()->sessionConfig();

  //load configured values
  load();

}

ConfigAccounts::~ConfigAccounts()
{
}

void ConfigAccounts::load()
{
/*  //get list of account names
  config->setGroup( CONFIG_GROUP_ACCOUNTS );
  QStringList accounts = config->readListEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

  //create list view items and order accounts to load their config
  for( QStringList::Iterator it = accounts.begin(); it != accounts.end(); ++it )
  {
    //create item
    AccountSetupItem* item = new AccountSetupItem( AccountListView, *it );

    //load item config
    item->load();

  }*/
}

void ConfigAccounts::save()
{
/*  config->setGroup( CONFIG_GROUP_ACCOUNTS );

  //get old account list from config file to remove old account entries
  QStringList oldList = config->readListEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

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
    item = (AccountSetupItem*)( AccountListView->itemAtIndex( index ) );
    if( item != NULL )
    {
      index++;
      accounts.append( item->getAccountName() );
    }
  } while( item != NULL );

  config->writeEntry( CONFIG_ENTRY_ACCOUNTS_LIST, accounts ); //write list of account names

  //order the items to save their configuration
  index = 0;
  item = NULL;
  do
  {
    item = (AccountSetupItem*)( AccountListView->itemAtIndex( index ) );
    if( item != NULL )
    {
      index++;
      item->save();
    }
  } while( item != NULL );

  //write configuration to disk
  config->sync();*/
}

void ConfigAccounts::defaults()
{
}

void ConfigAccounts::slotChanged( )
{
//   KCModule::changed();
}

void ConfigAccounts::slotAdd( )
{
/*  //open setup dialog
  AccountSetupDialog* dlg = new AccountSetupDialog( this, AccountListView, NULL );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialogBase::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;*/
}

void ConfigAccounts::slotEdit( )
{
/*  //get selected item
  AccountSetupItem* account = (AccountSetupItem*)( AccountListView->selectedItem() );

  //test item
  if( account == NULL )
    return;

  //open dialog
  AccountSetupDialog* dlg = new AccountSetupDialog( this, AccountListView, account );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialogBase::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;*/
}

void ConfigAccounts::slotRemove( )
{
/*  //get selected item
  AccountSetupItem* account = (AccountSetupItem*)( AccountListView->selectedItem() );

  //test item
  if( account == NULL )
    return;

  //remove item
  int result = KMessageBox::questionYesNo( this, i18n( "Do you really want to remove account %1?").arg( account->getAccountName() ) );
  if( result == KMessageBox::Yes )
  {
    delete account;
    slotChanged();
  }*/
}


