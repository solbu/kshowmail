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
  connect( accounts, SIGNAL( sigDeleteReady() ), this, SLOT( slotDeletionReady() ) );
  connect( accounts, SIGNAL( sigShowBodiesReady() ), this, SLOT( slotShowMessageReady() ) );

	
	//create the models for the account view and mail view
	AccountViewModel* accountModel = new AccountViewModel( accounts, this );
	mailModel = new MailViewModel( accounts, this );

  //create the mail selection model
  mailSelectModel = new QItemSelectionModel( mailModel );
	
	//set central widget
	view = new KShowmailView( accountModel, mailModel, mailSelectModel, this );
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
  configGeneral = new KConfigGroup( config, CONFIG_GROUP_GENERAL );

  //create system tray icon
  trayIcon = new SystemTrayIcon( this, QIcon( KStandardDirs::locate( "data", "kshowmail/pics/trayIcon.png" ) ) );
  trayIcon->show();
		
	//load the setup
	accounts->loadSetup();

  //at beginning the state is "idle"
  state = idle;


  //TEST
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

  //order the tray icon to flash
  trayIcon->showLooking();

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //order the account list to do the refresh
  accounts->refreshMailLists( &fLog );
}

void KShowmail::slotShowHeader() {

  //only show headers, if the app is idle
  if( state != idle )
  {
    kapp->beep ();
    return;
  }

  //get selected Mails
  QList<Mail*> mailList = accounts->getSelectedMails( mailSelectModel );

  //iterate over all mails
  QListIterator<Mail*> itMails( mailList );
  int dialogReturnValue = QDialog::Accepted;
  while( itMails.hasNext() && dialogReturnValue == QDialog::Accepted )
  {
    Mail* mail = itMails.next();

    //create and open the window
    QString account( mail->getAccount()->getName() );
    QString subject( mail->getSubject() );
    ShowHeaderDialog dlg( this->centralWidget(), account , subject, mail->getHeader() );
    dialogReturnValue = dlg.exec();
  }
}

void KShowmail::slotShowMessage() {

  //return, if application is not idle
  if( state != idle )
  {
    kapp->beep ();
    return;
  }

  //return, if no mails are selected
  if( !mailSelectModel->hasSelection() )
    return;

  //set the state
  state = showing;

  //show status message
  showStatusMessage( i18n( "Downloading ..." ) );

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //order the account list to show the selected mails
  accounts->showSelectedMails( mailSelectModel );


}

void KShowmail::slotDelete() {

  //just delete, if no operation is running
  if( state != idle )
  {
    kapp->beep();
    return;
  }

  //return, if no mails are selected
  if( !mailSelectModel->hasSelection() )
    return;

  //confirm deletion if required
  bool confirmDeletion = configGeneral->readEntry( CONFIG_ENTRY_CONFIRM_DELETE, DEFAULT_CONFIRM_DELETE );


  
  if( confirmDeletion )
  {
    
    //get subjects off all selected mails
    QStringList subjects = accounts->getSelectedSubjects( mailSelectModel );

    //show question
    int answer = KMessageBox::questionYesNoList( this, i18n( "Do you want to delete these mails?"), subjects, i18n( "Delete?" ) );

    if( answer == KMessageBox::No )
      return;
  }


  //set the state
  state = deleting;

  //show status message
  showStatusMessage( i18n( "Deleting Mail(s) ..." ) );

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //order the account list to delete the selected mails
  accounts->deleteSelectedMails( mailSelectModel );

}

void KShowmail::slotStop() {
  kDebug() << "slotStop" << endl;
}

void KShowmail::slotShowFilterLog() {

  //open dialog
  FilterLogView view( this, &fLog );
  view.exec();

  refreshFilterStatusBar();
}

void KShowmail::slotAddToBlacklist() {
  kDebug() << "slotAddToBlacklist" << endl;
}

void KShowmail::slotAddToWhitelist() {
  kDebug() << "slotAddToWhitelist" << endl;
}

void KShowmail::slotSetup() {


	//create the dialog and add the pages
	setupDialog = new KCMultiDialog( this );
  setupDialog->addModule( "kshowmailconfiggeneral.desktop" );
  setupDialog->addModule( "kshowmailconfigaccounts.desktop" );
  setupDialog->addModule( "kshowmailconfigactions.desktop" );
  setupDialog->addModule( "kshowmailconfigdisplay.desktop" );
  setupDialog->addModule( "kshowmailconfigfilter.desktop" );
  setupDialog->addModule( "kshowmailconfigspamcheck.desktop" );
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

  //Einstellungen speichern
  fLog.save();
  config->sync();
  accounts->saveOptions();

  return true;
}

void KShowmail::slotConfChanged() {

  accounts->loadSetup();
  fLog.loadSetup();

  //refresh the views
  view->refreshViews( mailSelectModel, QList<int>() );

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
  statusBar()->insertPermanentItem( i18n( "Ready" ), STATUSBAR_FIELD_STATE );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_FILTER, 1 );
  statusBar()->setItemAlignment( STATUSBAR_FIELD_FILTER, Qt::AlignLeft | Qt::AlignVCenter );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_NEXT_REFRESH );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_LAST_REFRESH );

  statusBar()->setSizeGripEnabled( true );

  statusBar()->setToolTip( i18n( "Shows the number of deleted, moved or ignored mails by the filter.\nThe positions denotes:\nby last refresh / since application start / listed by the log" ) );
  statusBar()->show();

  refreshFilterStatusBar();

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
  view->refreshViews( mailSelectModel, accounts->getMarkedMails() );

  //refresh filter status bar
  refreshFilterStatusBar();

  //show the number of mails
  trayIcon->drawNumber( accounts->getNumberMails(), accounts->getNumberNewMails() != 0 ? Qt::red : Qt::black );

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

void KShowmail::slotDeletionReady( )
{
  //set state to idle
  state = idle;

  //set normal cursor
  while( QApplication::overrideCursor() )
    QApplication::restoreOverrideCursor();

  //show status message
  showStatusMessage( i18n( "Ready." ) );

  //refresh mail list
  slotRefresh();
}

void KShowmail::refreshFilterStatusBar( )
{
  int numberDeletedLastRefresh = accounts->numberDeletedMailsLastRefresh();
  int numberDeletedSinceStart = accounts->numberDeletedMailsStart();
  int numberDeletedLog = fLog.numberDeletedMails();

  int numberMovedLastRefresh = accounts->numberMovedMailsLastRefresh();
  int numberMovedSinceStart = accounts->numberMovedMailsStart();
  int numberMovedLog = fLog.numberMovedMails();

  int numberIgnored = accounts->numberIgnoredMails();

  statusBar()->changeItem( i18n( "Filter: Deleted: %1/%2/%3; Moved: %4/%5/%6; Ignored: %7" ).arg( numberDeletedLastRefresh ).arg( numberDeletedSinceStart ).arg( numberDeletedLog ).arg( numberMovedLastRefresh ).arg( numberMovedSinceStart ).arg( numberMovedLog ).arg( numberIgnored ), STATUSBAR_FIELD_FILTER );
}

void KShowmail::slotShowMessageReady( )
{
  //set state to idle
  state = idle;

  //set normal cursor
  while( QApplication::overrideCursor() )
    QApplication::restoreOverrideCursor();

  //show status message
  showStatusMessage( i18n( "Ready." ) );


}


#include "kshowmail.moc"
