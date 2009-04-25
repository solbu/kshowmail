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

#include "./account.h"

Account::Account( QString name, QObject* parent ) 
	: QObject( parent )
{
	this->name = name;

  init();
	
	addMail( QString( "1234" ) );
	addMail( QString( "fefe" ) );
	addMail( QString( "23fw" ) );
	addMail( QString( "2rwe" ) );
}

Account::~Account()
{
}

QString Account::getName() const
{
	return name;
}

void Account::print() const
{
	//print the account data
	cout << getName().toStdString() << endl;
	cout << "====================" << endl;
	
	//print the mails
	QListIterator<Mail*> iter( mails );
	while( iter.hasNext() )
	{
		Mail* mail = iter.next();
		mail->print();
		cout << "------------------" << endl;
	}
}

void Account::addMail( const QString& unid )
{
	//create the mail object
	Mail* mail = new Mail( unid, this );
	
	//append it to the list
	mails.append( mail );
}

void Account::setServer( const QString& server )
{
  this->server = server;
}

QString Account::getServer() const
{
  return server;
}

void Account::init()
{
  active = false;
}

bool Account::isActive( ) const
{
  return active;
}

