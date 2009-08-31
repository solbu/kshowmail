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
}

void Account::load()
{
  KConfigGroup* accountConfig = new KConfigGroup( KGlobal::config(), getName() );

  setHost( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_SERVER, DEFAULT_ACCOUNT_SERVER ) );
  setProtocol( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, DEFAULT_ACCOUNT_PROTOCOL ) );
  setPort( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PORT, DEFAULT_ACCOUNT_PORT_POP3 ) );
  setUser( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_USER, DEFAULT_ACCOUNT_USER ) );
  passwordStorage = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, DEFAULT_ACCOUNT_PASSWORD_STORAGE );

  if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
    setPassword( decrypt( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, DEFAULT_ACCOUNT_PASSWORD ) ) );
  else if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    setPassword( KWalletAccess::getPassword( getName() ) );
  else
    setPassword( QString::null );

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

  delete accountConfig;
}

void Account::refreshMailList()
{
  //do nothing, if this account is not active
  if( !isActive() )
  {
    emit sigRefreshReady( getName() );
    return;
  } 

  //check whether we have a password for this account
  //if not, ask for it
  //return when no password is available
  if( !assertPassword() )
  {
    emit sigRefreshReady( name );
    kdDebug() << "No Password" << endl;
    return;
  }

  kdDebug() << "Password: " << getPassword() << endl;
  kdDebug() << "refresh " << getName() << endl;
  emit sigRefreshReady( getName() );
}

bool Account::hasPassword( ) const
{
  return url.hasPass();
}

QString Account::getPassword( ) const
{
  return url.pass();
}

void Account::setPassword( const QString& password )
{
  url.setPass( password );
}

void Account::setHost( const QString& host )
{
  url.setHost( host );
}

void Account::setProtocol( const QString& protocol )
{
  url.setProtocol( protocol );
}

void Account::setPort( unsigned short int port )
{
  url.setPort( port );
}

void Account::setUser( const QString & user )
{
  url.setUser( user );
}

QString Account::getUser( ) const
{
  return url.user();
}

QString Account::getHost( ) const
{
  return url.host();
}

QString Account::getProtocol( bool upperCase ) const
{
  if( upperCase )
    return url.protocol().toUpper();
  else
    return url.protocol();
}

unsigned short int Account::getPort( ) const
{
  return url.port();
}

bool Account::assertPassword( bool force )
{
  //is a password stored?
  if ( !hasPassword() || force )
  {
    //no password found, we will ask the user!
    //set normal cursor
    while( QApplication::overrideCursor() )
      QApplication::restoreOverrideCursor();

    //show password dialog
    KPasswordDialog pwdialog( NULL );
    pwdialog.setPrompt( i18n( "Please type in the password for %1" ).arg( getName() ) );
    int result = pwdialog.exec();

    //set waiting cursor
    QApplication::setOverrideCursor( Qt::WaitCursor );

    //let's look, what the user has done :o)
    if( result == KPasswordDialog::Accepted )
    {
      //the user has clicked OK in the password dialog
      //store the password
      setPassword( pwdialog.password() );

      //save password in file or KWallet
      KConfigGroup* accountConfig = new KConfigGroup( KGlobal::config(), getName() );

      if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
        accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, crypt( url ) );
      else
        accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, "" );

      if( passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
      {
        KWalletAccess::savePassword( getName(), url.pass() );
      }

      accountConfig->sync();

      delete accountConfig;

      //emit configuration changed signal
      emit ( sigConfigChanged() );

      //tell we have a password
      return true;
    }
    else
      //the user has clicked Cancel in the password dialog; we don't have a password
      return false;
  }
  else
    //we have already a password for this account
    return true;

}

void Account::setPasswordStorage( int storage )
{
  if( storage == CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE ||
      storage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE ||
      storage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )

    passwordStorage = storage;

  else

    passwordStorage =  DEFAULT_ACCOUNT_PASSWORD_STORAGE;
}

int Account::getPasswordStorage( ) const
{
  return passwordStorage;
}


