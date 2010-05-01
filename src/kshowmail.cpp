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

#include "kshowmail.h"

KShowmail::KShowmail() : KXmlGuiWindow()
{
  //init some flags
  forceExit = false;
  
	//create the account list
	accounts = new AccountList( this );
  connect( accounts, SIGNAL( sigMessageWindowOpened() ), this, SLOT( slotNormalCursor() ) );
  connect( accounts, SIGNAL( sigAllMessageWindowsClosed() ), this, SLOT( slotWaitingCursor() ) );
  connect( accounts, SIGNAL( sigRefreshReady() ), this, SLOT( slotRefreshReady() ) );
  connect( accounts, SIGNAL( sigDeleteReady() ), this, SLOT( slotDeletionReady() ) );
  connect( accounts, SIGNAL( sigShowBodiesReady() ), this, SLOT( slotShowMessageReady() ) );
	
	
	//create the models for the account view and mail view
	accountModel = new AccountViewModel( accounts, this );
  connect( accountModel, SIGNAL( dataChanged(QModelIndex,QModelIndex) ), this, SLOT( slotConfChanged() ) );
	mailModel = new MailViewModel( accounts, this );

  //create the selection models
  mailSelectModel = new QItemSelectionModel( mailModel );
  accountSelectModel = new QItemSelectionModel( accountModel );
	
	//set central widget
	view = new KShowmailView( accountModel, mailModel, mailSelectModel, accountSelectModel, this );
	setCentralWidget( view );

  // add a status bar
  initStatusBar();

  //initialize the actions
  initActions();

  //set actions for context menus
  view->addActionToAccountList( actionRefresh );
  view->addActionToAccountList( actionSetupAccount );
  view->addActionToMailList( actionShowHeader );
  view->addActionToMailList( actionShowMessage );
  view->addActionToMailList( actionDelete );
  view->addActionToMailList( actionAddWhitelist );
  view->addActionToMailList( actionAddBlacklist );
 

  // a call to KXmlGuiWindow::setupGUI() populates the GUI
  // with actions, using KXMLGUI.
  // It also applies the saved mainwindow settings, if any, and ask the
  // mainwindow to automatically save settings if changed: window size,
  // toolbar position, icon size, etc.
  setupGUI();

  //get the config objects
  config = KGlobal::config();
  configGeneral = new KConfigGroup( config, CONFIG_GROUP_GENERAL );

  //create system tray icon
  trayIcon = new SystemTrayIcon( this, QIcon( KStandardDirs::locate( "data", "kshowmail/pics/trayIcon.png" ) ), actionRefresh );
  trayIcon->show();
		
	//load the setup
	accounts->loadSetup();
  view->loadSetup();

  //refresh the view
  view->refreshViews( mailSelectModel );

	//read stored mails
	//accounts->readStoredMails();

  //this is to play the new mail sound
  mediaObject = new Phonon::MediaObject( this );
  audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory, this );
  Phonon::createPath( mediaObject, audioOutput );

  //the new mail dialog
  newMailDlg = new NewMailDialog( this );
  connect( newMailDlg, SIGNAL( cancelClicked() ), this, SLOT( slotShowMainWindow() ) );

  //refresh timer
  refreshTimer = new QTimer( this );
  connect( refreshTimer, SIGNAL( timeout() ), this, SLOT( slotRefreshTimer() ) );

  //at beginning the state is "idle"
  state = idle;

  //run initial refresh
  startAutomaticRefresh( true );


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
  actionRefresh->setText( i18nc( "@action:inmenu look for new messages", "Refresh messages" ) );
  actionRefresh->setIcon( KIcon( "view-refresh" ) );
  actionRefresh->setShortcut( Qt::Key_F5 );
  connect( actionRefresh, SIGNAL( triggered() ), this, SLOT( slotRefresh() ) );

  actionShowHeader = actionCollection()->addAction( "show_header" );
  actionShowHeader->setText( i18nc( "@action:inmenu show the headers of the selected mails", "Show header of highlighted messages" ) );
  actionShowHeader->setIcon( KIcon( "mail-mark-unread" ) );
  actionShowHeader->setShortcut( Qt::Key_F2 );
  connect( actionShowHeader, SIGNAL( triggered() ), this, SLOT( slotShowHeader() ) );

  actionShowMessage = actionCollection()->addAction( "show_complete" );
  actionShowMessage->setText( i18nc( "@action:inmenu show the body of the selected mails", "Show complete highlighted messages" ) );
  actionShowMessage->setIcon( KIcon( "mail-mark-read" ) );
  actionShowMessage->setShortcut( Qt::Key_F3 );
  connect( actionShowMessage, SIGNAL( triggered() ), this, SLOT( slotShowMessage() ) );
  
  actionDelete = actionCollection()->addAction( "delete" );
  actionDelete->setText( i18nc( "@action:inmenu delete selected mails", "Delete highlighted messages" ) );
  actionDelete->setIcon( KIcon( "draw-eraser" ) );
  actionDelete->setShortcut( Qt::Key_Delete );
  connect( actionDelete, SIGNAL( triggered() ), this, SLOT( slotDelete() ) );

  actionStop = actionCollection()->addAction( "stop" );
  actionStop->setText( i18nc( "@action:inmenu cancle the current running task", "Stop current transfer" ) );
  actionStop->setIcon( KIcon( "dialog-cancel" ) );
  connect( actionStop, SIGNAL( triggered() ), this, SLOT( slotStop() ) );

  actionShowFilterLog = actionCollection()->addAction( "show_filterlog" );
  actionShowFilterLog->setText( i18nc( "@action:inmenu open the filter log", "Show Filter Log" ) );
  actionShowFilterLog->setIcon( KIcon( "text-x-log" ) );
  actionShowFilterLog->setShortcut( Qt::Key_F4 );
  connect( actionShowFilterLog, SIGNAL( triggered() ), this, SLOT( slotShowFilterLog() ) );

  actionAddWhitelist = actionCollection()->addAction( "addWhitelist" );
  actionAddWhitelist->setText( i18nc( "@action:inmenu add the senders of selected mails to the whitelist", "Add sender to whitelist" ) );
  actionAddWhitelist->setIcon( KIcon( "list-add-user" ) );
  connect( actionAddWhitelist, SIGNAL( triggered() ), this, SLOT( slotAddToWhitelist() ) );
  
  actionAddBlacklist = actionCollection()->addAction( "addBlacklist" );
  actionAddBlacklist->setText( i18nc( "@action:inmenu add the senders of selected mails to the blacklist", "Add sender to blacklist" ) );
  actionAddBlacklist->setIcon( KIcon( "list-remove-user" ) );
  connect( actionAddBlacklist, SIGNAL( triggered() ), this, SLOT( slotAddToBlacklist() ) );

  
  // setup menu
  KStandardAction::preferences( this, SLOT( slotSetup() ), actionCollection() );

  //help menu
  KAction* actionSendFeedback = actionCollection()->addAction( "sendFeedback" );
  actionSendFeedback->setText( i18nc( "@action:inmenu open a dialog to send a feedback", "Send Feedback Mail" ) );
  actionSendFeedback->setIcon( KIcon( "mail-flag" ) );
  connect( actionSendFeedback, SIGNAL( triggered() ), this, SLOT( slotSendFeedbackMail() ) );

  //only for account context menu
  actionSetupAccount = actionCollection()->addAction( "setupAccount" );
  actionSetupAccount->setText( i18nc( "@action:inmenu setup the selected account", "Setup this account") );
  actionSetupAccount->setIcon( KIcon( "configure") );
  connect( actionSetupAccount, SIGNAL( triggered() ), this, SLOT( slotSetupAccount() ) );



}

