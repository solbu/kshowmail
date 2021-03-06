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

#ifndef ACCOUNT_H
#define ACCOUNT_H

class Account;

//C++ headers
#include <iostream>
#include <unistd.h>

//Qt header
#include <QObject>
#include <QList>
#include <QApplication>
#include <QTimer>
#include <QTextCodec>
#include <QDir>
#include <QSslCertificate>
#include <QThread>
#include <QByteArray>
#include <QPointer>

//KDE headers
#include <KConfigGroup>
#include <KConfig>
#include <KGlobal>
#include <KPasswordDialog>
#include <kcodecs.h>
#include <ktcpsocket.h>
#include <KApplication>
#include <KLocale>
#include <KMessageBox>
#include <kde_file.h>


//KShowmail headers
#include "mail.h"
#include "types.h"
#include "constants.h"
#include "encryption.h"
#include "kwalletaccess.h"
#include "maillist.h"
#include "accountlist.h"
#include "corruptdataexception.h"
#include "headerfilter.h"
#include "filterlog.h"
#include "showmaildialog.h"
#include "kshowmail.h"
#include "accountviewitem.h"

/**
 * @brief Constants of the POP3 protocol
 */
namespace POP3Constants
{
  /**
   * @brief positive server response
   */
  static const QString RESPONSE_POSITIVE( "+OK" );

  /**
   * @brief negative server response
   */
  static const QString RESPONSE_NEGATIVE( "-ERR" );

  /**
   * @brief End of multi-line response
   */
  static const QString END_MULTILINE_RESPONSE( "." );

  /**
   * @brief Capabilities request
   */
  static const QString CAPA_REQUEST( "CAPA" );

  /**
   * @brief Authentication mechanism request
   */
  static const QString AUTH_REQUEST( "AUTH" );

  /**
   * @brief Capabilities response STLS available
   */
  static const QString CAPA_RESPONSE_STLS( "STLS" );

  /**
   * @brief command for start TLS
   */
  static const QString START_TLS( "STLS" );

  /**
   * @brief Commit command
   */
  static const QString COMMIT( "QUIT" );

  /**
   * @brief Sends the username for login
   */
  static const QString LOGIN_USER( "USER" );

  /**
   * @brief Sends the password for login
   */
  static const QString LOGIN_PASSWD( "PASS" );

  /**
   * @brief Command for login using APOP
   */
  static const QString LOGIN_APOP( "APOP" );

  /**
   * @brief Command to get the UID list
   */
  static const QString UID_LIST( "UIDL" );

  /**
   * @brief Command to get a list of mail numbers and sizes
   */
  static const QString MAIL_LIST( "LIST" );

  /**
   * @brief This command gets the header of a mail
   */
  static const QString GET_HEADER( "TOP" );

  /**
   * @brief Deletes a mail
   */
  static const QString DELETE( "DELE" );

  /**
   * @brief Get a mail
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
 * @brief This class represents an POP3-Account
 */
class Account : public QObject
{

  Q_OBJECT

  public:

		/**
		 * @brief Constructor
		 * @param name name of this account
     * @param accountList pointer to the parent account list
		 * @param parent parent object
		 */
		Account( QString name, AccountList* accountList, QObject* parent );

		/**
		 * @brief Destructor
		 */
		~Account();

		/**
		 * @brief Prints all data of this account to stdout
		 */
		void print() const;

		/**
		 * @brief Returns the account name
		 * @return account name
		 */
		QString getName() const;

    /**
     * @brief Returns whether the account is active.
     * @return TRUE - account is active; FALSE - account is not active
     */
    bool isActive() const;

    /**
     * @brief Sets the account active.
     * @param active TRUE - active account; FALSE - deactivate account
     */
    void setActive( bool active );

    /**
     * @brief Loads the options of this account from the application config.
     */
    void load();

  /**
   * @brief Refreshes the mail list.
   *
   * This just starts the refresh and returns after then.
   * When the refresh is ready, the signal sigRefreshReady
   * will be emitted.
   * @param log pointer to the filter log
   * @see sigRefreshReady
   */
  void refreshMailList( FilterLog* log = NULL );

