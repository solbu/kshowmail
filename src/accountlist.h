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

#ifndef ACCOUNTLIST_H
#define ACCOUNTLIST_H

//Qt headers
#include <QObject>
#include <QList>
#include <qitemselectionmodel.h>

//KDE headers
#include <KGlobal>
#include <KConfigGroup>
#include <KDebug>
#include <KConfig>

//KShowmail headers
#include "account.h"
#include "constants.h"
#include "types.h"
#include "corruptdataexception.h"
#include "filterlog.h"

using namespace Types;

class Account;

class AccountList : public QObject
{
	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param parent parent object
		 */
		AccountList( QObject* parent );
		
		/**
		 * Destructor
		 */
		~AccountList();
		
		/**
		 * Creates an account and append it to the list
		 * @param name account name
     * @return pointer to the added account
		 */
		Account* addAccount( const QString& name );
		
		/**
		 * Prints the accounts to stdout
		 */
		void print() const;
		
		/**
		 * Returns the number of accounts
		 * @return number of accounts
		 */
		int numberAccounts() const;

    /**
     * Returns a pointer to the account object at the given index position
     * @param index the index
     * @return pointer to the requested account
     */
    Account* getAccount( int index ) const;

    /**
     * Returns whether the list contains a account with the given name
     * @param accoutName name of the wanted account
     * @return TRUE - the account is in the list
     * @return FALSE - the account is not in the list
     */
    bool hasAccount( QString accountName ) const;

    /**
     * Loads the setup
     */
    void loadSetup();

    /**
     * Orders the accounts to refresh the mails.
     * When the refresh is done, the signal refreshReady will be emitted.
     * @param log pointer to the filter log
     */
    void refreshMailLists( FilterLog* log );

    /**
     * Returns whether all new mails will keep new status until the app is closed.
     * @return TRUE - mails keep new status at next refresh
     * @return FALSE - mails don't keep new status at next refresh
     */
    bool keepNew();

    /**
     * Returns the number of mails.
     * @return number of mails
     */
    int getNumberMails() const;

    /**
     * Returns the mail with the given number.<p>
     * The number is not the number which is given by the mail server.
     * It is the number in order of storage in the account list. For example:<p>
     * Account GMX contains 4 mails.<p>
     * Account Freenet contains 3 mails.<p>
     * Account Yahoo contains 5 mails.<p>
     * (the accounts are stored in the account list in this order)<p>
     * getMail( 0 ) returns the first mail of account GMX.<p>
     * getMail( 5 ) returns the second mail of account Freenet.<p>
     * getMail( 11 ) returns the last mail of account Yahoo.<p>
     * getMail( 20 ) throws an exception.<p>
     * This methode is used by the mail view model.
     * @param number number of the desired mail
     * @return pointer to the found mail
     * @see MailViewModel
     * @throws CorruptDataException
     */
    Mail* getMail( int number ) const throw ( CorruptDataException );

    /**
     * Returns the selected mails.
     * @param mailSelectModel selection model of the mail view
     * @return selected mails
     */
    QList<Mail*> getSelectedMails( QItemSelectionModel* mailSelectModel ) const;

    /**
     * Returns the subjects of the selected mails.
     * @param mailSelectModel selection model of the mail view
     * @return subjects
     */
    QStringList getSelectedSubjects( QItemSelectionModel* mailSelectModel ) const;

    /**
     * Deletes all selected mails from the servers and the mail lists.
     * This just starts the deletion and returns after then. When all
     * accounts are ready the signal sigDeleteReady will be emitted.
     */
    void deleteSelectedMails( QItemSelectionModel* mailSelectModel );

    /**
      * Returns the number of deleted mails by last refresh.
      * @return number of deleted mails by last refresh
      */
    int numberDeletedMailsLastRefresh();

    /**
      * Returns the number of deleted mails since start.
      * @return number of deleted mails since start
      */
    int numberDeletedMailsStart();

    /**
      * Returns the number of moved mails by last refresh.
      * @return number of moved mails by last refresh
      */
    int numberMovedMailsLastRefresh();

    /**
      * Returns the number of moved mails since start.
      * @return number of moved mails since start
      */
    int numberMovedMailsStart();