void KShowmail::slotRefresh() {

  //just do it, if the app doesn't do anything
  if ( state != idle )
  {
    return;
  }

  //stop refresh timer
  refreshTimer->stop();

  //set new state
  state = refreshing;

  //show message in the status bar
  showStatusMessage( i18nc( "@info:status application is getting mails from the servers", "Refreshing ..." ) );

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
  QList<Mail> mailList = mailModel->getSelectedMails( mailSelectModel );

  //iterate over all mails
  QListIterator<Mail> itMails( mailList );
  int dialogReturnValue = KDialog::Accepted;
  while( itMails.hasNext() && dialogReturnValue == KDialog::Accepted )
  {
    Mail mail = itMails.next();

    //create and open the window
    QString account( mail.getAccountName() );
    QString subject( mail.getSubject() );
    QPointer<ShowHeaderDialog> dlg = new ShowHeaderDialog( this->centralWidget(), account , subject, mail.getHeader() );
    dialogReturnValue = dlg->exec();

    delete dlg;
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
  showStatusMessage( i18nc( "@info:status application is downloading mails to show", "Downloading ..." ) );

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //get selected Mails
  QList<Mail> mailsToShow = mailModel->getSelectedMails( mailSelectModel );

  //add the mails to the accounts deletion lists
  QListIterator<Mail> itShow( mailsToShow );
  while( itShow.hasNext() ) {

    Mail mail = itShow.next();
    mail.getAccount()->addMailToShow( mail.getNumber() );
  }

  accounts->showMails();


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
    QStringList subjects = mailModel->getSelectedSubjects( mailSelectModel );

    //show question
    int answer = KMessageBox::questionYesNoList( this, i18nc( "@info ask for delete the mails", "Do you want to delete these mails?"), subjects, i18nc( "@title:window ask for delete the mails", "Delete?" ) );

    if( answer == KMessageBox::No )
      return;
  }


  //set the state
  state = deleting;

  //show status message
  showStatusMessage( i18nc( "@info:status application is removing mails from server", "Deleting Mail(s) ..." ) );

  //set waiting cursor
  QApplication::setOverrideCursor( Qt::WaitCursor );

  //get selected Mails
  QList<Mail> mailsToDelete = mailModel->getSelectedMails( mailSelectModel );

  //add the mails to the accounts deletion lists
  QListIterator<Mail> itDel( mailsToDelete );
  while( itDel.hasNext() ) {

    Mail mail = itDel.next();

    QPointer<Account> acc = mail.getAccount();

    int mailNumber = mail.getNumber();

    acc->addMailToDelete( mailNumber );
    
  }

  accounts->deleteMails();

}