   /**
    * @brief Gets the account password.
    * @return password
    */
   QString getPassword() const;

   /**
    * @brief Sets the account password.
    * @param password new password
    */
   void setPassword( const QString& password );

   /**
    * @brief Sets the host name.
    * @param host host name
    */
   void setHost( const QString& host );

   /**
    * @brief Returns the hostname.
    * @return the name of the host or QString::null if no host is set
    */
   QString getHost() const;

   /**
    * @brief Sets the protocol.
    * @param protocol the protocol
    */
   void setProtocol( const QString& protocol );

   /**
    * @brief Returns the protocol.
    * @param upperCase TRUE - transforms the protocol string to upper case
    * @return protocol
    */
   QString getProtocol( bool upperCase = false ) const;

   /**
    * @brief Sets the port.
    * @param port the port
    */
   void setPort( unsigned short int port );

   /**
    * @brief Returns the port number.
    * @return port number
    */
   unsigned short int getPort() const;

   /**
    * @brief Sets the user.
    * @param user username
    */
   void setUser( const QString& user );

   /**
    * @brief Returns the user name.
    * @return the user name or QString::null if there is no user name
    */
   QString getUser() const;

   /**
    * @brief Sets the password storage type.
    *
    * The Constants are defined in constants.h
    * @param storage type of password storage; valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
    */
   void setPasswordStorage( int storage );

   /**
    * @brief Returns the password storage type.
    * The Constants are defined in constants.h.
    * @return type of password storage; valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
    */
   int getPasswordStorage() const;

  /**
   * @brief Returns the state of the account.
   * @return account state
   */
   Types::AccountState_Type getState();

   /**
    * @brief Return whether an unsecure login is allowed.
    * @return TRUE - unsecure login is allowed
    */
   bool isUnsecureLoginAllowed() const;

  /**
    * @brief Returns the number of mails if this account is active. Otherwise returns 0.
    * @return number of mails or 0
   */
   int getNumberMails() const;

    /**
     * @brief Adds a mail number to the list of mails to delete.
     *
     * The number is the mail number given by the mail server.
     * @param number mail number
     * @see mailsToDelete
     */
    void addMailToDelete( int number );

    /**
     * @brief Adds a mail number to the list of mails to show.
     *
     * The number is the mail number given by the mail server.
     * @param number mail number
     * @see mailsToShow
     */
    void addMailToShow( int number );

    /**
     * @brief Deletes mails which are listed in mailsToDelete.
     *
     * This just starts the deletion and returns after then.
     * When the deletion is ready the signal sigDeleteReady will be emitted.
     */
     virtual void deleteMails();

    /**
     * @brief Returns the number of deleted mails by last refresh.
     * @return number of deleted mails by last refresh
     */
    int numberDeletedMailsLastRefresh();

    /**
     * @brief Returns the number of deleted mails since start.
     * @return number of deleted mails since start
     */
  int numberDeletedMailsStart();

    /**
     * @brief Returns the number of moved mails by last refresh.
     * @return number of moved mails by last refresh
     */
  int numberMovedMailsLastRefresh();

    /**
     * @brief Returns the number of moved mails since start.
     * @return number of moved mails since start
     */
  int numberMovedMailsStart();

    /**
     * @brief Returns the number of ignored mails by last refresh.
     * @return number of ignored mails by last refresh
     */
    int numberIgnoredMails();

    /**
    * @brief Reloads the settings of the filters.
    *
    * It just calls the load() methode of the header filter.
    */
    void reloadFilterSettings();

    /**
    * @brief Saves the stored mails into the given DOM document.
    *
    * It doesn't create a new DOM element but used the given one.
    * Saves the setup into the application config.
    * @param doc DOM document which contains all application settings, mails and account settings
    * @param parent DOM element in which all settings and mails of this account will be saved
    */
    void saveOptions( QDomDocument& doc, QDomElement& parent );

    /**
     * @brief Downloads and shows the bodies of mails in <code>mailsToShow</code>.
     *
     * This just starts the download and returns after then.
     * When the download is ready and the body is shown the sigShowBodiesReady
     * will be emitted.
     * @see mailsToShow
     */
    void showMails();

