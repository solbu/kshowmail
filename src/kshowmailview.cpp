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

#include "./kshowmailview.h"


KShowmailView::KShowmailView( AccountViewModel* accountModel, MailViewModel* mailModel, QItemSelectionModel* mailSelectModel, QItemSelectionModel* accountSelectModel, QWidget* parent ) : QSplitter( Qt::Vertical, parent )
{

	//Split the view into two parts
	splitter = new QSplitter( Qt::Vertical, this );

	//create account view
	viewAccounts = new QTreeView( splitter );
	viewAccounts->setModel( accountModel );
  viewAccounts->setIndentation( 0 );
  viewAccounts->setContextMenuPolicy( Qt::ActionsContextMenu );
	AccountViewDelegate* delegateAccounts = new AccountViewDelegate( this );
	viewAccounts->setItemDelegate( delegateAccounts );
  viewAccounts->setContextMenuPolicy( Qt::ActionsContextMenu );
  viewAccounts->setSelectionModel( accountSelectModel );
  viewAccounts->setSortingEnabled( true );

	//create mail view
	viewMails = new QTreeView( splitter );
	viewMails->setModel( mailModel );
  viewMails->setIndentation( 0 );
  viewMails->setSelectionMode( QAbstractItemView::ExtendedSelection );
  viewMails->setSelectionModel( mailSelectModel );
  viewMails->setSortingEnabled( true );
  viewMails->setContextMenuPolicy( Qt::ActionsContextMenu );

  //a double click on an entry of the mail list calls slotMailDoubleClicked()
  connect( viewMails, SIGNAL( doubleClicked(QModelIndex) ), this, SLOT( slotMailDoubleClicked(QModelIndex) ) );

	//save the pointer to the models
	this->mailModel = mailModel;
	this->accountModel = accountModel;

	loadSetup();


}

KShowmailView::~KShowmailView(){}

void KShowmailView::refreshViews( QItemSelectionModel* mailSelectModel )
{
  AccountViewModel* accountModel = dynamic_cast<AccountViewModel*>( viewAccounts->model() );
  accountModel->refresh();

  MailViewModel* mailModel = dynamic_cast<MailViewModel*>( viewMails->model() );
  mailModel->refresh();

  //mark filtered mails
  QModelIndexList markedMails = mailModel->getMarkedMails();
  QListIterator<QModelIndex> itMark( markedMails );
  while( itMark.hasNext() )
  {
    QModelIndex index = itMark.next();
    mailSelectModel->select( index, QItemSelectionModel::Select | QItemSelectionModel::Rows );
  }
}