void KShowmail::slotStop() {
  accounts->cancelTasks();
}

void KShowmail::slotShowFilterLog() {

  //open dialog
  QPointer<FilterLogView> view = new FilterLogView( this, &fLog );
  view->exec();

  delete view;

  refreshFilterStatusBar();
}

void KShowmail::slotAddToBlacklist() {

  //get selected senders
  QStringList senders = mailModel->getSelectedSenders( mailSelectModel );
  kdDebug() << senders << endl;

  //get previous blacklist from config
  KConfigGroup* config = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_FILTER );
  QStringList blackList = config->readEntry( CONFIG_ENTRY_FILTER_BLACKLIST, QStringList() );

  //append new entries
  blackList.append( senders );

  //write extended whitelist back
  config->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST, blackList );
  config->sync();

  delete config;

  //load new config
  accounts->refreshFilterSetup();

}

void KShowmail::slotAddToWhitelist() {

  //get selected senders
  QStringList senders = mailModel->getSelectedSenders( mailSelectModel );
  kdDebug() << senders << endl;

  //get previous whitelist from config
  KConfigGroup* config = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_FILTER );
  QStringList whiteList = config->readEntry( CONFIG_ENTRY_FILTER_WHITELIST, QStringList() );

  //append new entries
  whiteList.append( senders );

  //write extended whitelist back
  config->writeEntry( CONFIG_ENTRY_FILTER_WHITELIST, whiteList );
  config->sync();

  delete config;

  //load new config
  accounts->refreshFilterSetup();
}

void KShowmail::slotSetup() {

  //save the column widthes
  view->saveSetup();

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

  //ask for confirmation
  if( configGeneral->readEntry( CONFIG_ENTRY_CONFIRM_CLOSE, DEFAULT_CONFIRM_CLOSE ) ) {

    if( askCloseConfirmation() == false )
      return;
  }

  //don't ask for confirmation again in queryClose()
  forceExit = true;

  //close it
  kapp->closeAllWindows();
}