    /**
     * @brief Returns the number of new mails.
     * @return number of new mails
     */
    int getNumberNewMails();

    /**
     * @brief Returns the total size (bytes) of all mails.
     * @return total size
     */
    long getTotalSize() const;

    /**
     * @brief Returns the total size of all mails with unit.
     */
    QString getTotalSizeUnit() const;

    /**
     * @brief Gets the stored mails.
     * @param parent DOM element which contains the mails of this account
     */
    void readStoredMails( QDomElement& parent );

    /**
		 * @brief Compares this account with <i>other</i> and returns an integer less than,
		 * equal to, or greater than zero if this account is less than, equal to,
		 * or greater than <i>other</i>.
		 * @param other other account
		 * @param property the account property which will be compared
		 */
		int compare( Account* other, AccountSort_Type property );

		/**
		 * @brief Returns a list which contains the pointers to all mails.
		 * @return all mails
		 */
		QList<Mail> getAllMails() const;

    /**
     * @brief Cancels the current running task
     */
    void cancelTask();

    /**
     * @brief Returns a appropriate view item
     * @return view item
     */
    AccountViewItem getViewItem();


  protected:

    /**
     * @brief Initiate the account
     */
    void init();

   /**
    * @brief Returns whether a password is stored.
    * @return TRUE - there is password stored; FALSE - no password stored
    */
   bool hasPassword() const;

  /**
   * @brief Opens a dialog to ask for a password, stores it temporarily in the account settings and
   * emits the signal sigConfigChanged.
   *
   * Does not open a dialog if a password is already stored in the account settings.
   * @param force ask for a new password even if a password is stored
   * @return <code>true</code> - a password is available; <code>false</code> - no password is available
   */
  bool assertPassword( bool force = false );

  /**
   * @brief Establishes a connection to the server
   */
  void doConnect();

  /**
   * @brief Closes the connection
   */
  void closeConnection();

  /**
   * @brief Initiates the account to do a connect.
   *
   * It also creates the socket.
   */
  void initBeforeConnect();

    /**
     * @brief Shows the given error, sets the state to idle and emits a ready signal if necessary
     * @param error the error message
     */
    void handleError( QString error );

    /**
     * @brief Reads all data from the socket
     * @param singleLine TRUE - the response will be a single line; FALSE - the response has more lines
     * @return read text
     */
    QStringList readfromSocket( bool singleLine );

    /**
     * @brief Sends a command to the server
     * @param command the command to send
     */
    void sendCommand( const QString& command );

    /**
     * @brief Gets the capabilities from the server.
     * Sends the CAPA-command connect the socket with slot slotReceiveCapabilities()
     * @see slotCapabilitiesResponse
     */
    void getCapabilities();

    /**
     * @brief Prints the a text received from the server to stdout
     * @param text server message
     */
    void printServerMessage( QStringList& text ) const;

    /**
     * @brief Returns whether the server answer is positive or negative
     * @param message server message
     * @return TRUE - positive answer; FALSE - negative answer
     */
    bool isPositiveServerMessage( QStringList& message ) const;

    /**
     * @brief Sends the command to get the authentication mechanism of this server.
     *
     * Connects the signal readyRead of the socket with the slot
     * slotAuthMechResponse(). If the socket has got the server
     * response it will send this signal
     * @see slotAuthMechResponse()
     */
    void getAuthMech();

    /**
     * @brief Sends the commit command to end the session.
     *
     * Connects the signal readyRead of the socket with the slot
     * slotCommitResponse().
     * @see slotCommitResponse()
     */
    virtual void commit();

    /**
     * @brief Finishes the running task (Refreshing, Deleting, ...)
     *
     * Emits the proper ready signal and closes the connection
     * @see closeConnection()
     * @see sigDeleteReady()
     * @see sigRefreshReady()
     * @see sigShowBodiesReady()
     */
    void finishTask();

    /**
     * @brief Sends the login name.
     *
     * The response will be received by slotLoginUserResponse().
     * @see slotLoginUserResponse
     */
    void loginUser();