    /**
      * Returns the number of ignored mails by last refresh.
      * @return number of ignored mails by last refresh
      */
    int numberIgnoredMails();

    /**
     * Reloads the filter settings
     */
    void refreshFilterSetup();



	private:
		
		/**
		 * this list contains the account objects
		 */
		QList<Account*> accounts;

    /**
     * This map is used by the methods to refresh the account mail lists.
     * refreshMailLists() clears it and after that inserts for every account
     * an item. The Key ist the account name and the data is TRUE.
     * When slotCheckRefreshState() is invoked by a signal sent by an account,
     * this slot will set the appropriate item data to FALSE. If the data of all
     * items are set to FALSE, the method will know all accounts have refreshed their
     * mail list and will emit sigRefreshReady.
     * @see refreshMailLists()
     * @see slotCheckRefreshState()
     */
    AccountTaskMap_Type accountRefreshMap;

    /**
     * This map is used by the delete methods.
     * deleteSelectedMails() clears it and after that inserts for every account
     * an item. The Key is the account name and the data is TRUE.
     * When slotCheckDeletionState() is invoked by a signal sent by an account,
     * this slot will set the appropriate item data to FALSE. If the data of all
     * items are set to FALSE, the method will know all accounts have ended the
     * deletion and will emit sigDeleteReady.
     * @see deleteSelectedMails()
     * @see slotCheckDeletionState()
     */
    AccountTaskMap_Type accountDeletionMap;

    /**
     * Number of windows, which have been opened by the accounts to show mails.
     * Used by slotMessageWindowOpened() and slotMessageWindowClosed().
     * @see slotMessageWindowOpened
     * @see slotMessageWindowClosed
     */
    int ctrOpenMessageWindows;

    /**
     * TRUE - mails keep new until termination
     */
    bool keepMailsNew;
    

  protected:

    /**
     * initializes this
     */
    void init();


  protected slots:

    /**
     * Connected with signal sigRefreshReady of all accounts.
     * When an account has sent this signal its appropriate item
     * in AccountRefreshMap will set to FALSE.
     * When all accounts have refreshed their mail list it will emit
     * signal sigRefreshReady.
     * @param account name of the account which has emitted the signal
     * @see AccountRefreshMap
     */
    void slotCheckRefreshState( QString account );

    /**
     * Connected with signal sigMessageWindowOpened of all accounts.
     * When an account has sent this signal the counter ctrOpenMessageWindows
     * will be incremented.
     * When the counter was incremented from zero (the first window was opened)
     * the signal sigMessageWindowOpened will be emitted.
     * @see ctrOpenMessageWindows
     * @see sigMessageWindowOpened
     */
    void slotMessageWindowOpened();

    /**
     * Connected with signal sigMessageWindowClosed of all accounts.
     * When an account has sent this signal the counter ctrOpenMessageWindows
     * will be decremented.
     * When the counter has reached zero the signal sigAllMessageWindowsClosed
     * will be emitted.
     * @see ctrOpenMessageWindows
     * @see sigAllMessageWindowsClosed
     */
    void slotMessageWindowClosed();


    /**
     * Connected with signal sigDeleteReady of all accounts.
     * When an account has sent this signal its appropriate item
     * in AccountDeletionMap will set to FALSE.
     * When all accounts have done the deletion it will emit signal
     * sigDeleteReady.
     * @param account name of the account which has emitted the signal
     * @see AccountDeletionMap
     */
    void slotCheckDeletionState( QString account );


  signals:

    /**
     * Will be emitted, when all accounts have refreshed their mail list.
     */
    void sigRefreshReady();

    /**
     * Will be emitted by slotMessageWindowOpened when an account has
     * opened a window to show a mail.
     * @see slotMessageWindowOpened
     */
    void sigMessageWindowOpened();

    /**
     * Will be emitted by slotMessageWindowClosed when all accounts have
     * closed their windows to show mails.
     * @see slotMessageWindowClosed
     */
    void sigAllMessageWindowsClosed();

    /**
     * Will be emitted when all selected mails are deleted.
     */
    void sigDeleteReady();

    /**
     * Will be emitted when all selected mails are shown.
     */
    void sigShowBodiesReady();

    
};

#endif // ACCOUNTLIST_H
