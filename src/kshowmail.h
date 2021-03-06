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

#ifndef KSHOWMAIL_H
#define KSHOWMAIL_H

class KShowmailView;
class MailViewModel;

//Qt headers
#include <QListIterator>
#include <QSortFilterProxyModel>
#include <QList>
#include <QStringList>
#include <QPointer>

//KDE headers
#include <kstatusbar.h>
#include <KLocale>
#include <kxmlguiwindow.h>
#include <kaction.h>
#include <KStandardAction>
#include <KActionCollection>
#include <KDebug>
#include <KApplication>
#include <KCMultiDialog>
#include <KMessageBox>
#include <KSystemTrayIcon>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <KProcess>
#include <kconfigdialog.h>
#include <kconfigskeleton.h>

//KShowmail headers
#include "kshowmailview.h"
#include "accountlist.h"
#include "accountviewmodel.h"
#include "mailviewmodel.h"
#include "mail.h"
#include "types.h"
#include "filterlogview.h"
#include "showheaderdialog.h"
#include "showmaildialog.h"
#include "systemtrayicon.h"
#include "accountsetupdialogcontext.h"
#include "newmaildialog.h"

using namespace Types;

/**
 * This class serves as the main window for kshowmail.
 * For my lovely Stephie!
 * @short main class
 * @author Eggert Ehmke
 * @author Ulrich Weigelt
 */
class KShowmail : public KXmlGuiWindow
{
    Q_OBJECT

  public:

    /**
     * Default Constructor
     */
    KShowmail();

    /**
     * Default Destructor
     */
    virtual ~KShowmail();

  protected:

    /**
     * sets up the actions.
     */
    void initActions();

    /**
     * Shows a message in the status bar
     * @param text text which shall be shown
     */
    void showStatusMessage( const QString& text );

    /**
     * Called before the main window is closed.
     * Reimplemented methode of KMainWindow.
     */
    virtual bool queryClose();

    /**
     * Initializes the statusbar
     */
    void initStatusBar();

    /**
     * Refreshes the status bar filter field.
     */
    void refreshFilterStatusBar();

     /**
      * The filter log.
      */
     FilterLog fLog;

    /**
     * Starts the refresh timer.
     * @param initiate TRUE - this is the first automatic refresh (use initiate time)
     */
    void startAutomaticRefresh( bool initiate = false );

    /**
     * Stops the refresh timer.
     */
    void stopAutomaticRefresh();

    /**
     * Handles all actions for new mails
     */
    void handleNewMails();

    /**
     * Handles all actions when no new mails are arrived
     */
    void handleNoNewMails();

    /**
     * Asks the user to confirm the closing
     * @return TRUE - close; FALSE don't close
     */
    bool askCloseConfirmation();


  protected slots:

    /**
     * Connected with m_actionRefresh.
     * Refreshes the mail list.
     */
    void slotRefresh();

    /**
     * Shows the headers of all selected mails.
     */
    void slotShowHeader();

    /**
     * Connected with action m_actionShowMessage.
     * Shows all selected mails.
     */
    void slotShowMessage();

    /**
     * Connected with action m_actionDelete.
     * Deletes all selected mails.
     */
    void slotDelete();

    /**
     * If the application state is not "idle" it will stop all current running POP3 jobs.
     * Connected with m_actionStop().
     */
    void slotStop();

    /**
     * Shows the filter log.
     */
    void slotShowFilterLog();

    /**
     * Adds the sender of the selected mails to the whitelist
     */
    void slotAddToWhitelist();

    /**
     * Adds the sender of the selected mails to the blacklist
     */
    void slotAddToBlacklist();

    /**
     * Connected with the standard setup action.
     * If the application state is "idle" (m_state == idle) it will open the configuration dialog.
     */
    void slotSetup();

    /**
     * Opens the feedback dialog
     */
    void slotSendFeedbackMail();

   /**
     * Calls the close() method of KShowmail to exit the application.
     */
    void slotFileQuit();

    /**
     * Connected with the setup dialog.
     * Reloads the configuration, if it was changed from the dialog.
     */
    void slotConfChanged();

    /**
     * Connected with signal sigRefreshReady of the account list.
     * This signal will be emitted when all accounts have refreshed
     * their mail list.
     */
    void slotRefreshReady();

    /**
     * Connected with signal sigMessageWindowOpened of m_ConfigList.
     * This signal will be emitted when a window to show a mail body
     * was opened.
     * Switches the cursor to normal view.
     */
    void slotNormalCursor();