    /**
     * @brief Sends the password for login
     *
     * The response will be received by slotLoginPasswdResponse().
     * @see slotLoginPasswdResponse
     */
    void loginPasswd();

    /**
     * @brief Initiates a login using APOP.
     *
     * The response will be received by slotLoginApopResponse().
     * @see slotLoginApopResponse()
     */
    void loginApop();

    /**
     * @brief Removes the status indicator from a single line server message.
     * @param message server message
     * @return server message without status indicator
     */
    QString removeStatusIndicator( const QString& message );

    /**
     * @brief If the first line of the server response is the status indicator (+OK or -ERR), this methode will
     * remove this line.
     */
    void removeStatusIndicator( QStringList* response );

    /**
     * @brief If the last line of the server response is the end of response marker,
     * this methode will remove this line.
     */
    void removeEndOfResponseMarker( QStringList* response );

    /**
     * @brief Sends the UIDL command to get a list of the mail UID's
     *
     * The response will be received by the slotUIDListResponse.
     * @see slotUIDListResponse
     */
    void getUIDList();

    /**
     * @brief Copies the pointer to the temporary list to the mail list.
     *
     * The temporary list will be the current mail list.
     */
    void swapMailLists();

    /**
     * @brief Sends the LIST command to get a list of mail sizes.
     *
     * The response will be received by slotMailListResponse.
     * @see slotMailSizesResponse
     */
    void getMailSizes();

    /**
     * @brief Gets the headers of the new mails from the server.
     */
    void getHeaders();

    /**
     * @brief Get the header of the first mail in newMails.
     *
     * After a successful download this
     * mail will be removed from the list by slotHeaderDownloaded() and this
     * method will be invoked again.
     * If the list is empty, it will call copyHeaders() to get the known
     * headers from the old mail list.
     * @see copyHeaders()
     * @see slotHeaderDownloaded()
     */
    virtual void getNextHeader();

    /**
     * @brief Returns whether the given string is a negative server response.
     * @param response server response to check
     * @return TRUE - response is negative
     * @return FALSE - response is positive
     */
    bool isNegativeResponse( const QString& response );

    /**
     * @brief Copies the known headers of the old mails from the old mail list to the new one.
     *
     * At the end it invokes swapMailLists().
     */
    void copyHeaders();

    /**
     * @brief Deletes the first mail of MailsToDelete.
     *
     * After a successful deletion this mail will be removed from the list
     * by slotMailDeleted() and this method will be invoked again.
     * If the list is empty, it will call commitDelete.
     * @see slotMailDeleted()
     * @see MailsToDelete
     */
    void deleteNextMail();

    /**
     * @brief Shows the body of the first mail in MailsToShow.
     *
     * After a successful download and opening of the window this
     * mail will be removed from the list by slotBodyDownloaded() and this
     * method will be invoked again.
     * If the list is empty, it will call commitDownloading().
     * @see commitDownloading()
     * @see slotBodyDownloaded()
     */
    void showNextMail();

    /**
     * @brief Does all filter actions for which we have to download the mails
     *
     * These are:
     * Moving, spam check
     * It doesn't removes they from the server.
     */
    void doDownloadActions();

    /**
     * @brief Gets the body of the first mail in MailsToDownload.
     *
     * After a successful download and writing into the mailbox this
     * mail will be removed from the list by slotMailDownloadedforDownloadActions() and this
     * method will be invoked again.
     * If the list is empty, it will call applyFilters() to continue the filtering.
     * @see applyFilters()
     * @see slotMailDownloadedforDownloadActions()
     */
    void getNextMailForDownloadActions();

    /**
     * @brief If there are mails to delete by filters applyFilters will call the regular deletion cycle of this class with set byFilter-flag.
     *
     * Therefore the deletion will not branch to commitDeletion() but to this methode.
     * This performs a second refresh cycle to get an effective mail list from the server.
     * This new cycle will not apply the filters again, because the flag filterApplied is set to TRUE.
     * @see applyFilters()
     * @see filterApplied
     */
    void applyFiltersDeleted();

