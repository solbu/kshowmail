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



KShowmailView::KShowmailView( QWidget* parent ) : QSplitter( Qt::Vertical, parent )
{
	//create account list
//	viewAccounts = new QTableWidget( this );
//	viewAccounts->setSelectionMode( QAbstractItemView::ExtendedSelection );
//   viewAccounts->setSorting( 1 ); // sort account column
//   viewAccounts->setShowSortIndicator( true );
//   viewAccounts->addColumn( i18n( "Active" ), DEFAULT_WIDTH_ACCOUNT_ACTIVE );
//   viewAccounts->addColumn( i18n( "Account" ), DEFAULT_WIDTH_ACCOUNT_ACCOUNT );
//   viewAccounts->addColumn( i18n( "Server" ), DEFAULT_WIDTH_ACCOUNT_SERVER );
//   viewAccounts->addColumn( i18n( "User" ), DEFAULT_WIDTH_ACCOUNT_USER );
//   viewAccounts->addColumn( i18n( "Messages" ), DEFAULT_WIDTH_ACCOUNT_MESSAGES );
//   viewAccounts->addColumn( i18n( "Size" ), DEFAULT_WIDTH_ACCOUNT_SIZE );
//   viewAccounts->setAllColumnsShowFocus( true );

	//create mail list
//	viewMails = new QTableView( this );
}

KShowmailView::~KShowmailView(){}