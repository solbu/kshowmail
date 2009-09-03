/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "./accountlist.h"

AccountList::AccountList( QObject* parent ) : QObject( parent )
{
	
}

AccountList::~AccountList(){}

Account* AccountList::addAccount( const QString& name )
{
	//create account object
	Account* acc = new Account( name, this, this );

  //conect the signals with the slots of this list
  connect( acc, SIGNAL( sigRefreshReady( QString ) ), this, SLOT( slotCheckRefreshState( QString ) ) );
	
	//append it to the list
	accounts.append( acc );

  return acc;
}

void AccountList::print() const
{
	//print the accounts
	QListIterator<Account*> iter( accounts );
	while( iter.hasNext() )
	{
		Account* acc = iter.next();
		acc->print();
		cout << "**********************" << endl;
	}
}

int AccountList::numberAccounts() const
{
	return accounts.size();
}

Account* AccountList::getAccount( int index ) const
{
  //check the index
  if( index > accounts.size() ) return NULL;

  //return the account if the index is valid
  return accounts.at( index );
}

void AccountList::loadSetup()
{
  //get account names from the config file
  KConfigGroup* confAccounts = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNTS );
  QStringList accountsConfig = confAccounts->readEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

  //remove deleted accounts from the account list
  //accounts are deleted, if they are in AccountList yet, but not in the list of the config file (accounts)
  QList<Account*>::iterator iter;
  iter = accounts.begin();
  while( iter != accounts.end() )
  {
    Account* accDel = *iter;
    QString accName = accDel->getName();

    if( !accountsConfig.contains( accName ) )
    {
      iter = accounts.erase( iter );
      delete accDel;
    }
    else
    {
      iter++;
    }
  }

  //iterate over all entries in the config account list and create a new account if necessary
  QStringListIterator iterAccountsConfig( accountsConfig );
  while( iterAccountsConfig.hasNext() )
  {
    QString accName = iterAccountsConfig.next();

    if( !hasAccount( accName ) )
    {
      addAccount( accName );
    }
  }
	
  //load the setup of all accounts
  iter = accounts.begin();
  while( iter != accounts.end() )
  {
    Account* acc = *iter;
		
		acc->load();
		
		iter++;
  }

}

bool AccountList::hasAccount( QString accountName ) const
{
  QListIterator<Account*> iter( accounts );
  while( iter.hasNext() )
  {
    Account* acc = iter.next();
    if( acc->getName() == accountName ) return true;
  }
  return false;
}

void AccountList::refreshMailLists()
{
  //return, if no accounts available
  if( accounts.count() == 0 )
  {
    emit sigRefreshReady();
    return;
  }

  //clear the map, which contains the names of the accounts,
  //which have gotten an order to show mails
  AccountRefreshMap.clear();

  //inserts an item for every account which will get an order to refresh
  //its mail list. The key is the account name and the data is TRUE.
  //it is important to do this in a seperate iteration because this avoids
  //race conditions
  QList<Account*>::const_iterator iter;
  for( iter = accounts.constBegin(); iter != accounts.constEnd(); iter++ )
  {
    Account* account = *iter;
    
    //insert item
    AccountRefreshMap.insert( account->getName(), true );
  }

  //iterate over all accounts and order them to refresh the mail lists
  for( iter = accounts.constBegin(); iter != accounts.constEnd(); iter++ )
  {
    Account* account = *iter;

    account->refreshMailList();
  }
}

void AccountList::slotCheckRefreshState( QString account )
{
  bool accountRefreshing = false;    //set to TRUE if an account is still refreshing
  AccountTaskMap_Type::Iterator it;   //iterator over the account map

  //set the appropriate item in AccountRefreshMap to FALSE
  AccountRefreshMap[ account ] = false;

  //iterate over the account map to check whether all accounts
  //are ready
  for ( it = AccountRefreshMap.begin(); it != AccountRefreshMap.end(); ++it )
  {
    if( *it == true )
      accountRefreshing = true;
  }

  //emit sigRefreshReady if all accounts are ready
  if( !accountRefreshing )
  {
    emit sigRefreshReady();
  }

}
