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
	cout << "Active: " << isActive() << endl;
	
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
  active = true;
}

bool Account::isActive( ) const
{
  return active;
}

void Account::setActive(bool active) {
	this->active = active;
	print();
}

void Account::load()
{
  KConfigGroup* accountConfig = new KConfigGroup( KGlobal::config(), getName() );

  server = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_SERVER, DEFAULT_ACCOUNT_SERVER );
  protocol = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, DEFAULT_ACCOUNT_PROTOCOL );
  port = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PORT, DEFAULT_ACCOUNT_PORT_POP3 );
  user = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_USER, DEFAULT_ACCOUNT_USER );
  int passwordStorage = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, DEFAULT_ACCOUNT_PASSWORD_STORAGE );

  if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
    password = decrypt( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, DEFAULT_ACCOUNT_PASSWORD ) );
  else if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    password = KWalletAccess::getPassword( getName() );
  else
    password = QString::null;

  active = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, DEFAULT_ACCOUNT_ACTIVE );
  
	int intTransferSecurity = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, DEFAULT_ACCOUNT_SECTRANSFER );
	if( intTransferSecurity == CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE )
		transferSecurity = TransSecNone;
	else if( intTransferSecurity == CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL )
		transferSecurity = TransSecSSL;
	else if( intTransferSecurity == CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS )
		transferSecurity = TransSecTLS;
	else
		transferSecurity = TransSecNone;
}

