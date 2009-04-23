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
	Account* acc = addAccount( "GMX" );
  acc->setServer( "pop3.gmx.de" );
	acc = addAccount( "Freenet" );
  acc->setServer( "pop3.freenet.de" );
	acc = addAccount( "Arcor" );
  acc->setServer( "pop.arcor.de" );
	acc = addAccount( "Versatel" );
  acc->setServer( "pop3.versatel.de" );
	
	print();
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