bool KShowmail::queryClose() {

  //save setup
  fLog.save();
  config->sync();
  accounts->saveOptions();
	view->saveSetup();

  //force exit
  if( forceExit ) {
    return true;
  }

  //just hide?
  if( configGeneral->readEntry( CONFIG_ENTRY_CLOSE_TO_TRAY, DEFAULT_CLOSE_TO_TRAY ) ) {

    hide();
    return false;
  }
  
  //confirm closing
  if( configGeneral->readEntry( CONFIG_ENTRY_CONFIRM_CLOSE, DEFAULT_CONFIRM_CLOSE ) ) {

    if( askCloseConfirmation() == true )
      return true;
  }

  return true;
}

void KShowmail::slotConfChanged() {

  accounts->loadSetup();
  fLog.loadSetup();
  view->loadSetup();

  //refresh the views
  view->refreshViews( mailSelectModel );

  //restart refresh timer
  startAutomaticRefresh();


}

void KShowmail::showStatusMessage( const QString& text)
{
  //set given text
  statusBar()->changeItem( text, STATUSBAR_FIELD_STATE );
}

void KShowmail::initStatusBar()
{
  statusBar()->insertPermanentItem( i18nc( "@info:status application is idle", "Ready" ), STATUSBAR_FIELD_STATE );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_FILTER, 1 );
  statusBar()->setItemAlignment( STATUSBAR_FIELD_FILTER, Qt::AlignLeft | Qt::AlignVCenter );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_NEXT_REFRESH );
  statusBar()->insertPermanentItem( "", STATUSBAR_FIELD_LAST_REFRESH );

  statusBar()->setSizeGripEnabled( true );

  statusBar()->setToolTip( i18nc( "@info:tooltip", "Shows the number of deleted, moved or ignored mails by the filter.\nThe positions denotes:\nby last refresh / since application start / listed by the log" ) );
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
  showStatusMessage( i18nc( "@info:status application is idle", "Ready" ) );

  //refresh view
  view->refreshViews( mailSelectModel );

  //refresh filter status bar
  refreshFilterStatusBar();

  //show the number of mails
  trayIcon->drawNumber( accounts->getNumberMails(), accounts->getNumberNewMails() != 0 ? Qt::red : Qt::black );

  //show refresh time
  QString sTime = QTime::currentTime().toString ();
  statusBar()->changeItem( i18nc( "@info:status time of the last refresh", "Last Refresh: %1", sTime ), STATUSBAR_FIELD_LAST_REFRESH );

  //handle new mails actions
  if( accounts->getNumberNewMails() > 0 ) {

    handleNewMails();

  } else {

    handleNoNewMails();
  }

  //start the refresh timer
  startAutomaticRefresh();

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
  showStatusMessage( i18nc( "@info:status application is idle", "Ready." ) );

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

  statusBar()->changeItem( i18nc( "@info:status numbers of filtered mails", "Filter: Deleted: %1/%2/%3; Moved: %4/%5/%6; Ignored: %7", numberDeletedLastRefresh, numberDeletedSinceStart, numberDeletedLog, numberMovedLastRefresh, numberMovedSinceStart, numberMovedLog, numberIgnored ), STATUSBAR_FIELD_FILTER );
}

void KShowmail::slotShowMessageReady( )
{
  //set state to idle
  state = idle;

  //set normal cursor
  while( QApplication::overrideCursor() )
    QApplication::restoreOverrideCursor();

  //show status message
  showStatusMessage( i18nc( "@info:status application is idle", "Ready." ) );


}

void KShowmail::startAutomaticRefresh( bool initiate ) {

  //just start if auto refresh is desired
  if( configGeneral->readEntry( CONFIG_ENTRY_AUTO_REFRESH, DEFAULT_AUTO_REFRESH ) ) {

    //get refresh time
    if( initiate ) {

      timeToRefresh = configGeneral->readEntry( CONFIG_ENTRY_INITIAL_TIME, DEFAULT_INITIAL_TIME );

    } else {

      timeToRefresh = configGeneral->readEntry( CONFIG_ENTRY_INTERVAL_TIME, DEFAULT_INTERVAL_TIME ) * 60;
    }

    //start the timer
    refreshTimer->start( 1000 );

  } else {

    refreshTimer->stop();
  }


}

