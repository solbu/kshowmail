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

  //Timeout timer
  timeoutTimer = new QTimer( this );

  //at start, we are idle
  state = AccountIdle;

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

  //set state
  state = AccountRefreshing;

  doConnect();
  
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

  //the first task is to read the server greeting message
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotReadFirstServerMessage() ) );
  
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
  quitSent = false;
  apopAvail = false;
}

void Account::slotConnected()
{
}

void Account::slotHostFound()
{
}

void Account::slotSocketError( QAbstractSocket::SocketError ErrorCode)
{
  QString message;    //the error message
  switch( ErrorCode )
  {
    case QAbstractSocket::ConnectionRefusedError    : message = i18n( "Connection refused" ); break;
    case QAbstractSocket::HostNotFoundError         : message = QString( i18n( "Host not found: %1" ).arg( getHost() ) ); break;
    default                                         : message = i18n( "Unknown connection error" ); break;
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

  kdDebug() << state << endl;
  //emit ready signal
  switch( state )
  {
    case AccountDeleting    : emit sigDeleteReady( getName() ); break;
    case AccountDownloading : emit sigShowBodiesReady( getName() ); break;
    case AccountRefreshing  : emit sigRefreshReady( getName() ); break;
    default                 : break;
  }

  //set state
  state = AccountIdle;


}

QStringList Account::readfromSocket()
{
  //buffer for the datas
  char lineBuffer[1024];

  //return object
  QStringList text;

  //wait for a whole line
  while( socket->bytesAvailable() != 0 )
  {
    while( socket->canReadLine() )
    {
      socket->readLine( lineBuffer, sizeof( lineBuffer ) );
      text.append( QString( lineBuffer ).simplified() );
    }
  }

  return text;
}

void Account::slotReadFirstServerMessage()
{
  QStringList text = readfromSocket();

  //it must be a positive response
  if( !isPositiveServerMessage( text ) )
  {
    //it is not a greeting of a POP3 server
    //invoke error handling and close the connection
    handleError( i18n( "%1 is not a POP3 mail server").arg( getHost() ) );
    return;
  }

  //get just the first line
  QString response = text.first();

  //get the APOP timestamp if avalailable
  QRegExp regEx( "<.*>" );
  if( regEx.indexIn( response ) != -1 )
  {
    //we have found a timestamp
    apopAvail = true;

    apopTimestamp = regEx.cap();
  }

  //get the capabilities
  getCapabilities();
}

void Account::sendCommand( const QString& command )
{
  kdDebug() << "Send " << command << " to " << getName() << endl;

  //call error handler, if the socket is not connected
  if( socket->state() != QAbstractSocket::ConnectedState )
  {
    handleError( i18n( "No connect to %1" ).arg( getHost() ) );
    return;
  }


  //the write methode of the socket needs a byte array
  QByteArray data;
  data.append( command );
  data.append( "\n" );
  

  //send it
  qint64 writtenBytes = socket->write( data );

  //if the return value ist -1 a error is occured
  if( writtenBytes == -1 )
  {
    handleError( i18n( "Could not send the command %1 to %2" ).arg( command ).arg( getName() ) );
    return;
  }
  
}

void Account::getCapabilities()
{
  //when the server answer is comming in, the slot slotReceiveCapabilities will be invoked
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotCapabilitiesResponse() ) );

  //send the CAPA-Command
  sendCommand( CAPA_REQUEST );
}

void Account::slotCapabilitiesResponse()
{
  //get server answer
  QStringList text = readfromSocket();

  //have we got capabilities?
  bool haveCapa = isPositiveServerMessage( text );
  
  //set some capabilities flags
  if( haveCapa )
  {
    //remove status indicator and termination char
    clearMessage( text );
  }

  //get authentification mechanism
  getAuthMech();

}

void Account::printServerMessage( QStringList& text ) const
{
  for( int i = 0; i < text.size(); ++i )
  {
    kdDebug() << text.at( i ) << endl;
  }
}

bool Account::isPositiveServerMessage( QStringList& message ) const
{
  //return false, if the list is empty
  if( message.isEmpty() ) return false;

  //a positive message starts with a +OK
  if( message.first().startsWith( RESPONSE_POSITIVE ) ) return true;

  //it is a negative answer
  return false;
}

void Account::clearMessage( QStringList& message )
{

  //return, if the message is empty
  if( message.isEmpty() ) return;
  
  //get the first line
  QString firstLine = message.first();

  //remove the status indicator
  if( firstLine.startsWith( RESPONSE_POSITIVE ) )
  {
    firstLine.remove( 0, RESPONSE_POSITIVE.length() );
    
  } else if ( firstLine.startsWith( RESPONSE_NEGATIVE ) ){

    firstLine.remove( 0, RESPONSE_NEGATIVE.length() );
  }

  //remove spaces at start
  firstLine = firstLine.trimmed();

  //if the firstLine is empty now, we remove it
  //otherwise we replace it with the processed line
  if( firstLine.isEmpty() )
  {
    message.removeFirst();
  }
  else
  {
    message.replace( 0, firstLine );
  }

  //remove termination line
  if( message.last() == END_MULTILINE_RESPONSE )
  {
    message.removeLast();
  }
}

void Account::getAuthMech()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotAuthMechResponse() ) );

  //send the command
  sendCommand( AUTH_REQUEST );
}

