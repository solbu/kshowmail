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
  socket = new KTcpSocket( this );

  //connect the socket with the slots
  connect( socket, SIGNAL( error( KTcpSocket::Error ) ), this, SLOT( slotSocketError( KTcpSocket::Error ) ) );
  connect( socket, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
  connect( socket, SIGNAL( hostFound() ), this, SLOT( slotHostFound() ) );

  

  init();
	
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

  allowUnsecureLogin = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN, DEFAULT_ACCOUNT_ALLOW_UNSECURE_LOGIN );

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

  //create a new mail list
  //When the refresh has finished successfully, this will
  //replace the old mail list
  tempMailList = new MailList( this );



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
  if( socket->state() != KTcpSocket::UnconnectedState )
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
  if( socket->state() != KTcpSocket::UnconnectedState && socket->state() != KTcpSocket::ClosingState )
  {
    kdDebug() << "Close Connection: " << getName() << endl;
    socket->close();
  }
}

void Account::initBeforeConnect()
{
  quitSent = false;
  apopAvail = false;

  //at the next refresh the filter was not applied yet
  filterApplied = false;

  refreshPerformedByFilters = false;  

}

void Account::slotConnected()
{
}

void Account::slotHostFound()
{
}

void Account::slotSocketError( KTcpSocket::Error ErrorCode)
{
  QString message;    //the error message
  switch( ErrorCode )
  {
    case KTcpSocket::UnknownError              : message = i18n( "Unknown error" ); break;
    case KTcpSocket::ConnectionRefusedError    : message = i18n( "Connection refused" ); break;
    case KTcpSocket::HostNotFoundError         : message = QString( i18n( "Host not found: %1" ).arg( getHost() ) ); break;
    case KTcpSocket::RemoteHostClosedError     : message = QString( i18n( "Host %1 closed" ).arg( getHost() ) ); break;
    case KTcpSocket::SocketAccessError         : message = i18n( "Socket access error" ); break;
    case KTcpSocket::SocketResourceError       : message = i18n( "Socket resource error" ); break;
    case KTcpSocket::SocketTimeoutError        : message = i18n( "Socket timeout error" ); break;
    case KTcpSocket::NetworkError              : message = i18n( "Network error" ); break;
    case KTcpSocket::UnsupportedSocketOperationError : message = i18n( "Unsupported Socket Operation Error" ); break;
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

QStringList Account::readfromSocket( QString charset, bool singleLine )
{
  QTextStream socketStream( socket ); //to read from socket
  QStringList serverResponse; //buffer for the server response


  //set charset if known
  if( !charset.isNull() && charset.length() != 0 )
  {
    QTextCodec* codec = QTextCodec::codecForName( charset.toAscii() );
    if( codec == NULL )
    {
      kdDebug() << "No codec found for " << charset << endl;
    }
    else
    {
      socketStream.setCodec( codec );
    }
    
  }

  //read from socket
  bool responseEndFound = false;  //end of a multi-line response found

  //loop until the last line is read
  //the last line is either a single point or a -ERR at first
  while( !responseEndFound )
  {
    //get line
    QString line = socketStream.readLine();

    if( line.isNull() )
    {
      if( !socket->waitForReadyRead() )
      {
        handleError( i18n( "Timeout" ) );
        return QStringList();
      }

      line = socketStream.readLine();
      
    }

    //check for a negative response
    if( isNegativeResponse( line ) )
    {
      serverResponse.append( line );
      responseEndFound = true;
    }
    //check for response end with the point
    else if( line == END_MULTILINE_RESPONSE )
    {
      responseEndFound = true;
    }
    //it is a normal data line
    else
    {
      serverResponse.append( line );
      //break the loop if we anticipate single line
      if( singleLine ) responseEndFound = true;
    }
  }

  return serverResponse;

/*  //buffer for the datas
  char lineBuffer[1024];

  //return object
  QStringList text;

  //wait for a whole line
  while( socket->bytesAvailable() != 0 )
  {
    while( socket->canReadLine() )
    {
      socket->readLine( lineBuffer, sizeof( lineBuffer ) );
      QString readedLine = QString( lineBuffer ).simplified();
      text.append( readedLine );
    }
  }*/
//  return text;
}

void Account::slotReadFirstServerMessage()
{
  QStringList text = readfromSocket( NULL, true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( text.isEmpty() )
  {
    finishTask();
    return;
  }

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
  if( socket->state() != KTcpSocket::ConnectedState )
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
  QStringList text = readfromSocket( NULL, false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( text.isEmpty() )
  {
    finishTask();
    return;
  }

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
  if( text.isEmpty() )
  {
    kdDebug() << "empty server message" << endl;
  }
  else
  {
    for( int i = 0; i < text.size(); ++i )
    {
      kdDebug() << text.at( i ) << endl;
    }
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

  if( !message.isEmpty() )
  {
    //remove termination line
    if( message.last() == END_MULTILINE_RESPONSE )
    {
      message.removeLast();
    }
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
  QStringList text = readfromSocket( NULL, false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( text.isEmpty() )
  {
    finishTask();
    return;
  }

  //have we got capabilities?
  bool haveAuth = isPositiveServerMessage( text );

  //Set some authentification flags
  if( haveAuth )
  {
    //remove status indicator and termination char
    clearMessage( text );
  }

  //the next step is the login
  //if APOP available, we use it
  //otherwise plain login
  //we also don't use APOP if the a previous APOP login failed
  if( apopAvail && !dontUseAPOP )
    loginApop();
  else
  {
    if( allowUnsecureLogin == true )
      loginUser();
    else
    {
      emit sigMessageWindowOpened();
      KMessageBox::sorry( NULL, i18n( "Account %1: This server doesn't provide a safety login and you have disallowed the using of an unsafe login.\n\
                                       If you want to use this Account you must allow unsafe login at the account setup.\
                                       Bear in mind in this case criminals could read your password!" ).arg( getName() ), i18n( "Unsafe login is not allowed") );
      emit sigMessageWindowClosed();

      //the user has not allowed unsafe login; finish the task
      commit();

    }
  }

}

void Account::commit()
{
  if( socket->state() == KTcpSocket::ConnectedState )
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
  QStringList response = readfromSocket( NULL, true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( response.isEmpty() )
  {
    finishTask();
    return;
  }

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
  QStringList response = readfromSocket( NULL, true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( response.isEmpty() )
  {
    finishTask();
    return;
  }

  if( !isPositiveServerMessage( response ) )
  {
    //the server has not accepted the user
    handleError( i18n( "Login has failed. Error message is: %1").arg( removeStatusIndicator( response.first() ) ) );
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
  QStringList response = readfromSocket( NULL, true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( response.isEmpty() )
  {
    finishTask();
    return;
  }

  if( !isPositiveServerMessage( response ) )
  {
    //the server has not accepted the password
    handleError( i18n( "Login has failed. Error message is: %1").arg( removeStatusIndicator( response.first() ) ) );
    return;
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
    case AccountRefreshing    : getUIDList(); break;
/*    case AccountDeleting      : deleteNextMail(); break;
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
  QStringList response = readfromSocket( NULL, true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( response.isEmpty() )
  {
    finishTask();
    return;
  }

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
          Bear in mind in this case criminals could read your password!").arg( removeStatusIndicator( response.first() ) ) );
      return;
    }
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
    case AccountRefreshing    : getUIDList(); return;
/*    case AccountDeleting      : deleteNextMail(); return;
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

void Account::getUIDList()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotUIDListResponse() ) );

  //send the command
  sendCommand( UID_LIST );
}

void Account::slotUIDListResponse()
{
  //get the response
  QStringList receivedUIDs = readfromSocket( NULL, false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( receivedUIDs.isEmpty() )
  {
    finishTask();
    return;
  }

  //no response from the server
  if( receivedUIDs.isEmpty() )
  {
    handleError( i18n( "%1 has not sent a response after %2 command.").arg( getHost() ).arg( UID_LIST ) );
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( receivedUIDs ) )
  {
    handleError( i18n( "%1 doesn't support mail UID's. KShowmail can't work without this. Error message is: %2" ).arg( getName() ).arg( receivedUIDs.first() ) );
    return;
  }

  //remove first and last line of the response
  //this are the state and the end of response marker
  //we don't need it
  removeStatusIndicator( &receivedUIDs );
  removeEndOfResponseMarker( &receivedUIDs );

  //have we get any UIDs?
  if( receivedUIDs.isEmpty() )
  {
    //we haven't received any UIDs. The account has no mails.
    //finalize the refresh
    swapMailLists();
    return;
  }


  long number;                 //an extracted mail number
  QString uid;                //an extracted uid
  bool isNew = false;         //state of the received mail

  //analyze UIDs
  //iterate over all UIDs in the list
  for ( QStringList::Iterator it = receivedUIDs.begin(); it != receivedUIDs.end(); ++it )
  {
    QString line = *it;

    //every line has the format "number UID", e.g.: 1 bf10d38018de7c1d628d65288d722f6a
    //get the position of the separating space
    int positionOfSpace = line.indexOf( " " );

    //if no space was found, the line is corrupt
    if( positionOfSpace == -1 )
    {
      handleError( i18n( "Get corrupt UID list. No spaces" ) );
      return;
    }
    else
    {
      //extract mail number and uid
      bool isNumber;
      number = line.left( positionOfSpace ).toLong( &isNumber );
      
      //check number
      if( !isNumber )
      {
        //the first part is not a number
        handleError( i18n( "Get corrupt UID list. No number found at begin." ) );
        return;
      }
      else
      {
        //number is ok; extract uid
        uid = line.mid( positionOfSpace + 1 );

        //determine about new mail or not
        if( !mails->hasMail( uid ) )
        {
          //the old list doesn't contain a mail with this uid
          //the mail is new
          isNew = true;
        }
        else if( ( accountList->keepNew() || refreshPerformedByFilters ) && mails->isNew( uid ) )
        {
          //the mail is already in the old list
          //but we will leave the state of formerly new mails, because the user wants it or this refresh is performed by filters
          isNew = true;
        }
        else
          isNew = false;

        //add mail to the list
        tempMailList->addMail( number, uid, isNew );

      }
    }
  }

  //the next step is to get the mail sizes
  getMailSizes();
}

void Account::removeEndOfResponseMarker( QStringList * response )
{
  //return, if the given list has no elements
  if( response->isEmpty() ) return;

  //get last line
  QString lastLine = response->last();

  //is it a marker?
  if( lastLine == END_MULTILINE_RESPONSE  )
  {
    //Yes, it is. Zack und weg!
    response->pop_back();
  }

}

void Account::swapMailLists( )
{
  //delete old mail list
  delete mails;

  //assign the new list
  if( tempMailList != NULL )
    mails = tempMailList;
  else
    mails = new MailList( this );


  //if the filters were not applied yet, we do it now
  //applyFilters() will either start a second refresh cycle if it did some deletions
  //or call commit() to commit the refresh cycle.
  //if the filters were already applied we commit the refresh.
//   if( filterApplied | !headerFilter.isActive() )
//   {
//     //reset the flag for the next refresh
//     filterApplied = false;
// 
//     //commit the refresh cycle
//     commit();
//     return;
//   }
//   else
//   {
//     //apply the filters
//     applyFilters();
//     return;
//   }

  commit();
}

void Account::applyFilters()
{
/*  //are we executed by the MOVE routines?
  if( !downloadActionsInvoked )
  {
    //this is the first call (at the current refresh cycle) of this methode
    //we get the lists of mails to delete an move and call the MOVE routines if necessary

    //OK, the filters were applied
    filterApplied = true;

    //order the mail list to apply the header filters
    //it returns lists of mail numbers which shall be deleted or moved
    //the marking will be done by the mail list itself
    //the mail list removes all mails which shall be ignored itself
    mailsToDelete.clear();
    mailList->applyHeaderFilter( &headerFilter, getAccountName(), mailsToDelete, mailsToDownload, nmbIgnoredMails, FLog );
    nmbDeletedMailsLastRefresh += mailsToDelete.count();
    nmbDeletedMailsLastStart += mailsToDelete.count();

    //This part will be executed, if mails shall be downloaded
    if( !mailsToDownload.empty() )
    {
      downloadActionsInvoked = true;
      doDownloadActions();

      //we quit this methode at this point, because after the bodies are downloaded and written this methode will recalled.
      //At this time the else branch of this IF-statement will be executed and the methode continues
      return;
    }

  }
  else
  {
    //this is the second call (at the current refresh cycle) of this methode.
    //it is called by the Move routines.
    //the downloading of the mailbodies and writing it to the mailboxes has ended.
    //A second call was just exceuted, if there was mails to move
    downloadActionsInvoked = false;

    //after an move error there are maybe some mails leftover in MailsToMove
    mailsToDownload.clear();
  }



  //we have get the list of mails to delete and the all mails to move are written to its mailboxes
  //now we delete this mails (the moved mails too)

  if( !mailsToDelete.empty() )
  {
    //there are mails to delete
    //we delete they
    //after the delete cycle has done its job, it will call applyFiltersDeleted()
    deletionPerformedByFilters = true;  //this is set to indicate the deletion is performed by filters and not by user
                                        //the deletion methodes need it to decide on branch targets
    deleteNextMail();
  }
  else
  {
    //if we need not to start a second refresh cycle (no mails was deleted or moved)
    //we just commit the refresh and let the filter applied flag to false for the next regular refresh
    commit();
    filterApplied = false;
  }*/
}

void Account::getMailSizes()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotMailSizesResponse() ) );

  //send the command
  sendCommand( MAIL_LIST );
}

void Account::slotMailSizesResponse()
{

  //get the response
  QStringList receivedSizes = readfromSocket( NULL, false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( receivedSizes.isEmpty() )
  {
    finishTask();
    return;
  }

  //no response from the server
  if( receivedSizes.isEmpty() )
  {
    handleError( i18n( "%1 has not sent a response after %2 command.").arg( getHost() ).arg( MAIL_LIST ) );
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( receivedSizes ) )
  {
    handleError( i18n(  "Error while try to get the mail sizes. Message is: %1" ).arg( receivedSizes.first() ) );
    return;
  }

  //remove first and last line of the response
  //this are the state and the end of response marker
  //we don't need it
  removeStatusIndicator( &receivedSizes );
  removeEndOfResponseMarker( &receivedSizes );


  int number;                 //an extracted mail number
  long size;                   //an extracted size

  //analyze Sizes
  if( receivedSizes.isEmpty() )
  {
    handleError( i18n(  "Error while try to get the mail sizes. All mails are disappeard." ) );
    return;
  }

  //iterate over all sizes in the list
  for ( QStringList::Iterator it = receivedSizes.begin(); it != receivedSizes.end(); ++it )
  {
    QString line = *it;

    //every line has the format "number size", e.g.: 1 1234
    //get the position of the separating space
    int positionOfSpace = line.indexOf( " " );

    //if no space was found, the line is corrupt
    if( positionOfSpace == -1 )
    {
      handleError( i18n( "Get corrupt size list. No spaces" ) );
      return;
    }
    else
    {
      //extract mail number and size
      bool isNumber;
      number = line.left( positionOfSpace ).toInt( &isNumber );
      //check number
      if( !isNumber )
      {
        //the first part is not a number
        handleError( i18n( "Get corrupt UID list. No numbers at begin." ) );
        return;
      }
      else
      {
        //number is ok; extract size
        size = line.mid( positionOfSpace + 1 ).toLong( &isNumber );

        //check size
        if( !isNumber )
        {
          //the second part of the string is not a number
          handleError( i18n( "Get corrupt UID list. No sizes found at end." ) );
          return;
        }
        else
        {
          //size is ok
          //set it
          tempMailList->setSize( number, size );
        }
      }
    }
  }

  //now we get the headers
  getHeaders();
}

void Account::getHeaders( )
{
  //get the numbers of all new mails
  newMails = tempMailList->getNewMails();
  if( newMails.empty() )
  {
    //no new mails available; copy the known headers from the old mail list
    copyHeaders();
    return;
  }

  //get the headers
  getNextHeader();
}

void Account::getNextHeader( )
{
  //if the list of mails empty, copy the known headers from the old mail list
  if( newMails.empty() )
  {
    copyHeaders();
    return;
  }

  //we get the header of mail number ...
  QString mailNumber;
  mailNumber =  mailNumber.setNum( *newMails.begin() );

  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotGetHeaderResponse() ) );

  //send the command
  sendCommand( GET_HEADER + " " + mailNumber + " 0" );
}

void Account::slotGetHeaderResponse( )
{
  //we get the header of mail number
  int mailNumber = *newMails.begin();

  //get the response
  QStringList header = readfromSocket( NULL, false );

  //no response from the server
  if( header.isEmpty() )
  {
    handleError( i18n( "%1 has not sent the header of mail %2.").arg( getHost() ).arg( mailNumber ) );
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( header ) )
  {
    handleError( i18n( "%1 doesn't support the TOP command. KShowmail can't work without this. Error message is: %2" ).arg( getName() ).arg( header.first() ) );
    return;
  }

  //remove first and last line of the response
  //this are the state and the end of response marker
  //we don't need it
  removeStatusIndicator( &header );
  removeEndOfResponseMarker( &header );

  //store header
  tempMailList->setHeader( *newMails.begin(), header );

  //remove the first item of the list of new mails
  newMails.removeFirst();

  //if the list of new mails is empty, copy the headers of old mails to the new list
  if( newMails.empty() )
  {
    copyHeaders();
    return;
  }

  //get next header
  getNextHeader();
}

bool Account::isNegativeResponse(const QString & response)
{
  return response.startsWith( RESPONSE_NEGATIVE );

}

void Account::copyHeaders( )
{
  //get the UIDs of the old mails in the temporary mail list
  QStringList UIDs = tempMailList->getUIDsOfOldMails();

  try
  {
    //iterate over all members of the list,
    //get the header from the old list and store it in the new one
    QStringList::iterator it;
    for ( it = UIDs.begin(); it != UIDs.end(); ++it )
    {
      QStringList header = mails->getHeaderOf( *it );
      tempMailList->setHeader( *it, header );
    }
  }
  catch( CorruptDataException& e )
  {
    kdDebug() << "Fehler: " << e.what() << endl;
  }
  
  //now we have the a complete new mail list
  swapMailLists();
}

int Account::getNumberMails() const
{
  if( isActive() ) return mails->getNumberMails();

  return 0;
}

Mail* Account::getMail( int number ) const throw ( CorruptDataException )
{
  return mails->getMail( number );
}