    /**
     * Connected with signal sigAllMessageWindowsClosed of m_ConfigList.
     * This signal will be emitted when all windows to show a mail body
     * have been closed.
     * Switches the cursor to waiting view if the application state is not "idle"
     */
    void slotWaitingCursor();

    /**
     * Connected with signal sigDeleteReady of account list.
     * This signal will be emitted when one or many accounts have
     * ended a deletion.
     * This slot refreshes the view, sets the state to idle and sets
     * a normal cursor.
     */
    void slotDeletionReady();

    /**
     * Connected with signal sigShowBodiesReady of account list.
     * This signal will be emitted when all accounts have downloaded
     * and shown the selected messages.
     * This slot sets the state to idle and sets a normal cursor.
     */
    void slotShowMessageReady();

    /**
     * This will be called by the refresh timer every second.<p>
     * It decreases timeToRefresh and calls slotRefresh when it is zero.
     */
    void slotRefreshTimer();

    /**
     * Connected with action actionSetupAccount<p>
     * shows the setup dialog for the selected account
     * @see actionSetupAccount
     */
    void slotSetupAccount();

    /**
     * Connected with newMailDlg, signal cancelClicked<p>
     * Shows the main window
     */
    void slotShowMainWindow();

    /**
     * Connected with Signal sidMailDoubleClicked of the mail list view.
     * Calls slotShowMessage() or slotShowHeader according with the configuration
     */
    void slotMailDoubleClicked();

	private:

		/**
		 * This view is the central widget of KShowmail
		 */
		KShowmailView* view;

    /**
     * refresh messages action; connected with slotRefresh()
     */
    KAction* actionRefresh;

    /**
     * Show header action; connected with slotShowHeader()
     * @see slotShowHeader
     */
    KAction* actionShowHeader;

    /**
     * Show mail action; connected with slotShowMessage()
     * @see slotShowMessage
     */
    KAction* actionShowMessage;

    /**
     * delete mail action; connected with slotDelete()
     * @see slotDelete
     */
    KAction* actionDelete;

    /**
     * stop action; connected with slotStop().
     * @see slotStop
     */
    KAction* actionStop;

    /**
     * Show filter log action; connected with slotShowFilterLog()
     * @see slotShowFilterLog
     */
    KAction* actionShowFilterLog;

    /**
     * Add to white list action, connected with slotAddToWhitelist()
     * @see slotAddToWhitelist
     */
    KAction* actionAddWhitelist;

    /**
     * Add to black list action; connected with slotAddToBlacklist()
     * @see slotAddToBlacklist
     */
    KAction* actionAddBlacklist;

    /**
     * setup account action; connected with slotSetupAccount()
     * @see slotSetupAccount
     */
    KAction* actionSetupAccount;

		/**
		 * account list
		 */
		AccountList* accounts;

    /**
     * The setup dialog. Created and used by slotSetup().
     */
    KCMultiDialog* setupDialog;

    /**
     * application config
     */
    KSharedConfigPtr config;

    /**
     * config group of general preferences
     */
    KConfigGroup* configGeneral;

    /**
     * Working state
     */
    State_Type state;

    /**
     * The selection model of the mail view
     */
    QItemSelectionModel* mailSelectModel;

    /**
     * The selection model of the account view
     */
    QItemSelectionModel* accountSelectModel;

    /**
     * The model of the mail view
     */
    MailViewModel* mailModel;

		/**
		 * The model of the account view
		 */
		AccountViewModel* accountModel;

    /**
     * The system tray icon
     */
    SystemTrayIcon* trayIcon;

    /**
     * The refresh timer. It times out every second and calls slotRefreshTimer().
     */
    QTimer* refreshTimer;

    /**
     * Number of seconds until the next refresh
     */
    unsigned int timeToRefresh;

    /**
     * This is to play a sound for new mails
     * @see handleNewMails
     */
    Phonon::MediaObject *mediaObject;

    /**
     * This is to play a sound for new mails
     * @see handleNewMails
     */
    Phonon::AudioOutput *audioOutput;

    /**
     * The new mail dialog
     * @see handleNewMails
     */
    NewMailDialog* newMailDlg;

    /**
     * This is to avoid a confirm close dialog
     */
    bool forceExit;

};

#endif // _KSHOWMAIL_H_
