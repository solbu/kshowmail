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
	Account* acc = new Account( name, this );
	
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
  //load the setup of all accounts
  QStringListIterator iterAccountsConfig( accountsConfig );
  while( iterAccountsConfig.hasNext() )
  {
    QString accName = iterAccountsConfig.next();

    if( !hasAccount( accName ) )
    {
      addAccount( accName );
    }
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
