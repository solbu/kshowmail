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

class Account;

//C++ headers
#include <iostream>

//Qt header
#include <QObject>
#include <QList>
#include <QApplication>
#include <QTimer>
#include <QTextCodec>

//KDE headers
#include <KConfigGroup>
#include <KConfig>
#include <KGlobal>
#include <KPasswordDialog>
#include <kcodecs.h>
#include <ktcpsocket.h>

//KShowmail headers
#include "mail.h"
#include "types.h"
#include "constants.h"
#include "encryption.h"
#include "kwalletaccess.h"
#include "maillist.h"
#include "accountlist.h"
#include "corruptdataexception.h"


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

  /**
   * Returns the state of the account.
   * @return account state
   */
   Types::AccountState_Type getState();

   /**
    * Return whether an unsecure login is allowed.
    * @return TRUE - unsecure login is allowed
    */
   bool isUnsecureLoginAllowed() const;

  /**
    * Returns the number of mails if this account is active. Otherwise returns 0.
    * @return number of mails or 0
   */
   int getNumberMails() const;

    /**
     * Returns the mail with the given number.<p>
     * The number is not the number which is given by the mail server.
     * It is the number in order of storage in this list.<p>
     * @param number number of the desired mail
     * @return pointer to the desired mail
     * @throws CorruptDataException no mail with this number is available
     */
    Mail* getMail( int number ) const throw ( CorruptDataException );

    /**
     * Adds a mail number to the list of mails to delete.<p>
     * The number is the mail number given by the mail server.
     * @param number mail number
     * @see mailsToDelete
     */
    void addMailToDelete( int number );

  /**
   * Deletes mails which are listed in mailsToDelete.
   * This just starts the deletion and returns after then.
   * When the deletion is ready the signal sigDeleteReady will be emitted.
   */
   virtual void deleteMails();



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
     * @param charset the Charset
     * @param singleLine TRUE - the response will be a single line; FALSE - the response has more lines
     * @return read text
     */
    QStringList readfromSocket( QString charset, bool singleLine );

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

    /**
     * Sends the login name.
     * The response will be received by slotLoginUserResponse().
     * @see slotLoginUserResponse
     */
    void loginUser();

    /**
     * Sends the password for login
     * The response will be received by slotLoginPasswdResponse().
     * @see slotLoginPasswdResponse
     */
    void loginPasswd();

    /**
     * Initiates a login using APOP.
     * The response will be received by slotLoginApopResponse().
     * @see slotLoginApopResponse()
     */
    void loginApop();

    /**
     * Removes the status indicator from a single line server message.
     * @param message server message
     * @return server message without status indicator
     */
    QString removeStatusIndicator( const QString& message );

    /**
     * If the first line of the server response is the status indicator (+OK or -ERR), this methode will
     * remove this line.
     */
    void removeStatusIndicator( QStringList* response );

    /**
     * If the last line of the server response is the end of response marker,
     * this methode will remove this line.
     */
    void removeEndOfResponseMarker( QStringList* response );

    /**
     * Sends the UIDL command to get a list of the mail UID's
     * The response will be received by the slotUIDListResponse.
     * @see slotUIDListResponse
     */
    void getUIDList();

    /**
     * Copies the pointer to the temporary list to the mail list.
     * The temporary list will be the current mail list.
     */
    void swapMailLists();

    /**
     * Sends the LIST command to get a list of mail sizes.
     * The response will be received by slotMailListResponse.
     * @see slotMailSizesResponse
     */
    void getMailSizes();

    /**
     * Gets the headers of the new mails from the server.
     */
    void getHeaders();

    /**
     * Get the header of the first mail in newMails.
     * After a succesful download this
     * mail will be removed from the list by slotHeaderDownloaded() and this
     * method will be invoked again.
     * If the list is empty, it will call copyHeaders() to get the known
     * headers from the old mail list.
     * @see copyHeaders()
     * @see slotHeaderDownloaded()
     */
    virtual void getNextHeader();

    /**
     * Returns whether the given string is a negative server response.
     * @param response server response to check
     * @return TRUE - response is negative
     * @return FALSE - response is positive
     */
    bool isNegativeResponse( const QString& response );

    /**
     * Copies the known headers of the old mails from the
     * old mail list to the new one.
     * At the end it invokes swapMailLists().
     */
    void copyHeaders();


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
   void slotSocketError( KTcpSocket::Error ErrorCode );

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

    /**
     * Connected with signal readyRead of the socket by commit().
     * Analyzes the commit response of the server.
     * @see ccommit()
     */
    void slotCommitResponse();

    /**
     * Received the response of the USER command during login.
     * @see loginUser()
     */
    void slotLoginUserResponse();

    /**
     * Received the response of the PASS command during login.
     * @see loginPasswd()
     */
    void slotLoginPasswdResponse();

    /**
     * Receives the response of a login using APOP.
     * @see loginApop()
     */
    void slotLoginApopResponse();

    /**
     * Reiceves the response of the UIDL command, which gets the UID's of the mails.
     * @see getUIDList()
     */
    void slotUIDListResponse();

    /**
     * step of the refresh cycle.
     * Applies the filters to the mails in the mail list.
     * Invoked by swapMailLists().
     * This methode maybe starts a new refresh cycle because after a deletion or moving we need a
     * actual list of mails on the server.
     * To avoid a never-ending loop you must not call this methode during the second refresh cycle.
     * Therefore it sets filterApplied to TRUE.
     * @see filterApplied
     * @see applyFiltersDeleted()
     */
    void applyFilters();

    /**
     * Receives the response of the LIST command, which gets the
     * numbers and sizes of the mails
     * @see getMailSizes()
     */
    void slotMailSizesResponse();

    /**
     * Receives the header requested by getNextHeader().
     * Stores the received headers into the appropriate instance of the mail list.
     * Removes the first mail from newMails and invokes
     * getNextHeader() again to get the next header.
     * If the list is empty after it has removed the first item, it will call
     * copyHeaders().
     */
    void slotGetHeaderResponse();


		
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
     * While refreshing, this mail list instance will be used to
     * build a new mail list. After that this pointer will be assigned
     * to maillist.
     */
    MailList* tempMailList;
    

    /**
     * The socket. Handles all server operations
     */
    KTcpSocket* socket;

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

    /**
     * APOP available
     */
    bool apopAvail;

    /**
     * Is set to TRUE by slotLoginAPOPResponse if the first login with APOP is failed.
     * After a setup load it will set to false.
     */
    bool dontUseAPOP;

    /**
     * Timestamp for APOP sent with the server greeting
     */
    QString apopTimestamp;

    /**
     * TRUE - unsafe login is allowed if a secure login failed
     */
    bool allowUnsecureLogin;

   /**
     * This is a flag whether the filters was applied during a refresh cycle.
     * If some mails was deleted (recommend by the filter), we need a actual
     * list of mails from the server. Therefore a second refresh cycle will be started.
     * To avoid a never-ending loop this flag will be set to TRUE by applyFilters().
     * If the second refresh is running this one will not invoke the filters.
    */
    bool filterApplied;

    /**
     * This flag is set by applyFiltersDeleted() to indicate the refresh cycle is the second one after a deletion which was performed by filters.
     * The refresh cycle need it to hold the remaining mails as new.
     */
    bool refreshPerformedByFilters;

    /**
     * Contains the numbers of new mails.
     * Set by getHeaders() and used by getNextHeader() to download
     * the headers of the new mails.
     * @see getHeaders()
     * @see getNextHeader()
     */
    MailNumberList_Type newMails;

   /**
     * Contains the numbers of mails which will be deleted.
     * Set by addMailToDelete() and used by deleteNextMail() to get
     * the number of the next mail to delete.
     * @see addMailToDelete()
     * @see deleteNextMail()
    */
    MailNumberList_Type mailsToDelete;


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
