/*
 * kshowmail.h
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */
#ifndef KSHOWMAIL_H
#define KSHOWMAIL_H


//KDE headers
#include <kstatusbar.h>
#include <KDE/KLocale>
#include <kxmlguiwindow.h>
#include <kaction.h>
#include <KStandardAction>
#include <KActionCollection>
#include <KDebug>
#include <KApplication>

//KShowmail headers
#include "kshowmailview.h"
#include "accountlist.h"
#include "accountviewmodel.h"
#include "mailviewmodel.h"
#include "mail.h"

/**
 * This class serves as the main window for kshowmail.
 *
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

    virtual bool queryClose();

  protected slots:

    /**
     * Connected with m_actionRefresh.
     * Refreshes the mail list.
     */
    void slotRefresh();

    /**
     * Shows the headers of all selected mails.
     * Connected with action m_actionShowHeader.
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

};

#endif // _KSHOWMAIL_H_
