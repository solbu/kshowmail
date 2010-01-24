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