void Account::slotAuthMechResponse()
{
  //get server answer
  QStringList text = readfromSocket();

  //have we got capabilities?
  bool haveAuth = isPositiveServerMessage( text );

  //Set some authentification flags
  if( haveAuth )
  {
    //remove status indicator and termination char
    clearMessage( text );
  }


  commit();

}

void Account::commit()
{
  if( socket->state() == QAbstractSocket::ConnectedState )
  {
    //connect the signal readyRead of the socket with the response handle methode
    disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
    connect( socket, SIGNAL( readyRead() ), this, SLOT( slotCommitResponse() ) );

    //set the this flag to avoid an error message, if the server close the connection
    //immediately
    quitSent = true;

    //send the command
    sendCommand( COMMIT );
  }
  else
  {
    finishTask();
  }
}

void Account::finishTask()
{
  kdDebug() << "finishTask " << getName() << endl;

  //stop the timeout timer
  timeoutTimer->stop();

  //close connection
  closeConnection();

  //set state but save the old state
  Types::AccountState_Type oldState = state;
  state = AccountIdle;

  //emit ready signal
  switch( oldState )
  {
    case AccountDeleting    : emit sigDeleteReady( getName() ); break;
    case AccountDownloading : emit sigShowBodiesReady( getName() ); break;
    case AccountRefreshing  : emit sigRefreshReady( getName() ); break;
    default                 : break;
  }


}

void Account::slotCommitResponse()
{
  //get the response
  QStringList response = readfromSocket();

  if( !isPositiveServerMessage( response ) )
  {
    //the server has not accepted the commit
    handleError( i18n( "%1 has not accepted the %2 command. Error message is: %3").arg( getHost() ).arg( COMMIT ).arg( response.first() ) );
    return;
  }

  finishTask();
}

void Account::loginUser()
{
  kdDebug() << "Login without security on " << getName() << endl;

  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotLoginUserResponse() ) );

  //send the command
  sendCommand( LOGIN_USER + " " + getUser() );
}

void Account::slotLoginUserResponse()
{
  //get the response
  QStringList response = readfromSocket();

  if( !isPositiveServerMessage( response ) )
  {
    //the server has not accepted the user
    handleError( i18n( "Login has failed. Error message is: %1").arg( removeStatusIndicator( response ) ) );
    return;
  }

  //send the password
  loginPasswd();
}

void Account::loginPasswd()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotLoginPasswdResponse() ) );

  //send the command
  sendCommand( LOGIN_PASSWD + " " + getPassword() );
}

void Account::slotLoginPasswdResponse()
{
  //get the response
  QStringList response = readfromSocket();

  if( !isPositiveServerMessage( response ) )
  {
    //the server has not accepted the password
    handleError( i18n( "Login has failed. Error message is: %1").arg( removeStatusIndicator( response ) ) );
    return;
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
/*    case AccountRefreshing    : getUIDList(); break;
    case AccountDeleting      : deleteNextMail(); break;
    case AccountDownloading   : showNextMail(); break;*/
    default                   : commit(); break;
  }
}

QString Account::removeStatusIndicator(const QString & message)
{
  QString ret( message );

  if( ret.startsWith( RESPONSE_POSITIVE ) )
  {
    return ret.remove( 0, RESPONSE_POSITIVE.length() );
  }
  else if( ret.startsWith( RESPONSE_NEGATIVE ) )
  {
    return ret.remove( 0, RESPONSE_NEGATIVE.length() );
  }

  return ret;
}




void Account::loginApop()
{
  kdDebug() << "Login with APOP on " << getName() << endl;

  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotLoginApopResponse() ) );

  //calculate MD5 string
  QString secret( apopTimestamp + getPassword() );
  KMD5 md5( secret.toAscii() );
  QString md5Digest( md5.hexDigest() );

  //send the command
  sendCommand( LOGIN_APOP + " " + getUser() + " " + md5Digest );
}

void Account::slotLoginApopResponse()
{
  //get the response
  QStringList response = readfromSocket();

  if( !isPositiveServerMessage( response ) )
  {
    //the login using APOP is failed, if the user has allowed it, we try it with plain login
    if( allowUnsecureLogin )
    {
      //disconnect and try a new login without APOP
      closeConnection();
      dontUseAPOP = true;
      doConnect();
      return;
    }
    else
    {
      handleError( i18n( "Login has failed. Error message is: %1\n\
          Maybe the secure login of this server is faulty. You can try to allow the unsafe login for this account at the account setup.\n\
          Bear in mind in this case criminals could read your password!").arg( removeStatusIndicator( response ) ) );
      return;
    }
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
/*    case AccountRefreshing    : getUIDList(); return;
    case AccountDeleting      : deleteNextMail(); return;
    case AccountDownloading   : showNextMail(); return;*/
    default                   : commit(); return;
  }

}

void Account::removeStatusIndicator( QStringList* response )
{
  //return, if the given list has no elements
  if( response->isEmpty() ) return;

  //get first line
  QString firstLine = response->first();

  //is it an indicator?
  if( firstLine.startsWith( RESPONSE_POSITIVE ) || firstLine.startsWith( RESPONSE_NEGATIVE ) )
  {
    //Yes, it is. Zack und weg!
    response->pop_front();
  }
}

Types::AccountState_Type Account::getState()
{
  return state;
}

bool Account::isUnsecureLoginAllowed() const
{
  return allowUnsecureLogin;
}