void KShowmail::slotRefreshTimer() {

  //decrease refresh counter
  timeToRefresh--;

  //show time to next refrsh in statusbar
  QTime time;
  time = time.addSecs( timeToRefresh );

  statusBar()->changeItem( i18nc( "@info:status time to next refresh", "Next Refresh: %1" ).arg( time.toString() ), STATUSBAR_FIELD_NEXT_REFRESH );

  //start refresh at zero
  if( timeToRefresh == 0 ) {

    stopAutomaticRefresh();
    slotRefresh();
  }

}

void KShowmail::stopAutomaticRefresh() {

  refreshTimer->stop();
  timeToRefresh = 0;

}

void KShowmail::slotSetupAccount() {

  if( !accountSelectModel->hasSelection() ) return;

  
  //get account
  QModelIndex selIndex = accountSelectModel->selectedRows().first();
  Account* acc = accountModel->getAccount( selIndex );

  //open setup dialog
  QPointer<AccountSetupDialogContext> dlg = new AccountSetupDialogContext( this, acc->getName() );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialog::Accepted )
    slotConfChanged();

  //delete dialog
  delete dlg;

}

void KShowmail::handleNewMails()
{
  //get config
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACTIONS );
  
  //play sound
  if( conf->readEntry( CONFIG_ENTRY_NEW_MAIL_SOUND, DEFAULT_ACTION_NEW_MAIL_SOUND ) ) {

    //get file
    QString file = conf->readEntry( CONFIG_ENTRY_NEW_MAIL_SOUNDPATH, "" );
    if( file.length() != 0 )
    {
      mediaObject->setCurrentSource( Phonon::MediaSource( file ) );
      mediaObject->play();

    }

  }

  if( conf->readEntry( CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW, DEFAULT_ACTION_NEW_MAIL_ALERTWINDOW) ) {

      newMailDlg->show();
  }

  if( conf->readEntry( CONFIG_ENTRY_NEW_MAIL_MAINWINDOW, DEFAULT_ACTION_NEW_MAIL_MAINWINDOW ) ) {

      slotShowMainWindow();
  }

  if( conf->readEntry( CONFIG_ENTRY_NEW_MAIL_BEEP, DEFAULT_ACTION_NEW_MAIL_BEEP ) ) {

      kapp->beep();
  }

  if( conf->readEntry( CONFIG_ENTRY_NEW_MAIL_COMMAND, DEFAULT_ACTION_NEW_MAIL_COMMAND ) ) {

    //get command
    QString command = conf->readEntry( CONFIG_ENTRY_NEW_MAIL_COMMANDPATH, "" );

    //execute it
    if( command.length() > 0 ) {

      QStringList parts = command.split( ' ', QString::SkipEmptyParts );

      if( !command.isEmpty() )
      {
        KProcess::execute( parts );
      }

    }

  }
  
  delete conf;
  
}

void KShowmail::slotShowMainWindow()
{
  show();
  setWindowState( windowState() & ~Qt::WindowMinimized );
}

void KShowmail::handleNoNewMails()
{

  //get config
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACTIONS );

  if( conf->readEntry( CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE, DEFAULT_ACTION_NO_NEW_MAIL_MINIMIZE ) ) {

    hide();
  }

  if( conf->readEntry( CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE, DEFAULT_ACTION_NO_NEW_MAIL_TERMINATE ) ) {

    forceExit = true;
    kapp->closeAllWindows();
  }
}

bool KShowmail::askCloseConfirmation()
{
  return KMessageBox::questionYesNo( this, i18nc( "@info ask for closing the application", "KShowmail will be closed.\nAre you sure?") ) == KMessageBox::Yes;

}


#include "kshowmail.moc"
