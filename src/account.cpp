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

Account::Account( QString name, AccountList* accountList, QObject* parent ) 
	: QObject( parent )
{
	this->name = name;
  this->accountList = accountList;

  //create mail container
  mails = new MailList( this );

  //create TCP-Socket
  socket = new QTcpSocket( this );

  //connect the socket with the slots
  connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( slotSocketError( QAbstractSocket::SocketError ) ) );
  connect( socket, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
  connect( socket, SIGNAL( hostFound() ), this, SLOT( slotHostFound() ) );
  connect( socket, SIGNAL( connectionClosed() ), this, SLOT( slotConnectionClosedByServer() ) );

  

  init();
	
	mails->addMail( QString( "1234" ) );
	mails->addMail( QString( "fefe" ) );
	mails->addMail( QString( "23fw" ) );
	mails->addMail( QString( "2rwe" ) );
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
  cout << "Host: " << url.host().toStdString() << endl;
  cout << "Port: " << url.port() << endl;
  cout << "User: " << url.user().toStdString() << endl;
  cout << "Password: " << url.password().toStdString() << endl;
  
	
	//print the mails
  mails->print();
}

void Account::init()
{
  //set active
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

  kdDebug() << "refresh " << getName() << endl;

  doConnect();
  
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

void Account::doConnect()
{
  //break, if there is already a connection
  if( socket->state() != QTcpSocket::UnconnectedState )
  {
    //handleError( "Already connected" );
    closeConnection();
    //return;
  }

  initBeforeConnect();

  //do connect
  socket->connectToHost( getHost(), getPort() );
}

void Account::closeConnection()
{
  if( socket->state() != QTcpSocket::UnconnectedState && socket->state() != QTcpSocket::ClosingState )
  {
    kdDebug() << "Close Connection: " << getName() << endl;
    socket->close();
  }
}

void Account::initBeforeConnect()
{
}

void Account::slotConnected()
{
  kdDebug() << getName() << ": connected with " << getHost() << endl;
  closeConnection();
}

void Account::slotHostFound()
{
  kdDebug() << getName() << ": Host " << getHost() << " found." << endl;
}

void Account::slotSocketError( QAbstractSocket::SocketError ErrorCode)
{
  QString message;    //the error message
  switch( ErrorCode )
  {
    case QAbstractSocket::ConnectionRefusedError    : message = i18n( "Connection refused" ); break;
    case QAbstractSocket::HostNotFoundError         : message = QString( i18n( "Host not found: %1" ).arg( getHost() ) ); break;
    default                                    : message = i18n( "Unknown connection error" ); break;
  }

  //show error and handle all other
  handleError( message );

}

void Account::handleError( QString error )
{
  //close connection
  closeConnection();

  //show error

  emit sigMessageWindowOpened();
  KMessageBox::error( NULL, i18n( "Account %1: %2" ).arg( getName() ).arg( error ) );
  emit sigMessageWindowClosed();

  //emit ready signal
//   switch( state )
//   {
//     case AccountDeleting    : emit sigDeleteReady( getAccountName() ); break;
//     case AccountDownloading : emit sigShowBodiesReady( getAccountName() ); break;
//     case AccountRefreshing  : emit sigRefreshReady( getAccountName() ); break;
//     default                 : break;
//   }
// 
//   //set state
//   state = AccountIdle;


}