    /**
     * @brief Send a commit and restart the refresh cycle
     *
     * We restart the refresh to get a fresh maillist after a deletion performed by the filter.
     * We need a commit before because the mails server only after a commit reorders the mail numbers
     */
    void commitBeforeRefresh();

    /**
     * @brief Applies the filters to the mails in the mail list.
     *
     * step of the refresh cycle.
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
     * @brief Writes the given mail into the mailbox.
     * @param mail the Mail
     * @param box path to the mailbox
     * @return TRUE - Writing was successful
     */
    bool writeToMailBox( const QStringList& mail, const QString& box );

    /**
     * @brief Returns whether the given directory is a maildir directory
     * @param path the directory for test
     * @return TRUE - directory is a maildir directory
     */
    bool isMailDir( const QDir& path );

    /**
     * @brief Forwards the given mail to SpamAssassin and returns the test result
     * @param mail the mail to test
     * @return TRUE - mail is spam
     */
    bool isSpam( QStringList mail ) const;

    /**
     * @brief Looks for a running spamd daemon of SpamAssassin.
     * @return TRUE - SpamAssassin is running
     */
    bool isSpamAssassinRunning() const;

    /**
     * @brief Invites the server to crypt using TLS (STARTTLS)
     */
    void startTLS();


  protected slots:

   /**
    * @brief Connected with signal connected of the socket.
    * Will be invoked after the socket has connected the host
    */
   virtual void slotConnected();

   /**
    * @brief Connected with signal hostFound of the socket.
    * Will be invoked after the socket has done the DNS lookup
    */
   virtual void slotHostFound();

   /**
    * @brief Connected with signal error of the socket.
    * Will be invoked, when an socket error occures.
    * @param errorCode Error code of the socket
    */
   void slotSocketError( KTcpSocket::Error errorCode );

   /**
    * @brief Connected with signale sslErrors of the socket<p>
    * Will be invoked, when a ssl socket error occures.
    * @param errors list of errors
    */
   void slotSSLError( const QList<KSslError>& errors );

    /**
     * @brief Reads the first message send by server.
     */
    void slotReadFirstServerMessage();

    /**
     * @brief Reads the capabilities from the server.
     * @see getCapabilities()
     */
    void slotCapabilitiesResponse();

    /**
     * @brief Connected with the signal readyRead of the socket by getAuthMech().
     * Analyzes the response and sets the authentication mechanism flags
     * of this account.
     * @see getAuthMech
     */
    void slotAuthMechResponse();

    /**
     * @brief Connected with signal readyRead of the socket by commit().
     * Analyzes the commit response of the server.
     * @see ccommit()
     */
    void slotCommitResponse();

    /**
     * @brief Received the response of the USER command during login.
     * @see loginUser()
     */
    void slotLoginUserResponse();

    /**
     * @brief Received the response of the PASS command during login.
     * @see loginPasswd()
     */
    void slotLoginPasswdResponse();

    /**
     * @brief Receives the response of a login using APOP.
     * @see loginApop()
     */
    void slotLoginApopResponse();

    /**
     * @brief Reiceves the response of the UIDL command, which gets the UID's of the mails.
     * @see getUIDList()
     */
    void slotUIDListResponse();

    /**
     * @brief Receives the response of the LIST command, which gets the
     * numbers and sizes of the mails
     * @see getMailSizes()
     */
    void slotMailSizesResponse();

    /**
     * @brief Receives the header requested by getNextHeader().
     *
     * Stores the received headers into the appropriate instance of the mail list.
     * Removes the first mail from newMails and invokes
     * getNextHeader() again to get the next header.
     * If the list is empty after it has removed the first item, it will call
     * copyHeaders().
     */
    void slotGetHeaderResponse();

    /**
     * @brief Removes the deleted mail from the internal mail list (m_pshowrecord).
     *
     * Removes the first mail from MailsToDelete and invokes
     * deleteNextMail() again to delete the next mail.
     * If the list is empty after it has removed the first item, it will call
     * commitDelete().
     * @see deleteNextMail()
     * @see MailsToDelete
     */
    void slotMailDeleted();

