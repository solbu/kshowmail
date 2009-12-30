/*
 * kshowmail.cpp
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */

#include "kshowmail.h"

KShowmail::KShowmail() : KXmlGuiWindow()
{
	//create the account list
	accounts = new AccountList( this );
  connect( accounts, SIGNAL( sigMessageWindowOpened() ), this, SLOT( slotNormalCursor() ) );
  connect( accounts, SIGNAL( sigAllMessageWindowsClosed() ), this, SLOT( slotWaitingCursor() ) );
  connect( accounts, SIGNAL( sigRefreshReady() ), this, SLOT( slotRefreshReady() ) );

	
	//create the models for the account view and mail view
	AccountViewModel* accountModel = new AccountViewModel( accounts, this );
	MailViewModel* mailModel = new MailViewModel( accounts, this );
	
	//set central widget
	view = new KShowmailView( accountModel, mailModel, this );
	setCentralWidget( view );

  // add a status bar
  initStatusBar();

  //initialize the actions
  initActions();

  // a call to KXmlGuiWindow::setupGUI() populates the GUI
  // with actions, using KXMLGUI.
  // It also applies the saved mainwindow settings, if any, and ask the
  // mainwindow to automatically save settings if changed: window size,
  // toolbar position, icon size, etc.
  setupGUI();

  //get the application config object
  config = KGlobal::config();
		
	//load the setup
	accounts->loadSetup();

  //at beginning the state is "idle"
  state = idle;
	
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



  //loads the setup
  accounts->loadSetup();
  
}

void KShowmail::slotRefresh() {

  //just do it, if the app doesn't do anything
  if ( state != idle )
  {
    kapp->beep();
    return;
  }

  //set new state
  state = refreshing;

  //show message in the status bar
  showStatusMessage( i18n( "Refreshing ..." ) );

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //order the account list to do the refresh
  accounts->refreshMailLists();
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

//    setupDialog->addModule( "kshowmailconfigdisplay.desktop" );
//    setupDialog->addModule( "kshowmailconfigspamcheck.desktop" );

	//create the dialog and add the pages
	setupDialog = new KCMultiDialog( this );
  //setupDialog->addModule( "kshowmailconfiggeneral.desktop" );
  setupDialog->addModule( "kshowmailconfigaccounts.desktop" );
  setupDialog->addModule( "kshowmailconfigactions.desktop" );
  setupDialog->addModule( "kshowmailconfigdisplay.desktop" );
  setupDialog->addModule( "kshowmailconfigfilter.desktop" );
  setupDialog->addModule( "kshowmailconfiglog.desktop" );

  //If the configuration was changed, it will call slotConfChanged
  connect( setupDialog, SIGNAL( configCommitted() ), this, SLOT( slotConfChanged() ) );

  //execute preferences dialog
  setupDialog->exec();
	
	//clear memory
	delete setupDialog;
	
	
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

void KShowmail::slotConfChanged() {
  accounts->loadSetup();

  //refresh the views
  view->refreshViews();
}

void KShowmail::showStatusMessage( const QString& text)
{
  //get current time
  // added by Gustavo Zamorano to include time
  QString sTime = QTime::currentTime().toString ();

  //set given text
  statusBar()->changeItem( text, STATUSBAR_FIELD_STATE );

  //set current time
  statusBar()->changeItem( i18n( "Last Refresh: %1" ).arg( sTime ), STATUSBAR_FIELD_LAST_REFRESH );
}

void KShowmail::initStatusBar()
{
  statusBar()->insertItem( i18n( "Ready" ), STATUSBAR_FIELD_STATE );
  statusBar()->insertItem( "", STATUSBAR_FIELD_FILTER, 1 );
  statusBar()->setItemAlignment( STATUSBAR_FIELD_FILTER, Qt::AlignLeft | Qt::AlignVCenter );
  statusBar()->insertItem( "", STATUSBAR_FIELD_NEXT_REFRESH );
  statusBar()->insertItem( "", STATUSBAR_FIELD_LAST_REFRESH );

  statusBar()->setToolTip( i18n( "Shows the number of deleted, moved or ignored mails by the filter.\nThe positions denotes:\nby last refresh / since application start / listed by the log" ) );
  statusBar()->show();

}

void KShowmail::slotRefreshReady()
{
  //set state to idle
  state = idle;

  //set normal cursor
  while( QApplication::overrideCursor() )
    QApplication::restoreOverrideCursor();

  //show status message
  showStatusMessage( i18n( "Ready" ) );

  //refresh view
  view->refreshViews();
}

void KShowmail::slotNormalCursor( )
{
  //set normal cursor
  while( QApplication::overrideCursor() )
    QApplication::restoreOverrideCursor();

}

void KShowmail::slotWaitingCursor( )
{
  //set waiting cursor
  if( state != idle )
    QApplication::setOverrideCursor( Qt::WaitCursor );
}



#include "kshowmail.moc"
