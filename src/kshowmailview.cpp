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

#include "./kshowmailview.h"


KShowmailView::KShowmailView( AccountViewModel* accountModel, MailViewModel* mailModel, QItemSelectionModel* mailSelectModel, QWidget* parent ) : QSplitter( Qt::Vertical, parent )
{
	
	//Split the view into two parts
	QSplitter* splitter = new QSplitter( Qt::Vertical, this );

	//create account view
	viewAccounts = new QTreeView( splitter );
	viewAccounts->setModel( accountModel );
  viewAccounts->setIndentation( 0 );
  viewAccounts->setContextMenuPolicy( Qt::ActionsContextMenu );
	AccountViewDelegate* delegateAccounts = new AccountViewDelegate( this );
	viewAccounts->setItemDelegate( delegateAccounts );
  
	//create mail view
	viewMails = new QTreeView( splitter );
	viewMails->setModel( mailModel );
  viewMails->setIndentation( 0 );
  viewMails->setSelectionMode( QAbstractItemView::ExtendedSelection );
  viewMails->setSelectionModel( mailSelectModel );
	
	loadSetup();

}

KShowmailView::~KShowmailView(){}

void KShowmailView::refreshViews( QItemSelectionModel* mailSelectModel, QList<int> markedMails )
{
  AccountViewModel* accountModel = dynamic_cast<AccountViewModel*>( viewAccounts->model() );
  accountModel->refresh();

  MailViewModel* mailModel = dynamic_cast<MailViewModel*>( viewMails->model() );
  mailModel->refresh();

  //mark filtered mails
  QListIterator<int> itMark( markedMails );
  while( itMark.hasNext() )
  {
    int row = itMark.next();
    mailSelectModel->select( mailModel->index( row, 0 ), QItemSelectionModel::Select | QItemSelectionModel::Rows );
  }
}

void KShowmailView::saveSetup() {

	//get config objects
	KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNT_LIST );
	KConfigGroup* configMail = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_MESSAGE_LIST );
	
	//save the column widths
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACTIVE, viewAccounts->columnWidth( 0 ) );
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_ACCOUNT, viewAccounts->columnWidth( 1 ) );
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SERVER, viewAccounts->columnWidth( 2 ) );
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_USER, viewAccounts->columnWidth( 3 ) );
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_MESSAGES, viewAccounts->columnWidth( 4 ) );
	configAcc->writeEntry( CONFIG_ENTRY_WIDTH_ACCOUNT_SIZE, viewAccounts->columnWidth( 5 ) );
	
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_STATE, viewMails->columnWidth( 0 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_NUMBER, viewMails->columnWidth( 1 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_ACCOUNT, viewMails->columnWidth( 2 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_FROM, viewMails->columnWidth( 3 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_TO, viewMails->columnWidth( 4 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SUBJECT, viewMails->columnWidth( 5 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_DATE, viewMails->columnWidth( 6 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_SIZE, viewMails->columnWidth( 7 ) );
	configMail->writeEntry( CONFIG_ENTRY_WIDTH_MESSAGE_CONTENT, viewMails->columnWidth( 8 ) );
	
	
	configAcc->sync();
	configMail->sync();
	
}

void KShowmailView::loadSetup() {

		//get config object for the account list
	KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNT_LIST );
	KConfigGroup* configMail = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_MESSAGE_LIST );
	
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
	
	
}