    /**
     * @brief Opens a window (class ShowMailDialog) with the downloaded mail body.
     *
     * Removes the first mail from MailsToShow and invokes showNextMail()
     * again to show the next mail.
     * If the list is empty after it has removed the first item, it will call
     * commitDownloading().
     * @see showNextMail()
     * @see MailsToShow
     * @see ShowMailDialog
     */
    void slotBodyDownloaded();

    /**
     * @brief Downloads a mail to write it into a mailbox or do a spam check.
     *
     * Maybe it puts the number of the downloaded mail into the list of mails to delete (mailsToDelete).
     * Removes the first mail from MailsToMove and invokes getNextMailForMove()
     * again to get and write the next mail.
     * If the list is empty after it has removed the first item, it will call
     * applyFilters() to continue the filtering.
     * If an error is occurred the current mail will not put into MailsToDelete. And it returns
     * to applyFilters() immediately.
     * @see MailsToDownload
     * @see getNextMailForDownloadActions()
     * @see applyFilters()
     */
    void slotMailDownloadedForAction();

    /**
     * @brief Restarts a second refresh cycle after a deletion performed by the filter.
     * @see commitBeforeRefresh
     * @see applyFiltersDeleted
     */
    void slotCommitBeforeRefreshDone();

     /**
      * @brief Connected with the timeout timer.
      *
      * Closes the server connect and finishes the task
      * @see timeoutTimer
      */
     void slotTimeout();

     /**
      * @brief Receives the response of the STLS command
      */
     void slotStartTLSResponse();

  private:

    /**
     * @brief Name of this account
     */
    QString name;

    /**
     * @brief TRUE - account is active; FALSE - account is not active
     */
    bool active;

    /**
     * @brief Type of the transfer security
     */
    TransferSecurity_Type transferSecurity;

    /**
     * @brief Uniform Resource Locator of the account on the mail server.
     */
    KUrl url;

    /**
     * @brief Type of password storage.
     *
     * valid values are: CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET
     * The Constants are defined in constants.h
     * @see getPasswordStorage()
     * @see setPasswordStorage()
     */
    int passwordStorage;

    /**
     * @brief The mail container
     */
    MailList* mails;

    /**
     * @brief While refreshing, this mail list instance will be used to build a new mail list.
     * After that this pointer will be assigned
     * to maillist.
     */
    MailList* tempMailList;


    /**
     * @brief The socket. Handles all server operations
     */
    QPointer<KTcpSocket> socket;

    /**
     * @brief Pointer to the account list object
     */
    AccountList* accountList;

    /**
     * @brief TRUE - the QUIT command was sent to the server. Normally set by the commit methode.
     *
     * Inititates by initBeforeConnect()
     * @see initBeforeConnect()
     */
    bool quitSent;

    /**
     * @brief This timer kills a running operation after the preset time
     */
    QTimer* timeoutTimer;

    /**
     * @brief State of the account.
     */
    Types::AccountState_Type state;

    /**
     * @brief APOP available
     */
    bool apopAvail;

    /**
     * @brief Is set to TRUE by slotLoginAPOPResponse if the first login with APOP is failed.
     *
     * After a setup load it will set to false.
     */
    bool dontUseAPOP;

    /**
     * @brief Timestamp for APOP sent with the server greeting
     */
    QString apopTimestamp;

    /**
     * @brief TRUE - unsafe login is allowed if a secure login failed
     */
    bool allowUnsecureLogin;

   /**
     * @brief This is a flag whether the filters was applied during a refresh cycle.
     *
     * If some mails was deleted (recommend by the filter), we need a actual
     * list of mails from the server. Therefore a second refresh cycle will be started.
     * To avoid a never-ending loop this flag will be set to TRUE by applyFilters().
     * If the second refresh is running this one will not invoke the filters.
    */
    bool filterApplied;

    /**
     * @brief This flag is set by applyFiltersDeleted() to indicate the refresh cycle is the second one after a deletion which was performed by filters.
     *
     * The refresh cycle need it to hold the remaining mails as new.
     */
    bool refreshPerformedByFilters;

