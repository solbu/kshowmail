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

#ifndef ACCOUNT_H
#define ACCOUNT_H

//C++ headers
#include <iostream>

//Qt header
#include <QObject>
#include <QList>
#include <QApplication>
#include <QTcpSocket>
#include <QTimer>

//KDE headers
#include <KConfigGroup>
#include <KConfig>
#include <KGlobal>
#include <KPasswordDialog>

//KShowmail headers
#include "mail.h"
#include "types.h"
#include "constants.h"
#include "encryption.h"
#include "kwalletaccess.h"
#include "maillist.h"
#include "accountlist.h"


/**
 * @brief Constants of the POP3 protocol
 */
namespace POP3Constants
{
  /**
   * positive server response
   */
  static const QString RESPONSE_POSITIVE( "+OK" );

  /**
   * negative server response
   */
  static const QString RESPONSE_NEGATIVE( "-ERR" );

  /**
   * End of multi-line response
   */
  static const QString END_MULTILINE_RESPONSE( "." );

  /**
   * Capabilities request
   */
  static const QString CAPA_REQUEST( "CAPA" );

  /**
   * Authentification mechanism request
   */
  static const QString AUTH_REQUEST( "AUTH" );

  /**
   * Capabilities response STLS available
   */
  static const QString CAPA_RESPONSE_STLS( "STLS" );

  /**
   * Commit command
   */
  static const QString COMMIT( "QUIT" );

  /**
   * Sends the username for login
   */
  static const QString LOGIN_USER( "USER" );

  /**
   * Sends the password for login
   */
  static const QString LOGIN_PASSWD( "PASS" );

  /**
   * Command for login using APOP
   */
  static const QString LOGIN_APOP( "APOP" );

  /**
   * Command to get the UID list
   */
  static const QString UID_LIST( "UIDL" );

  /**
   * Command to get a list of mail numbers and sizes
   */
  static const QString MAIL_LIST( "LIST" );

  /**
   * This command gets the header of a mail
   */
  static const QString GET_HEADER( "TOP" );

  /**
   * Deletes a mail
   */
  static const QString DELETE( "DELE" );

  /**
   * Get a mail
   */
  static const QString GET_MAIL( "RETR" );
}

using namespace POP3Constants;
using namespace std;
using namespace Types;
using namespace Encryption;
using namespace KWalletAccess;

class AccountList;

/**
 * This class represents an POP3-Account
 */
class Account : public QObject
{

  Q_OBJECT

  public:

		/**
		 * Constructor
		 * @param name name of this account
     * @param accountList pointer to the parent account list
		 * @param parent parent object
		 */
		Account( QString name, AccountList* accountList, QObject* parent );
		
		/**
		 * Destructor
		 */
		~Account();
		
		/**
		 * Prints all data of this account to stdout
		 */
		void print() const;
		 
		/**
		 * Returns the account name
		 * @return account name
		 */
		QString getName() const;
		
    /**
     * Returns whether the account is active.
     * @return TRUE - account is active; FALSE - account is not active
     */
    bool isActive() const;

    /**
     * Sets the account active.
     * @param active TRUE - active account; FALSE - deactivate account
     */
    void setActive( bool active );

    /**
     * Loads the options of this account from the application config.
     */
    void load();

  /**
   * Refreshes the mail list.
   * This just starts the refresh and returns after then.
   * When the refresh is ready, the signal sigRefreshReady
   * will be emitted.
   * @see sigRefreshReady
   */
  void refreshMailList( );

   /**
    * Gets the account password.
    * @return password
    */
   QString getPassword() const;

   /**
    * Sets the account password.
    * @param password new password
    */
   void setPassword( const QString& password );

   /**
    * Sets the host name.
    * @param host host name
    */
   void setHost( const QString& host );

   /**
    * Returns the hostname.
    * @return the name of the host or QString::null if no host is set
    */
   QString getHost() const;

   /**
    * Sets the protocol.
    * @param protocol the protocol
    */
   void setProtocol( const QString& protocol );

   /**
    * Returns the protocol.
    * @param upperCase TRUE - transforms the protocol string to upper case
    * @return protocol
    */
   QString getProtocol( bool upperCase = false ) const;

   /**
    * Sets the port.
    * @param port the port
    */
   void setPort( unsigned short int port );

   /**
    * Returns the port number.
    * @return port number
    */
   unsigned short int getPort() const;

   /**
    * Sets the user.
    * @param user username
    */
   void setUser( const QString& user );

   /**
    * Returns the user name.
    * @return the user name or QString::null if there is no user name
    */
   QString getUser() const;

   /**
    * Sets the password storage type.
    * The Constants are defined in constants.h
    * @param storage type of password storage; valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
    */
   void setPasswordStorage( int storage );

   /**
    * Returns the password storage type.
    * The Constants are defined in constants.h.
    * @return type of password storage; valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
    */
   int getPasswordStorage() const;

  protected:

    /**
     * Initiate the account
     */
    void init();

   /**
    * Returns whether a password is stored.
    * @return TRUE - there is password stored; FALSE - no password stored
    */
   bool hasPassword() const;

