/*
Copyright 2010  Ulrich Weigelt <ulrich.weigelt@gmx.de>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "account.h"
#include "maillist.h"

Account::Account( QString name, AccountList* accountList, QObject* parent )
	: QObject( parent )
{
	this->name = name;
  this->accountList = accountList;

  //create mail container
  mails = new MailList( this, this );


  //create timeout timer
  timeoutTimer = new QTimer( this );
  connect( timeoutTimer, SIGNAL( timeout() ), this, SLOT( slotTimeout() ) );

  init();

}

Account::~Account()
{
	delete mails;
  if( !socket.isNull() ) delete socket;
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

  //at start, we are idle
  state = AccountIdle;

  //initialize counters
  nmbDeletedMailsLastRefresh = 0;
  nmbMovedMailsLastRefresh = 0;
  nmbIgnoredMails = 0;
  moveCounter = 0;
  nmbDeletedMailsLastStart = 0;
  nmbMovedMailsLastStart = 0;
  nmbIgnoredMails = 0;

  downloadActionsInvoked = false;
  deletionPerformedByFilters = false;
  filterApplied = false;

  //handle the errors
  dontHandleError = false;

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
    setPassword( QString() );

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

	  //read Spam configs
	KConfigGroup* spamConfig = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_SPAMCHECK );
  int intSpamAction = spamConfig->readEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, DEFAULT_SPAMCHECK_ACTION );

  switch( intSpamAction )
  {
    case CONFIG_VALUE_SPAMCHECK_ACTION_DELETE       : spamAction = FActDelete; break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MARK         : spamAction = FActMark; break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MOVE         : spamAction = FActMove; break;
    default                                         :
      kdError() << "Invalid value in " << CONFIG_ENTRY_SPAMCHECK_ACTION << ". Set default value." << endl;
      switch( DEFAULT_SPAMCHECK_ACTION )
      {
        case CONFIG_VALUE_SPAMCHECK_ACTION_DELETE       : spamAction = FActDelete; break;
        case CONFIG_VALUE_SPAMCHECK_ACTION_MARK         : spamAction = FActMark; break;
        case CONFIG_VALUE_SPAMCHECK_ACTION_MOVE         : spamAction = FActMove; break;
        default                                         : spamAction = FActMark; break;
      }

  }

  if( spamAction == FActMove )
    spamMailbox = spamConfig->readEntry( CONFIG_ENTRY_SPAMCHECK_MOVE_MAILBOX, DEFAULT_SPAMCHECK_ACTION_MOVE_MAILBOX );

  //read general configs
  KConfigGroup* generalConfig = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_GENERAL );
  informAboutErrors = generalConfig->readEntry( CONFIG_ENTRY_SHOW_CONNECTION_ERRORS, DEFAULT_SHOW_CONNECTION_ERRORS );
  timeOutTime = generalConfig->readEntry( CONFIG_ENTRY_TIMEOUT_TIME, DEFAULT_TIMEOUT_TIME );

  delete accountConfig;
	delete spamConfig;
  delete generalConfig;
}

void Account::refreshMailList( FilterLog* log )
{
  //store pointer to log
  if( log != NULL )
    fLog = log;

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
  tempMailList = new MailList( this, this );

  //init counter
  if( !refreshPerformedByFilters )
  {
    nmbDeletedMailsLastRefresh = 0;
    nmbMovedMailsLastRefresh = 0;
    nmbIgnoredMails = 0;
  }



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
    QPointer<KPasswordDialog> pwdialog = new KPasswordDialog( NULL );
    pwdialog->setPrompt( i18nc( "@info we need the password", "Please type in the password for <resource>%1</resource>", getName() ) );
    int result = pwdialog->exec();


    //set waiting cursor
    QApplication::setOverrideCursor( Qt::WaitCursor );

    //let's look, what the user has done :o)
    if( result == KPasswordDialog::Accepted )
    {
      //the user has clicked OK in the password dialog
      //store the password
      setPassword( pwdialog->password() );

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
      delete pwdialog;

      //emit configuration changed signal
      emit ( sigConfigChanged() );

      //tell we have a password
      return true;
    }
    else {
      //the user has clicked Cancel in the password dialog; we don't have a password
      delete pwdialog;
      return false;
    }
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
 //if there is already a connection, close it
 if( !socket.isNull() ) {

  if( socket->state() != KTcpSocket::UnconnectedState )
  {
    closeConnection();
  }

 }


  initBeforeConnect();

  //the first task is to read the server greeting message
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotReadFirstServerMessage() ) );

  //do connect
  if( transferSecurity == TransSecNone || transferSecurity == TransSecTLS ) {

    socket->connectToHost( getHost(), getPort() );

  } else if( transferSecurity == TransSecSSL ) {

    socket->connectToHostEncrypted( getHost(), getPort() );


  } else {

    handleError( "Unsupported Transfer Security" );
    return;
  }
}

void Account::closeConnection()
{
  if( socket->state() != KTcpSocket::UnconnectedState && socket->state() != KTcpSocket::ClosingState )
  {
    kdDebug() << "Close Connection: " << getName() << endl;

    socket->flush();
    socket->disconnectFromHost();
    if( socket->state() != KTcpSocket::UnconnectedState )
      socket->waitForDisconnected();
    socket->reset();

    //we try to close up to 5 times
    int nrTry = 0;
    while( nrTry < 5 && socket->state() != KTcpSocket::UnconnectedState ) {
      socket->close();
      nrTry++;
    }
  }
}

void Account::initBeforeConnect()
{

  //we instance a new socket because sometimes the old doesn't disconnect correctly
//   if( !socket.isNull() ) {
//
//     disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
//     disconnect( socket, SIGNAL( error( KTcpSocket::Error ) ), 0, 0 );
//     disconnect( socket, SIGNAL( connected() ), 0, 0 );
//     disconnect( socket, SIGNAL( hostFound() ), 0, 0 );
//     disconnect( socket, SIGNAL( sslErrors(QList<KSslError>)), 0, 0 );
//
//     delete socket;
//   }


  //create TCP-Socket
  if( socket.isNull() )
    socket = new KTcpSocket( this );

  //connect the socket with the slots
  connect( socket, SIGNAL( error( KTcpSocket::Error ) ), this, SLOT( slotSocketError( KTcpSocket::Error ) ) );
  connect( socket, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
  connect( socket, SIGNAL( hostFound() ), this, SLOT( slotHostFound() ) );
  connect( socket, SIGNAL( sslErrors(QList<KSslError>) ), this, SLOT( slotSSLError(QList<KSslError>) ) );

  //init some flags
  quitSent = false;
  apopAvail = false;
  dontUseAPOP = false;

  downloadActionsInvoked = false;
  deletionPerformedByFilters = false;
  filterApplied = false;

  dontHandleError = false;

  //init capability flags
  supportsStartTLS = false;

  //start timeout timer
  timeoutTimer->start( timeOutTime * 1000 );

}

void Account::slotConnected()
{
}

void Account::slotHostFound()
{
}

void Account::slotSocketError( KTcpSocket::Error errorCode)
{
  //maybe we must not handle this error
  if( dontHandleError ) return;


  QString message;    //the error message
  switch( errorCode )
  {
    case KTcpSocket::UnknownError              : message = i18nc( "@info error message", "Unknown error" ); break;
    case KTcpSocket::ConnectionRefusedError    : message = i18nc( "@info error message", "Connection refused" ); break;
    case KTcpSocket::HostNotFoundError         : message = QString( i18nc( "@info error message", "Host not found: <resource>%1</resource>", getHost() ) ); break;
    case KTcpSocket::RemoteHostClosedError     : message = QString( i18nc( "@info error message", "Host <resource>%1</resource> closed", getHost() ) ); break;
    case KTcpSocket::SocketAccessError         : message = i18nc( "@info error message", "Socket access error" ); break;
    case KTcpSocket::SocketResourceError       : message = i18nc( "@info error message", "Socket resource error" ); break;
    case KTcpSocket::SocketTimeoutError        : message = i18nc( "@info error message", "Socket timeout error" ); break;
    case KTcpSocket::NetworkError              : message = i18nc( "@info error message", "Network error" ); break;
    case KTcpSocket::UnsupportedSocketOperationError : message = i18nc( "@info error message", "Unsupported Socket Operation Error" ); break;
    default                                    : message = i18nc( "@info error message", "Unknown connection error" ); break;
  }

//   switch( ErrorCode ) {
//
//     case QAbstractSocket::ConnectionRefusedError    : message = i18n( "The connection was refused by the peer or timed out" ); break;
//     case QAbstractSocket::RemoteHostClosedError     : message = i18n( "The remote host closed the connection" ); break;
//     case QAbstractSocket::HostNotFoundError         : message = QString( i18n( "Host not found: %1" ).arg( getHost() ) ); break;
//     case QAbstractSocket::SocketAccessError         : message = i18n( "Socket access error" ); break;
//     case QAbstractSocket::SocketResourceError       : message = i18n( "Socket resource error" ); break;
//     case QAbstractSocket::SocketTimeoutError        : message = i18n( "Socket timeout error" ); break;
//     case QAbstractSocket::DatagramTooLargeError     : message = i18n( "The datagram was larger than the operation system's limit" ); break;
//     case QAbstractSocket::NetworkError              : message = i18n( "Network error" ); break;
//     case QAbstractSocket::AddressInUseError         : message = i18n( "The address is already in use." ); break;
//     case QAbstractSocket::SocketAddressNotAvailableError : message = i18n( "The address does not belong to the host" ); break;
//     case QAbstractSocket::UnsupportedSocketOperationError : message = i18n( "Unsupported Socket Operation Error" ); break;
//     case QAbstractSocket::ProxyAuthenticationRequiredError : message = i18n( "The socket is using a proxy, and the proxy requires authentication." ); break;
//     case QAbstractSocket::SslHandshakeFailedError   : message = i18n( "The SSL/TLS handshake failed, so the connection was closed." ); break;
//     case QAbstractSocket::UnfinishedSocketOperationError : message = i18n( "The last operation attempted has not finished yet." ); break;
//     case QAbstractSocket::ProxyConnectionRefusedError : message = i18n( "Could not contact the proxy server because the connection to that server was denied." ); break;
//     case QAbstractSocket::ProxyConnectionClosedError  : message = i18n( "The connection to the proxy server was closed unexpectedly." ); break;
//     case QAbstractSocket::ProxyConnectionTimeoutError : message = i18n( "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase." ); break;
//     case QAbstractSocket::ProxyNotFoundError        : message = i18n( "The proxy address was not found." ); break;
//     case QAbstractSocket::ProxyProtocolError        : message = i18n( "The connection negotiation with the proxy server because the response from the proxy server could not be understand." ); break;
//     case QAbstractSocket::UnknownSocketError        : message = i18n( "Unknown socket error" ); break;
//     default                                         : message = i18n( "Unknown connection error" ); break;
//
//
//   }

  //show error and handle all other
  handleError( message );

}

void Account::handleError( QString error )
{

  //close connection
  closeConnection();

  //stop timeout timer
  timeoutTimer->stop();

  //show error
  if( informAboutErrors ) {

    emit sigMessageWindowOpened();
    KMessageBox::error( NULL, i18nc( "@info general error message", "Account <resource>%1</resource>: %2", getName(), error ) );
    emit sigMessageWindowClosed();
  }

  //set state but save old state to emit the right ready signal
  AccountState_Type oldState = state;
  state = AccountIdle;

  //clear lists
  mailsToDelete.clear();
  mailsToDownload.clear();
  mailsToShow.clear();

  //emit ready signal
  switch( oldState )
  {
    case AccountDeleting    : emit sigDeleteReady( getName() ); break;
    case AccountDownloading : emit sigShowBodiesReady( getName() ); break;
    case AccountRefreshing  : emit sigRefreshReady( getName() ); break;
    default                 : break;
  }



}

QStringList Account::readfromSocket( bool singleLine )
{
  QString readed;
  bool responseEndFound = false;  //end of a multi-line response found

  //all lines of the response are terminated by a CRLF pair.
  QString lineTerm( 13 );
  lineTerm.append( 10 );


  //this string is at the end of a multiline response
  QString endOfMultiLine( lineTerm );
  endOfMultiLine.append( END_MULTILINE_RESPONSE );
  endOfMultiLine.append( lineTerm );


  //read all bytes until we have get the line end of a single line response
	//or the end line of a multiline response
  while( !responseEndFound ) {

		//wait for new bytes
    if( socket->bytesAvailable() == 0 ) {

      //this is a workaround for SSL
      //sometimes there isn't the whole response available at once
      //In this case a waitForReadyRead() is hanging until timeout.
      //Therefore we send a NOOP to the server. This released the missing bytes
      //from the sockt. But the problem is, the response of the NOOP is now in front
      //of the response caused by the next command. Therefore removeStatusIndicator()
      //must remove all status lines.
      if( transferSecurity == TransSecSSL || transferSecurity == TransSecTLS ) {

        dontHandleError = true;
        if( !socket->waitForReadyRead( 500 ) ) {

          QByteArray toWrite( "NOOP\n" );
          socket->write( toWrite );

          dontHandleError = false;
          if( !socket->waitForReadyRead() ) {

            return QStringList();
          }

        }
        dontHandleError = false;

      } else {

        if( !socket->waitForReadyRead() )
        {
          return QStringList();
        }
      }



    }

    //read
    QByteArray data = socket->readAll();

    //append the read data
    if( data.size() > 0 ) {
      readed.append( data );
    }

    //check for end of response
    //-------------------------
    //is negative response?
    if( isNegativeResponse( readed ) )
    {
      responseEndFound = true;
    }
    //check for end of multiline response
    else if( readed.endsWith( endOfMultiLine ) )
    {
      responseEndFound = true;
    }
    //it is a normal data line
    else
    {
      //break the loop if we anticipate single line
      if( singleLine && readed.endsWith( lineTerm ) ) responseEndFound = true;
    }


  }


  //split the response into lines
  QStringList response = readed.split( lineTerm );

  //the last line could be an empty string, because there is a CRLF at the end of
  //the received byte stream and therefore QString.split() creates this.
  //We can't use QString::SkipEmptyParts in split() because we want to keep the empty lines
  //into the mail
  if( response.last().isEmpty() ) {

    response.removeLast();
  }


  return response;


}

void Account::slotReadFirstServerMessage()
{
  QStringList text = readfromSocket( true );

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
    handleError( i18nc( "@info error message: this is not a POP3 server", "<resource>%1</resource> is not a POP3 mail server", getHost() ) );
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
    handleError( i18nc( "@info error message: connection lost", "No connect to <resource>%1</resource>", getHost() ) );
    return;
  }

  //the write methode of the socket needs a byte array
  QByteArray data;
  data.append( command );
  data.append( "\n" );

  //send it
  //qint64 writtenBytes = socket->write( data );
  qint64 writtenBytes = socket->write( data.data() );

  //if the return value ist -1 a error is occurred
  if( writtenBytes == -1 )
  {
    handleError( i18nc( "@info error message: could not send a command to the server", "Could not send the command <icode>%1</icode> to <resource>%2</resource>", command, getName() ) );
    return;
  }

}

void Account::getCapabilities()
{
  //when the server answer is coming in, the slot slotReceiveCapabilities will be invoked
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotCapabilitiesResponse() ) );

  //send the CAPA-Command
  sendCommand( CAPA_REQUEST );
}

void Account::slotCapabilitiesResponse()
{

  //get server answer
  QStringList text = readfromSocket( false );


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
    removeStatusIndicator( &text );
    removeEndOfResponseMarker( &text );

    //has STARTTLS?
    supportsStartTLS = text.contains( CAPA_RESPONSE_STLS, Qt::CaseInsensitive );

		//if TLS is selected but this provide doesn't support it,
		//we will finish this task here
		if( transferSecurity == TransSecTLS && !supportsStartTLS ) {

			handleError( i18nc( "@info error message", "No support for START-TLS" ) );
			return;
		}

  }


  //get authentication mechanism
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
  QStringList text = readfromSocket( false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( text.isEmpty() )
  {
    finishTask();
    return;
  }

  //have we got capabilities?
  bool haveAuth = isPositiveServerMessage( text );

  //Set some authentication flags
  if( haveAuth )
  {
    //remove status indicator and termination char
    removeStatusIndicator( &text );
    removeEndOfResponseMarker( &text );
  }

  //if the STARTTLS is supported, we start this now
  //otherwise the next step is the login
  //if APOP available, we use it
  //otherwise plain login
  //we also don't use APOP if the a previous APOP login failed
  if( supportsStartTLS && transferSecurity == TransSecTLS ) {

    startTLS();
    return;

  } else if( transferSecurity == TransSecSSL ) {

    loginUser();
    return;

  } else if( apopAvail && !dontUseAPOP )
    loginApop();
  else
  {
    if( allowUnsecureLogin == true )
      loginUser();
    else
    {
      emit sigMessageWindowOpened();
      KMessageBox::sorry( NULL, i18nc( "@info Warning: the server does'nt support secure login", "Account <resource>%1</resource>: This server doesn't provide a safety login and you have disallowed the using of an unsafe login. If you want to use this Account you must allow unsafe login at the account setup.<nl/><warning>Bear in mind in this case criminals could read your password!</warning>", getName() ), i18nc( "@title:window", "Unsafe login is not allowed") );
      emit sigMessageWindowClosed();

      //the user has not allowed unsafe login; finish the task
      finishTask();

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

  //clear lists
  mailsToDelete.clear();
  mailsToDownload.clear();
  mailsToShow.clear();

}

void Account::slotCommitResponse()
{
  //get the response
  QStringList response = readfromSocket( true );

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
    handleError( i18nc( "@info error message: we could not quit the POP3 session", "<resource>%1</resource> has not accepted the <icode>%2</icode> command. Error message is: <message>%3</message>", getHost(), COMMIT, response.first() ) );
    return;
  }

  finishTask();
}

void Account::loginUser()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotLoginUserResponse() ) );

  //send the command
  sendCommand( LOGIN_USER + ' ' + getUser() );
}

void Account::slotLoginUserResponse()
{
  //get the response
  QStringList response = readfromSocket( true );

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
    handleError( i18nc( "@info error message", "Login has failed. Maybe your user name is invalid. Error message is: <message>%1</message>", removeStatusIndicator( response.first() ) ) );
    //reload account settings to clear the invalid password
    load();

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
  sendCommand( LOGIN_PASSWD + ' ' + getPassword() );
}

void Account::slotLoginPasswdResponse()
{
  //get the response
  QStringList response = readfromSocket( true );

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
    handleError( i18nc( "@info error message", "Login has failed. Maybe your user name or password is invalid. Error message is: <message>%1</message>", removeStatusIndicator( response.first() ) ) );
    //reload account settings to clear the invalid password
    load();
    return;
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
    case AccountRefreshing    : getUIDList(); break;
    case AccountDeleting      : deleteNextMail(); break;
    case AccountDownloading   : showNextMail(); break;
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
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotLoginApopResponse() ) );

  //calculate MD5 string
  QString secret( apopTimestamp + getPassword() );
  KMD5 md5( secret.toAscii() );
  QString md5Digest( md5.hexDigest() );

  //send the command
  sendCommand( LOGIN_APOP + ' ' + getUser() + ' ' + md5Digest );
}

void Account::slotLoginApopResponse()
{
  //get the response
  QStringList response = readfromSocket( true );

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
      handleError( i18nc( "@info error message", "Login has failed. Maybe your user name or password is invalid. Error message is: <message>%1</message><nl/>Maybe the secure login of this server is faulty. You can try to allow the unsafe login for this account at the account setup.<nl/><warning>Bear in mind in this case criminals could read your password!</warning>", removeStatusIndicator( response.first() ) ) );
      //reload account settings to clear the invalid password
      load();
      return;
    }
  }

  //now we have to decide what we want to do at next
  switch( getState() )
  {
    case AccountRefreshing    : getUIDList(); return;
    case AccountDeleting      : deleteNextMail(); return;
    case AccountDownloading   : showNextMail(); return;
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
  //see the info about the SSL workaround in readFromSocket()
  while( firstLine.startsWith( RESPONSE_POSITIVE ) || firstLine.startsWith( RESPONSE_NEGATIVE ) )
  {
    //Yes, it is. Zack und weg!
    response->pop_front();

    firstLine = response->first();
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
  kdDebug() << "slotUIDListRespones" << endl;

  //get the response
  QStringList receivedUIDs = readfromSocket( false );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( receivedUIDs.isEmpty() )
  {
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( receivedUIDs ) )
  {
    handleError( i18nc( "@info error message: a server doesn't support mail UIDs", "<resource>%1</resource> doesn't support mail UID's. KShowmail can't work without this. Error message is: <message>%2</message>", getName(), receivedUIDs.first() ) );
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
      handleError( i18nc( "@info error message", "Get corrupt UID list. No spaces" ) );
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
        handleError( i18nc( "@info error message", "Get corrupt UID list. No number found at begin." ) );
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
    mails = new MailList( this, this );

  refreshPerformedByFilters = false;

  //if the filters were not applied yet, we do it now
  //applyFilters() will either start a second refresh cycle if it did some deletions
  //or call commit() to commit the refresh cycle.
  //if the filters were already applied we commit the refresh.
  if( filterApplied | !headerFilter.isActive() )
  {
    //reset the flags for the next refresh
    filterApplied = false;

    //commit the refresh cycle
    commit();
    return;
  }
  else
  {
    //apply the filters
    applyFilters();
    return;
  }

  commit();
}

void Account::applyFilters()
{
  //are we executed by the MOVE routines?
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
    mails->applyHeaderFilter( &headerFilter, getName(), mailsToDelete, mailsToDownload, nmbIgnoredMails, fLog );
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
  }
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
  QStringList receivedSizes = readfromSocket( false );

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
    handleError( i18nc( "@info error message: a server has not send a response", "<resource>%1</resource> has not sent a response after <icode>%2</icode> command.", getHost(), MAIL_LIST ) );
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( receivedSizes ) )
  {
    handleError( i18nc( "@info error message: a server has not send the mail sizes", "Error while try to get the mail sizes. Message is: <message>%1</message>", receivedSizes.first() ) );
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
    handleError( i18nc( "@info error message", "Error while try to get the mail sizes. All mails are disappeard." ) );
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
      handleError( i18nc( "@info error message", "Get corrupt size list. No spaces" ) );
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
        handleError( i18nc( "@info error message", "Get corrupt size list. No numbers at begin." ) );
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
          handleError( i18nc( "@info error message", "Get corrupt size list. No sizes found at end." ) );
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
  sendCommand( GET_HEADER + ' ' + mailNumber + " 0" );
}

void Account::slotGetHeaderResponse( )
{
  //we get the header of mail number
  int mailNumber = *newMails.begin();

  //get the response
  QStringList header = readfromSocket( false );

  //no response from the server
  if( header.isEmpty() )
  {
    handleError( i18nc( "@info error message: could not get a mail header", "<resource>%1</resource> has not sent the header of mail <numid>%2</numid>.", getHost(), mailNumber ) );
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( header ) )
  {
    handleError( i18nc( "@info error message: a server doesn't know the command TOP", "<resource>%1</resource> doesn't support the TOP command. KShowmail can't work without this. Error message is: <message>%2</message>", getName(), header.first() ) );
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

void Account::addMailToDelete( int number )
{
  mailsToDelete.append( number );
}

void Account::addMailToShow( int number )
{
  mailsToShow.append( number );
}

void Account::deleteMails()
{
  //return if no mails in the list
  if( mailsToDelete.empty() )
  {
    emit sigDeleteReady( name );
    return;
  }

  //check whether we have a password for this account
  //if not, ask for it
  //return when no password is available
  if( !assertPassword() )
  {
    emit sigDeleteReady( name );
    return;
  }

  //set account state
  state = AccountDeleting;

  //connect
  doConnect();

}

void Account::deleteNextMail( )
{
  //if the list of mails to delete is empty, finalize the deletion and return
  if( mailsToDelete.empty() )
  {
    if( deletionPerformedByFilters )
    {
      applyFiltersDeleted();
    }
    else
    {
      commit();
    }
    return;
  }

  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotMailDeleted() ) );

  //send deletion command
  sendCommand( DELETE + ' ' + QString( "%1" ).arg( mailsToDelete.first() ) );
}

void Account::showNextMail()
{
  //if the list of mails to show is empty finalize it
  if( mailsToShow.isEmpty() )
  {
    commit();
    return;
  }

  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotBodyDownloaded() ) );

  //send download command
  sendCommand( GET_MAIL + ' ' + QString( "%1" ).arg( mailsToShow.first() ) );

}

void Account::slotMailDeleted()
{
  //get the response
  QStringList answer = readfromSocket( true );

  //no response from the server
  if( answer.isEmpty() )
  {
    handleError( i18nc( "@info error message: no server responce after we have deleted a mail", "<resource>%1</resource> has not sent a answer after removing of mail <numid>%2</numid>.", getHost(), mailsToDelete.first() ) );
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( answer ) )
  {
    handleError( i18nc( "@info error message: error while removing a mail", "Error while removing mail <numid>%1</numid> of <resource>%2</resource>: <message>%3</message>", mailsToDelete.first(), getName(), answer.first() ) );
    return;
  }

  //if the mail was manual deleted we write it into the log
  //the log entries about mails deleted by the filter will be written in Mail::applyHeaderFilter()
  if( !deletionPerformedByFilters ) {

    fLog->addDeletedMail( mails->getDateTimeOf( mailsToDelete.first() ),
                          mails->getSenderOf( mailsToDelete.first() ),
                          getName(),
                          mails->getSubjectOf( mailsToDelete.first() ),
                          DelManual
                        );
  }

  //remove the first item of the list of mails to delete
  mailsToDelete.removeFirst();

  //if the list of mails to delete is empty, finalize the deletion and return
  if( mailsToDelete.empty() )
  {
    if( deletionPerformedByFilters )
    {
      applyFiltersDeleted();
    }
    else
    {
      commit();
    }
    return;
  }

  //delete next mail in list
  deleteNextMail();

}

void Account::slotBodyDownloaded()
{
  //get the response
  QStringList answer = readfromSocket( false );

  //no response from the server
  if( answer.isEmpty() )
  {
    handleError( i18nc( "@info error message: we could not download a mail", "<resource>%1</resource> has not sent an answer after retrieve of mail <numid>%2</numid>.", getHost(), mailsToShow.first() ) );
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( answer ) )
  {
    handleError( i18nc( "@info error message: we could not download a mail", "Error while downloading mail <numid>%1</numid> of <resource>%2</resource>: <message>%3</message>", mailsToShow.first(), getName(), answer.first() ) );
    return;
  }

  //remove first and last line of the response
  //this are the state and the end of response marker
  //we don't need it
  removeStatusIndicator( &answer );
  removeEndOfResponseMarker( &answer );

  //successful download
  //show mail

  //is HTML allowed?
  KConfigGroup* configView = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );
  bool allowHTML = configView->readEntry( CONFIG_ENTRY_VIEW_USE_HTML, DEFAULT_VIEW_USE_HTML );


  int currentMail = mailsToShow.first();


  QString tsender = mails->getSenderOf( currentMail );
  QString tdate = mails->getDateOf( currentMail );
  QString tsize = mails->getSizeOf( currentMail );
  QString tsubject = mails->getSubjectOf( currentMail );
  bool isHTML;
  QStringList body = mails->decodeMailBody( answer, currentMail, allowHTML , isHTML );


	//emit signal to notify the opening of a window
  emit sigMessageWindowOpened();

  //stop timeout timer
  timeoutTimer->stop();

  //create and open the window
  QPointer<ShowMailDialog> dlg = new ShowMailDialog( kapp->activeWindow(), getName(), allowHTML, isHTML, tsender, tdate, tsize, tsubject, body );
  int ret = dlg->exec();

  delete dlg;

	//emit signal to notify the closing of a window
  emit sigMessageWindowClosed();

  //restart timeout timer
  timeoutTimer->start( timeOutTime * 1000 );


  //cancel the download if desired
  if( ret == KDialog::Rejected )
	{
		mailsToShow.clear();
		commit();
    return;
	}

  //remove the first item of the list of mails to show
  mailsToShow.removeFirst();

	//if the list of mails is empty, finalize the showing and return
  if( mailsToShow.empty() )
	{
    commit();
		return;
	}


  //show next mail in list
  showNextMail();
}

int Account::numberDeletedMailsLastRefresh( )
{
  return nmbDeletedMailsLastRefresh;
}

int Account::numberDeletedMailsStart( )
{
  return nmbDeletedMailsLastStart;
}

int Account::numberMovedMailsLastRefresh( )
{
  return nmbMovedMailsLastRefresh;
}

int Account::numberMovedMailsStart( )
{
  return nmbMovedMailsLastStart;
}

int Account::numberIgnoredMails( )
{
  return nmbIgnoredMails;
}

void Account::reloadFilterSettings( )
{
  headerFilter.load();
}

void Account::doDownloadActions()
{
  //get first mail
  getNextMailForDownloadActions();
}

void Account::getNextMailForDownloadActions()
{
    //if the list of mails to move is empty return to applyFilters
  if( mailsToDownload.empty() )
  {
    applyFilters();
    return;
  }


  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotMailDownloadedForAction() ) );

  //send download command
  sendCommand( GET_MAIL + ' ' + QString( "%1" ).arg( mailsToDownload.begin().key() ) );

}

void Account::slotMailDownloadedForAction()
{

    //get the response
  QStringList mail = readfromSocket( false );

  //no response from the server
  if( mail.isEmpty() )
  {
    handleError( i18nc( "@info error message: we could not download a mail", "<resource>%1</resource> has not sent a answer after retrieve of mail <numid>%2</numid>.", getHost(), mailsToShow.first() ) );
    finishTask();
    return;
  }

  //it is a negative response?
  if( !isPositiveServerMessage( mail ) )
  {
    handleError( i18nc( "@info error message: we could not download a mail", "Error while downloading mail <numid>%1</numid> of <resource>%2</resource>: <message>%3</message>", mailsToShow.first(), getName(), mail.first() ) );
    return;
  }

  //remove first and last line of the response
  //this are the state and the end of response marker
  //we don't need it
  removeStatusIndicator( &mail );
  removeEndOfResponseMarker( &mail );

  //successful download

  //do action
  MailToDownloadMap_Type::Iterator firstMail = mailsToDownload.begin();
  int currentMailNumber = firstMail.key();  //get mail number
  QString currentMailBox( firstMail.value().mailbox ); //get mailbox

  FilterAction_Type action = firstMail.value().action; //get action

  bool resultMove = false;    //TRUE - mail is written into the mailbox
  bool resultSpam = false;  //TRUE - mail is Spam
  bool deleteIt = false;    //TRUE - mail shall be deleted
  bool resultAction = false;  //True - the action was successful performed

  switch( action )
  {
    case FActMove       : resultMove = writeToMailBox( mail, currentMailBox );
                          //log entry is made by Mail::applyHeaderFilter
                          if( resultMove == true )
                          {
                            nmbMovedMailsLastRefresh++;
                            nmbMovedMailsLastStart++;

                            resultAction = true;
                            deleteIt = true;
                          }
                          else
                          {
                            resultAction = false;
                            deleteIt = false;
                          }
                          break;

    case FActSpamcheck  : resultSpam = isSpam( mail );  //it is spam?
                          if( resultSpam == true )          //yes, it is spam! Arrgghh! Torture it!!!
                          {
                            switch( spamAction )
                            {
                              case FActMove   : resultMove = writeToMailBox( mail, spamMailbox );
                                                if( resultMove == true )
                                                {
                                                  nmbMovedMailsLastRefresh++;
                                                  nmbMovedMailsLastStart++;

                                                  if( fLog != NULL )
                                                    mails->writeToMoveLog( fLog, currentMailNumber, getName(), spamMailbox );
                                                  resultAction = true;
                                                  deleteIt = true;
                                                }
                                                else
                                                {
                                                  resultAction = false;
                                                  deleteIt = false;
                                                }
                                                break;

                              case FActMark   : mails->setMarkAtNextViewRefresh( currentMailNumber );
                                                resultAction = true;
                                                deleteIt = false;
                                                break;

                              case FActDelete : if( fLog != NULL )
                                                  mails->writeToDeleteLog( fLog, currentMailNumber, getName(), DelFilter );

                                                nmbDeletedMailsLastRefresh++;
                                                nmbDeletedMailsLastStart++;
                                                resultAction = true;
                                                deleteIt = true;
                                                break;

                              default         : kdError() << "invalid action for spam mail" << endl;
                                                resultAction = false;
                                                deleteIt = false;
                                                break;

                            }
                          }
                          else    //mail is not spam
                          {
                            resultAction = true;
                            deleteIt = false;
                          }
                          break;

    default             : deleteIt = false;
                          resultAction = false;

  }

  if( resultAction == true )
  {
    //Action was successful
    //remove this mail from the list
    mailsToDownload.remove( firstMail.key() );

    //maybe add this mail to list of mails to delete
    if( deleteIt )
      mailsToDelete.append( currentMailNumber );
  }
  else
  {
    //Action was not successful
    //returns to applyFilters() to continue the filtering
    applyFilters();
    return;
  }


  //if the list of mails is empty, return to applyFilters() to continue the filtering
  if( mailsToDownload.empty() )
  {
    applyFilters();
    return;
  }

  //get next mail in list
  getNextMailForDownloadActions();
}

void Account::applyFiltersDeleted( )
{
  //unset the flag
  deletionPerformedByFilters = false;

  //start the second refresh cycle
  refreshPerformedByFilters = true;

  //this sends a commit and restart the refresh
  commitBeforeRefresh();
}

void Account::commitBeforeRefresh()
{
  if( socket->state() == KTcpSocket::ConnectedState )
  {
    //connect the signal readyRead of the socket with the response handle methode
    disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
    connect( socket, SIGNAL( readyRead() ), this, SLOT( slotCommitBeforeRefreshDone() ) );

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

void Account::slotCommitBeforeRefreshDone( )
{
  //after a commit was send, we start a new refresh cyle
  refreshMailList();
}

void Account::saveOptions( QDomDocument& doc, QDomElement& parent )
{
  //get application config
  KConfigGroup* config = new KConfigGroup( KGlobal::config(), getName() );

  //save the active state
  config->writeEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, isActive() );
  config->sync();

  //save the stored mails inside this account
  parent.setAttribute( ATTRIBUTE_ACCOUNT_NAME, getName() );
  mails->saveMails( doc, parent );
}

void Account::showMails()
{
  //return if no mails in the list
  if( mailsToShow.empty() )
  {
    emit sigShowBodiesReady( name );
    return;
  }

  //check whether we have a password for this account
  //if not, ask for it
  //return when no password is available
  if( !assertPassword() )
  {
    emit sigShowBodiesReady( name );
    return;
  }

  //set account state
  state = AccountDownloading;

  //connect
  doConnect();

}

bool Account::writeToMailBox( const QStringList& mail, const QString& box )
{
  QDir mailDir( box );

  //check whether the given path is a maildir
  if( !isMailDir( mailDir ) )
  {
    //show an error message
    KMessageBox::error( NULL, i18nc( "@info error message: writing a mail into a mailbox: this is not a mail box", "<filename>%1</filename> is not a mailbox.", box ) );
    return false;
  }

  //create unique file name according http://cr.yp.to/proto/maildir.html
  QString partTime = QString::number( time( NULL ) );   //left part, output of time()

  char hname[256];                                      //right part, the hostname
  QString partHostname;
  if( gethostname( hname, 255 ) == 0 )
    partHostname = QString( hname );
  else
  {
    //the hostname is not readable
    //show an error message and exit
    KMessageBox::error( NULL, i18nc( "@info error message", "Can't read the hostname of your computer. But KShowmail need it to write a mail into the mailbox." ) );
    return false;
  }

  QString partPID = QString::number( getpid() );      //middle part, the PID

  QString partCounter = QString::number( moveCounter++ );

  QString uniqueName( partTime + '.' + partPID + partCounter + '.' + partHostname );

  //build absolute path
  mailDir.cd( "tmp" );
  QString absFile = mailDir.filePath( uniqueName );

  //and writing!
  QFile file( absFile );
  if( file.open( QFile::WriteOnly ) )
  {
    QTextStream stream( &file );
    stream << mail.join( "\n" ) << endl;
    file.close();
  }
  else
  {
    KMessageBox::detailedError( NULL, i18nc( "@info error message: we could not write a mail into a mailbox", "Could not file a mail to <filename>%1</filename>.", box ), file.errorString() );
    return false;
  }

  //now we move it to the "new" subdirectory
  mailDir.cdUp();
  mailDir.cd( "new" );
  QString absNewFile = mailDir.filePath( uniqueName );

  if( KDE::rename( absFile.toAscii(), absNewFile.toAscii() ) == -1 )
  {
    KMessageBox::error( NULL, i18nc( "@info error message: error during writing a mail into a mailbox", "Could not move a mail from <filename>%1</filename> to <filename>%2</filename>.", absFile, absNewFile ) );
    return false;
  }

  //the writing was successful
  return true;
}

bool Account::isMailDir( const QDir& path )
{
  //get a list of all subdirectories in this directory
  const QStringList entries = path.entryList( QDir::Dirs | QDir::Readable | QDir::Writable | QDir::Hidden, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware );

  //a maildir folder must contains the folders "cur", "new" and "tmp"
  bool curFound = false;
  bool newFound = false;
  bool tmpFound = false;

  //iterate over all directories and look for the three necessary dirs
  QStringList::const_iterator it = entries.begin();
  while( it != entries.end() && !( curFound && newFound && tmpFound ) )
  {
    if( *it == "tmp" )
      tmpFound = true;
    else if( *it == "cur" )
      curFound = true;
    else if( *it == "new" )
      newFound = true;

    ++it;
  }

  return curFound && newFound && tmpFound;
}

bool Account::isSpam( QStringList mail ) const
{

  //check for a running spamassassin
  if( !isSpamAssassinRunning() )
  {
    KMessageBox::information( NULL, i18nc( "@info", "You want to check your mails for spam, but SpamAssassin is not running.<nl/>KShowmail skips the spam check." ), i18nc( "@title:window", "SpamAssassin is not running" ), "AccountNoSpamAssassinRunning" );
    return false;
  }

  //calls spmac and get an file pointer to stdin of it
  FILE *write_fp;
  write_fp = popen( "spamc -E", "w" );

  //forward the mail to SpamAssassin
  if( write_fp != NULL )
  {
    //join the mail to a string
    QString joinedMail = mail.join( "\n" );
    fwrite( joinedMail.toAscii(), sizeof( char), joinedMail.size(), write_fp );

    //check exit code of spamc and return result
    int excode = pclose( write_fp );
    if(  excode == 0 )
      return false;
    else
      return true;
  }
  else
  {
    kdError() << "Could not call the command spamc of SpamAssassin." << endl;
    return false;
  }

  return false;
}

bool Account::isSpamAssassinRunning( ) const
{
  FILE *read_fp;
  char buffer[ BUFSIZ + 1 ];
  int chars_read;
  bool found = false;

  memset( buffer, '\0', sizeof( buffer ) );
  read_fp = popen( "sa-check_spamd"
                   , "r" );
  if( read_fp != NULL )
  {
    chars_read = fread( buffer, sizeof( char ), BUFSIZ, read_fp );
    while( chars_read > 0 )
    {
      buffer[ chars_read - 1 ] = '\0';
      QString output( buffer );
      found = output.contains( "SPAMD OK" ) > 0;
      if( found ) return found;
      chars_read = fread( buffer, sizeof( char ), BUFSIZ, read_fp );
    }
    pclose( read_fp );
  }

  return found;
}

int Account::getNumberNewMails( )
{
  return mails->getNumberNewMails();
}

long Account::getTotalSize() const {
  return mails->getTotalSize();
}

QString Account::getTotalSizeUnit() const {

  int size = getTotalSize();
  QString strSize;

  if( size >= 1024 * 1024 )
  {
    //prefix is mega
    strSize = QString( "%L1M" ).arg( ( (double)size / ( 1024 * 1024 ) ), 0, 'f', 1 );
  }
  else if( size >= 1024 )
  {
    //prefix is kilo
    strSize = QString( "%L1K" ).arg( ( (double)size / 1024 ), 0, 'f', 1 );
  }
  else
    //no prefix
    strSize = QString( "%L1" ).arg( size );

  return strSize;

}

void Account::slotSSLError( const QList<KSslError>& errors ) {

  //make a single error string
  QString message;
  QListIterator<KSslError> it( errors );
  while( it.hasNext() ) {

    KSslError error = it.next();

    message.append( error.errorString() );
  }

  //ask the user whether he want to cancel or continue
  emit sigMessageWindowOpened();
  int answer = KMessageBox::warningContinueCancel( NULL,
                                                   i18nc( "@info error message: general SSL error", "SSL error: <message>%1</message><nl/>Do you want to continue?", message ),
                                                   i18nc( "@title:window", "SSL-Error - %1", getName() ),
                                                   KStandardGuiItem::cont(),
                                                   KStandardGuiItem::cancel(),
                                                   QString( "askSSLErrorContinue_%1").arg( getName() ) );
  emit sigMessageWindowClosed();

  if( answer == KMessageBox::Continue ) {

    //continue
    socket->ignoreSslErrors();

  } else {

    //the socket calls slotSocketError, but we don't want a further error message
    dontHandleError = true;

    //cancel
    finishTask();

  }

  return;

}

void Account::readStoredMails( QDomElement& parent )
{
  //get mails
  mails->readStoredMails( parent );
}

int Account::compare( Account* other, AccountSort_Type property ) {

	switch( property ) {

		//compare by active state
		case AccSortActive : {

			if( other->isActive() == isActive() ) return 0;
			else if( isActive() == false ) return -1;
			else return 1;
			break;

		}

		//compare by name
		case AccSortName : {

			return QString::localeAwareCompare( getName(), other->getName() );
		}

		//compare by server
		case AccSortServer : {

			return QString::localeAwareCompare( getHost(), other->getHost() );
		}

		//compare by user
		case AccSortUser : {

			return QString::localeAwareCompare( getUser(), other->getUser() );
		}

		//compare by number of messages
		case AccSortNrMess : {

			if( getNumberMails() == other->getNumberMails() ) return 0;
			else if( getNumberMails() < other->getNumberMails() ) return -1;
			else return 1;
		}

		//compare by total size of messages
		case AccSortSize : {

			if( getTotalSize() == other->getTotalSize() ) return 0;
			else if( getTotalSize() < other->getTotalSize() ) return -1;
			else return 1;
		}

		default: {
			return QString::localeAwareCompare( getName(), other->getName() );
		}
	}
}

QList<Mail> Account::getAllMails() const
{
	if( isActive() )
		return mails->getAllMails();
	else
		return QList<Mail>();
}

void Account::slotTimeout()
{
  handleError( i18nc( "@info error message", "Timeout" ) );
}

void Account::cancelTask()
{
  if( state != AccountIdle ) {

    socket->close();
    handleError( i18nc( "@info the task was canceled by user", "Task canceled" ) );
  }
}

void Account::startTLS()
{
  //connect the signal readyRead of the socket with the response handle methode
  disconnect( socket, SIGNAL( readyRead() ), 0, 0 );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( slotStartTLSResponse() ) );

  //send the command
  sendCommand( START_TLS );

}

void Account::slotStartTLSResponse()
{
  //get server answer
  QStringList text = readfromSocket( true );

  //if the socket has not returned something, we finish at this point
  //we don't need to show an error message, because the handleError-Methode was called already
  if( text.isEmpty() )
  {
    finishTask();
    return;
  }

  //have we got a acknowledgment?
  bool ack = isPositiveServerMessage( text );

  //if the server isn't ready for TLS, continue without TLS
  if( !ack ) {

    kdError() << "The Server " << getHost() << " says it supports STLS but it doesn't accept the STLS command: " << text.first() << endl;

    if( apopAvail && !dontUseAPOP ) {

      loginApop();
      return;

    } else {

      if( allowUnsecureLogin == true ) {

        loginUser();

      } else {

        emit sigMessageWindowOpened();
        KMessageBox::sorry( NULL, i18nc( "@info error message: this server doesn't support secure login", "Account <resource>%1</resource>: This server doesn't provide a safety login and you have disallowed the using of an unsafe login. If you want to use this Account you must allow unsafe login at the account setup.<nl/><warning>Bear in mind in this case criminals could read your password!</warning>", getName() ), i18nc( "@title:window", "Unsafe login is not allowed") );
        emit sigMessageWindowClosed();

        //the user has not allowed unsafe login; finish the task
        finishTask();

      }
    }

  } else {

    //Yes, lets start TLS!!!

    socket->startClientEncryption();

    //login
    loginUser();
  }

}

AccountViewItem Account::getViewItem()
{
  return AccountViewItem( isActive(), getName(), getHost(), getUser(), getNumberMails(), getTotalSize(), QPointer<Account>( this ) );
}