    /**
     * @brief Contains the numbers of new mails.
     *
     * Set by getHeaders() and used by getNextHeader() to download
     * the headers of the new mails.
     * @see getHeaders()
     * @see getNextHeader()
     */
    MailNumberList_Type newMails;

   /**
     * @brief Contains the numbers of mails which will be deleted.
     *
     * Set by addMailToDelete() and used by deleteNextMail() to get
     * the number of the next mail to delete.
     * @see addMailToDelete()
     * @see deleteNextMail()
    */
    MailNumberList_Type mailsToDelete;

    /**
     * @brief Contains the numbers of the mails shall be moved for actions like moving to mailbox or spam check.
     *
     * Contains also some parameters (like mailbox) about the action which have to be performed.
     * Set and used by applyFilters().
     * @see applyFilters()
     */
    MailToDownloadMap_Type mailsToDownload;

    /**
     * @brief Contains the numbers of mails for which the bodies will be shown.
     *
     * Set by showSelectedMails() and used by showNextMail() to get
     * the number of the next mail to show.
     * @see showSelectedMails()
     * @see showNextMail()
     */
    MailNumberList_Type mailsToShow;

    /**
     * @brief Pointer to the filter log.
     *
     * Will be set by refreshMailList() at every refresh.
     */
    FilterLog* fLog;

    /**
     * @brief Number of deleted mails by last refresh.
     */
    int nmbDeletedMailsLastRefresh;

    /**
     * @brief Number of moved mails by last refresh.
     */
    int nmbMovedMailsLastRefresh;

    /**
     * @brief Number of moved mails since start.
     */
    int nmbMovedMailsLastStart;

    /**
     * @brief Number of ignored mails by last refresh.
     */
    int nmbIgnoredMails;

    /**
     * @brief Counter for moving to create an unique file name
     */
    int moveCounter;

    /**
     * @brief Number of deleted mails since start.
     */
    int nmbDeletedMailsLastStart;

    /**
     * @brief This flag is set to TRUE by applyFilters() to indicate the mailbodies are downloading and writing to the mailboxes or spam checking.
     *
     * When this is ended, applyFilters() will be called again and it reset this flag
     */
    bool downloadActionsInvoked;

    /**
     * @brief Every account has is own header filter instance.
     */
    HeaderFilter headerFilter;

    /**
     * @brief This flag is set by applyFilters() to indicate the deletion cycle is performed by filters and not by user.
     *
     * The deletion methodes need it to decide on branch targets
     */
    bool deletionPerformedByFilters;

    /**
     * @brief Mailbox for Spam, if configured
     */
     QString spamMailbox;

    /**
     * @brief Action which shall be done, if the mail is Spam
     */
		FilterAction_Type spamAction;

    /**
     * @brief TRUE - the error will not be handled by slotSocketError.
     */
    bool dontHandleError;

    /**
     * @brief TRUE - informs the user about connection errors
     *
     * FALSE - doesn't inform
     */
    bool informAboutErrors;

    /**
     * @brief Seconds to Timeout
     */
    int timeOutTime;

    /**
     * @brief TRUE - the server supports StartTLS.
     *
     * Set by slotCapabilitiesResponse()
     * @see slotCapabilitiesResponse
     */
    bool supportsStartTLS;


  signals:

    /**
     * @brief Will be emitted when all selected mails are deleted.
     * @param account name of the account, which has emitted this signal
     */
    void sigDeleteReady( QString account );

    /**
     * @brief Will be emitted when the bodies of all selected mails are
     * downloaded and shown.
     * @param account name of the account, which has emitted this signal
     */
    void sigShowBodiesReady( QString account );

    /**
     * @brief Will be emitted when the settings have been changed.
     */
    void sigConfigChanged();

    /**
     * @brief Will be emitted, when a window to show a message was opened.
     */
    void sigMessageWindowOpened();

    /**
     * @brief Will be emitted, when a window to show a message was closed.
     */
    void sigMessageWindowClosed();

    /**
     * @brief Will be emitted, when the mail list was refreshed.
     * @param account name of the account, which has emitted this signal
     */
    void sigRefreshReady( QString account );



};

#endif // ACCOUNT_H