  /**
   * Opens a dialog to ask for a password, stores it temporarily in the account settings and
   * emits the signal sigConfigChanged.
   * Does not open a dialog if a password is already stored in the account settings.
   * @param force ask for a new password even if a password is stored
   * @return TRUE - a password is available
   * @return FALSE - no password is available
   */
  bool assertPassword( bool force = false );

  /**
   * Establishes a connection to the server
   */
  void doConnect();

  /**
   * Closes the connection
   */
  void closeConnection();

  /**
   * Initiates the account to do a connect.
   */
  void initBeforeConnect();

    /**
     * Shows the given error, sets the state to idle and emits a ready signal if neccessary
     * @param error the error message
     */
    void handleError( QString error );

    /**
     * Reads all data from the socket
     * @return read text
     */
    QStringList readfromSocket();

    /**
     * Sends a command to the server
     * @param command the command to send
     */
    void sendCommand( const QString& command );

    /**
     * Gets the capabilities from the server.
     * Sends the CAPA-command connect the socket with slot slotReceiveCapabilities()
     * @see slotCapabilitiesResponse
     */
    void getCapabilities();
    
    /**
     * Prints the a text received from the server to stdout
     * @param text server message
     */
    void printServerMessage( QStringList& text ) const;

    /**
     * Returns whether the server answer is positive or negative
     * @param message server message
     * @return TRUE - positive answer; FALSE - negative answer
     */
    bool isPositiveServerMessage( QStringList& message ) const;

    /**
     * Removes status indicator and termination char from a server message
     * @param message server message
     * @return the cleared server message
     */
    void clearMessage( QStringList& message );

    /**
     * Sends the command to get the authentication mechanism of this server.
     * Connects the signal readyRead of the socket with the slot
     * slotAuthMechResponse(). If the socket has got the server
     * response it will send this signal
     * @see slotAuthMechResponse()
     */
    void getAuthMech();

    /**
     * Sends the commit command to end the session.
     * Connects the signal readyRead of the socket with the slot
     * slotCommitResponse().
     * @see slotCommitResponse()
     */
    virtual void commit();

    /**
     * Finishes the running task (Refreshing, Deleting, ...)
     * Emits the proper ready signal and closes the connection
     * @see closeConnection()
     * @see sigDeleteReady()
     * @see sigRefreshReady()
     * @see sigShowBodiesReady()
     */
    void finishTask();

    


  protected slots:

   /**
    * Connected with signal connected of the socket.
    * Will be invoked after the socket has connected the host
    */
   virtual void slotConnected();

   /**
    * Connected with signal hostFound of the socket.
    * Will be invoked after the socket has done the DNS lookup
    */
   virtual void slotHostFound();

   /**
    * Connected with signal error of the socket.
    * Will be invoked, when an socket error occures.
    * @param errorNr Error code of the socket
    */
   void slotSocketError( QAbstractSocket::SocketError ErrorCode );

    /**
     * Reads the first message send by server.
     */
    void slotReadFirstServerMessage();

    /**
     * Reads the capabilities from the server.
     * @see getCapabilities()
     */
    void slotCapabilitiesResponse();

    /**
     * Connected with the signal readyRead of the socket by getAuthMech().
     * Analyzes the response and sets the authentification mechanism flags
     * of this account.
     * @see getAuthMech
     */
    void slotAuthMechResponse();


		
	private:
		
		/**
		 * Name of this account
		 */
		QString name;
		
    /**
     * TRUE - account is active; FALSE - account is not active
     */
    bool active;

    /**
     * Type of the transfer security
     */
    TransferSecurity_Type transferSecurity;

    /**
     * Uniform Resource Locator of the account on the mail server.
     */
    KUrl url;

    /**
     * Type of password storage.
     * valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
     * The Constants are defined in constants.h
     * @see getPasswordStorage()
     * @see setPasswordStorage()
     */
    int passwordStorage;

    /**
     * The mail container
     */
    MailList* mails;

    /**
     * The socket. Handles all server operations
     */
    QTcpSocket* socket;

    /**
     * Pointer to the account list object
     */
    AccountList* accountList;

    /**
     * TRUE - the QUIT command was sent to the server. Normally set by the commit methode.
     * Inititates by initBeforeConnect()
     * @see initBeforeConnect()
     */
    bool quitSent;

    /**
     * This timer kills a running operation after the preset time
     */
    QTimer* timeoutTimer;

    /**
     * State of the account.
     */
    Types::AccountState_Type state;



  signals:

    /**
     * Will be emitted when all selected mails are deleted.
     * @param account name of the account, which has emitted this signal
     */
    void sigDeleteReady( QString account );

    /**
     * Will be emitted when the bodies of all selected mails are
     * downloaded and shown.
     * @param account name of the account, which has emitted this signal
     */
    void sigShowBodiesReady( QString account );

    /**
     * Will be emitted when the settings have been changed.
     */
    void sigConfigChanged();

    /**
     * Will be emitted, when a window to show a message was opened.
     */
    void sigMessageWindowOpened();

    /**
     * Will be emitted, when a window to show a message was closed.
     */
    void sigMessageWindowClosed();

    /**
     * Will be emitted, when the mail list was refreshed.
     * @param account name of the account, which has emitted this signal
     */
    void sigRefreshReady( QString account );

    
		
};

#endif // ACCOUNT_H
