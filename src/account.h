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

using namespace std;
using namespace Types;
using namespace Encryption;
using namespace KWalletAccess;

class AccountList;

/**
 * This class represents a account
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
