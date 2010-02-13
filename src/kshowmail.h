/*
 * kshowmail.h
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */
#ifndef KSHOWMAIL_H
#define KSHOWMAIL_H

class KShowmailView;
class MailViewModel;

//Qt headers
#include <QListIterator>


//KDE headers
#include <kstatusbar.h>
#include <KDE/KLocale>
#include <kxmlguiwindow.h>
#include <kaction.h>
#include <KStandardAction>
#include <KActionCollection>
#include <KDebug>
#include <KApplication>
#include <KCMultiDialog>
#include <KMessageBox>

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
		 * account list
		 */
		AccountList* accounts;
		
    /**
     * The setup dialog. Created and used by slotSetup().
     */
    KCMultiDialog* setupDialog;

    /**
     * The application config object
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
     * The model of the mail view
     */
    MailViewModel* mailModel;

    /**
     * The system tray icon
     */
    SystemTrayIcon* trayIcon;

};

#endif // _KSHOWMAIL_H_
