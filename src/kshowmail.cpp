/*
 * kshowmail.cpp
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */

#include "kshowmail.h"

KShowmail::KShowmail() : KXmlGuiWindow()
{
	//set central widget
	view = new KShowmailView( this );
	setCentralWidget( view );

    // add a status bar
    statusBar()->show();

    //initialize the actions
    initActions();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    setupGUI();
}

KShowmail::~KShowmail()
{
}

void KShowmail::initActions()
{
    // file menu
  KStandardAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

  // action menu
  actionRefresh = actionCollection()->addAction( "refresh" );
  actionRefresh->setText( i18n( "Refresh messages" ) );
  actionRefresh->setIcon( KIcon( "view-refresh" ) );
  actionRefresh->setShortcut( Qt::Key_F5 );
  connect( actionRefresh, SIGNAL( triggered() ), this, SLOT( slotRefresh() ) );

  KAction* actionShowHeader = actionCollection()->addAction( "show_header" );
  actionShowHeader->setText( i18n( "Show header of highlighted messages" ) );
  actionShowHeader->setIcon( KIcon( "mail-mark-unread" ) );
  actionShowHeader->setShortcut( Qt::Key_F2 );
  connect( actionShowHeader, SIGNAL( triggered() ), this, SLOT( slotShowHeader() ) );

  KAction* actionShowMessage = actionCollection()->addAction( "show_complete" );
  actionShowMessage->setText( i18n( "Show complete highlighted messages" ) );
  actionShowMessage->setIcon( KIcon( "mail-mark-read" ) );
  actionShowMessage->setShortcut( Qt::Key_F3 );
  connect( actionShowMessage, SIGNAL( triggered() ), this, SLOT( slotShowMessage() ) );
  
  KAction* actionDelete = actionCollection()->addAction( "delete" );
  actionDelete->setText( i18n( "Delete highlighted messages" ) );
  actionDelete->setIcon( KIcon( "draw-eraser" ) );
  actionDelete->setShortcut( Qt::Key_Delete );
  connect( actionDelete, SIGNAL( triggered() ), this, SLOT( slotDelete() ) );

  KAction* actionStop = actionCollection()->addAction( "stop" );
  actionStop->setText( i18n( "Stop current transfer" ) );
  actionStop->setIcon( KIcon( "dialog-cancel" ) );
  connect( actionStop, SIGNAL( triggered() ), this, SLOT( slotStop() ) );

  KAction* actionShowFilterLog = actionCollection()->addAction( "show_filterlog" );
  actionShowFilterLog->setText( i18n( "Show Filter Log" ) );
  actionShowFilterLog->setIcon( KIcon( "text-x-log" ) );
  actionShowFilterLog->setShortcut( Qt::Key_F4 );
  connect( actionShowFilterLog, SIGNAL( triggered() ), this, SLOT( slotShowFilterLog() ) );

  KAction* actionAddWhitelist = actionCollection()->addAction( "addWhitelist" );
  actionAddWhitelist->setText( i18n( "Add sender to whitelist" ) );
  actionAddWhitelist->setIcon( KIcon( "list-add-user" ) );
  connect( actionAddWhitelist, SIGNAL( triggered() ), this, SLOT( slotAddToWhitelist() ) );
  
  KAction* actionAddBlacklist = actionCollection()->addAction( "addBlacklist" );
  actionAddBlacklist->setText( i18n( "Add sender to blacklist" ) );
  actionAddBlacklist->setIcon( KIcon( "list-remove-user" ) );
  connect( actionAddBlacklist, SIGNAL( triggered() ), this, SLOT( slotAddToBlacklist() ) );

  
  // setup menu
  KStandardAction::preferences( this, SLOT( slotSetup() ), actionCollection() );

  //help menu
  KAction* actionSendFeedback = actionCollection()->addAction( "sendFeedback" );
  actionSendFeedback->setText( i18n( "Send Feedback Mail" ) );
  actionSendFeedback->setIcon( KIcon( "mail-flag" ) );
  connect( actionSendFeedback, SIGNAL( triggered() ), this, SLOT( slotSendFeedbackMail() ) );
  
}

void KShowmail::slotRefresh() {
  kDebug() << "slotRefresh" << endl;
}

void KShowmail::slotShowHeader() {
  kDebug() << "slotShowHeader" << endl;
}

void KShowmail::slotShowMessage() {
  kDebug() << "slotShowMessage" << endl;
}

void KShowmail::slotDelete() {
  kDebug() << "slotDelete" << endl;
}

void KShowmail::slotStop() {
  kDebug() << "slotStop" << endl;
}

void KShowmail::slotShowFilterLog() {
  kDebug() << "slotShowFilterLog" << endl;
}

void KShowmail::slotAddToBlacklist() {
  kDebug() << "slotAddToBlacklist" << endl;
}

void KShowmail::slotAddToWhitelist() {
  kDebug() << "slotAddToWhitelist" << endl;
}

void KShowmail::slotSetup() {
  kDebug() << "slotSetup" << endl;
}

void KShowmail::slotSendFeedbackMail() {
  kDebug() << "slotSendFeedbackMail" << endl;
}

void KShowmail::slotFileQuit() {
  kapp->closeAllWindows();
}

bool KShowmail::queryClose() {
  kDebug() << "queryClose" << endl;
  return true;
}

#include "kshowmail.moc"