void KShowmailView::saveSetup() {

	//get config objects
	KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNT_LIST );
	KConfigGroup* configMail = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_MESSAGE_LIST );
  KConfigGroup* configView = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

	//save the column widths
  if( !viewAccounts->isColumnHidden( 0 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACTIVE, viewAccounts->columnWidth( 0 ) );

  if( !viewAccounts->isColumnHidden( 1 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACCOUNT, viewAccounts->columnWidth( 1 ) );

  if( !viewAccounts->isColumnHidden( 2 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SERVER, viewAccounts->columnWidth( 2 ) );

  if( !viewAccounts->isColumnHidden( 3 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_USER, viewAccounts->columnWidth( 3 ) );

  if( !viewAccounts->isColumnHidden( 4 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_MESSAGES, viewAccounts->columnWidth( 4 ) );

  if( !viewAccounts->isColumnHidden( 5 ) )
    configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SIZE, viewAccounts->columnWidth( 5 ) );


  if( !viewMails->isColumnHidden( 0 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_STATE, viewMails->columnWidth( 0 ) );

  if( !viewMails->isColumnHidden( 1 ) )
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_NUMBER, viewMails->columnWidth( 1 ) );

  if( !viewMails->isColumnHidden( 2 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_ACCOUNT, viewMails->columnWidth( 2 ) );

  if( !viewMails->isColumnHidden( 3 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_FROM, viewMails->columnWidth( 3 ) );

  if( !viewMails->isColumnHidden( 4 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_TO, viewMails->columnWidth( 4 ) );

  if( !viewMails->isColumnHidden( 5 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SUBJECT, viewMails->columnWidth( 5 ) );

  if( !viewMails->isColumnHidden( 6 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_DATE, viewMails->columnWidth( 6 ) );

  if( !viewMails->isColumnHidden( 7 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SIZE, viewMails->columnWidth( 7 ) );

  if( !viewMails->isColumnHidden( 8 ) )
    configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_CONTENT, viewMails->columnWidth( 8 ) );

  //save position of the splitter
  configView->writeEntry( CONFIG_ENTRY_VIEW_MAIN_WINDOW_SPLITTER, splitter->saveState() );

	//save models setup
	accountModel->saveSetup();
	mailModel->saveSetup();

	configAcc->sync();
	configMail->sync();
  configView->sync();

  delete configAcc;
  delete configMail;
  delete configView;
}

void KShowmailView::loadSetup() {

  //get config objects
	KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNT_LIST );
	KConfigGroup* configMail = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_MESSAGE_LIST );
  KConfigGroup* configView = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

	viewAccounts->setColumnWidth( 0, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACTIVE, DEFAULT_WIDTH_ACCOUNT_ACTIVE ) );
	viewAccounts->setColumnWidth( 1, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACCOUNT, DEFAULT_WIDTH_ACCOUNT_ACCOUNT ) );
	viewAccounts->setColumnWidth( 2, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SERVER, DEFAULT_WIDTH_ACCOUNT_SERVER ) );
	viewAccounts->setColumnWidth( 3, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_USER, DEFAULT_WIDTH_ACCOUNT_USER ) );
	viewAccounts->setColumnWidth( 4, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_MESSAGES, DEFAULT_WIDTH_ACCOUNT_MESSAGES ) );
	viewAccounts->setColumnWidth( 5, configAcc->readEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SIZE, DEFAULT_WIDTH_ACCOUNT_SIZE ) );

	viewMails->setColumnWidth( 0, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_STATE, DEFAULT_WIDTH_MESSAGE_STATE ) );
	viewMails->setColumnWidth( 1, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_NUMBER, DEFAULT_WIDTH_MESSAGE_NUMBER ) );
	viewMails->setColumnWidth( 2, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_ACCOUNT, DEFAULT_WIDTH_MESSAGE_ACCOUNT ) );
	viewMails->setColumnWidth( 3, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_FROM, DEFAULT_WIDTH_MESSAGE_FROM ) );
	viewMails->setColumnWidth( 4, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_TO, DEFAULT_WIDTH_MESSAGE_TO ) );
	viewMails->setColumnWidth( 5, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SUBJECT, DEFAULT_WIDTH_MESSAGE_SUBJECT ) );
	viewMails->setColumnWidth( 6, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_DATE, DEFAULT_WIDTH_MESSAGE_DATE ) );
	viewMails->setColumnWidth( 7, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SIZE, DEFAULT_WIDTH_MESSAGE_SIZE ) );
	viewMails->setColumnWidth( 8, configMail->readEntry( CONFIG_ENTRY_WIDTH_MESSAGE_CONTENT, DEFAULT_WIDTH_MESSAGE_CONTENT ) );

  viewAccounts->setColumnHidden( 0 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE, DEFAULT_DISPLAY_ACCOUNT_ACTIVE ) );
  viewAccounts->setColumnHidden( 1 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT, DEFAULT_DISPLAY_ACCOUNT_ACCOUNT ) );
  viewAccounts->setColumnHidden( 2 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER, DEFAULT_DISPLAY_ACCOUNT_SERVER ) );
  viewAccounts->setColumnHidden( 3 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_USER, DEFAULT_DISPLAY_ACCOUNT_USER ) );
  viewAccounts->setColumnHidden( 4 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES, DEFAULT_DISPLAY_ACCOUNT_MESSAGES ) );
  viewAccounts->setColumnHidden( 5 , !configAcc->readEntry( CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE, DEFAULT_DISPLAY_ACCOUNT_SIZE ) );

  viewMails->setColumnHidden( 0 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_STATE, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 1 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 2 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 3 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_FROM, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 4 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_TO, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 5 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 6 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_DATE, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 7 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE, DEFAULT_DISPLAY_MESSAGE_STATE ) );
  viewMails->setColumnHidden( 8 , !configMail->readEntry( CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT, DEFAULT_DISPLAY_MESSAGE_STATE ) );

	//load sorting
	KConfigGroup* confSort = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

	QString strSortOrderAcc = confSort->readEntry( CONFIG_ENTRY_SORT_ORDER_ACCOUNT, DEFAULT_SORT_ORDER );
	int sortColumnAcc = confSort->readEntry( CONFIG_ENTRY_SORT_COLUMN_ACCOUNT, DEFAULT_SORT_COLUMN_ACCOUNT );
	if( strSortOrderAcc == CONFIG_VALUE_SORT_ORDER_DESCENDING ) {

		viewAccounts->sortByColumn( sortColumnAcc, Qt::DescendingOrder );

	} else {

		viewAccounts->sortByColumn( sortColumnAcc, Qt::AscendingOrder );
	}

	QString strSortOderMail = confSort->readEntry( CONFIG_ENTRY_SORT_ORDER_MESSAGE, DEFAULT_SORT_ORDER );
	int sortColumnMail = confSort->readEntry( CONFIG_ENTRY_SORT_COLUMN_MESSAGE, DEFAULT_SORT_COLUMN_MESSAGE );
	if( strSortOderMail == CONFIG_VALUE_SORT_ORDER_DESCENDING ) {

		viewMails->sortByColumn( sortColumnMail, Qt::DescendingOrder );

	} else {

		viewMails->sortByColumn( sortColumnMail, Qt::AscendingOrder );
	}

	//load splitter state
  splitter->restoreState( configView->readEntry( CONFIG_ENTRY_VIEW_MAIN_WINDOW_SPLITTER, QByteArray() ) );

	delete confSort;
	delete configAcc;
	delete configMail;
  delete configView;

  //load the setup of the mail view model
  mailModel->loadSetup();

}

void KShowmailView::addActionToAccountList( KAction* action ) {

  viewAccounts->addAction( action );

}

void KShowmailView::addActionToMailList(KAction* action)
{
  viewMails->addAction( action );
}

void KShowmailView::slotMailDoubleClicked(const QModelIndex& )
{
  emit sigMailDoubleClicked();
}

