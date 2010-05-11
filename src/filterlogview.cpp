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

#include "filterlogview.h"

FilterLogView::FilterLogView( QWidget *parent, FilterLog* log )
  : KDialog( parent )
{
  //save the log pointer
  this->log = log;

  //set caption
  setCaption( i18nc( "@title:window", "Filter Log View" ) );

  //main widget
  QWidget* pgMain = new QWidget( this );
  setMainWidget( pgMain );

  //basic layout
  QVBoxLayout* layMain = new QVBoxLayout();
  pgMain->setLayout( layMain );

  //view of deleted views
  QLabel* lblDeletedMails = new QLabel( i18nc( "@label:listbox list of deleted mails", "Deleted Mails:"), pgMain );
  layMain->addWidget( lblDeletedMails );

  lstViewDeleted = new QTreeWidget( pgMain );
  lstViewDeleted->setColumnCount( 4 );

  QStringList headersDeleted;
  headersDeleted.append( i18nc( "@title:column send date", "Date" ) );
  headersDeleted.append( i18nc( "@title:column sender of the mail", "Sender" ) );
  headersDeleted.append( i18nc( "@title:column account name", "Account" ) );
  headersDeleted.append( i18nc( "@title:column mail subject", "Subject" ) );
  lstViewDeleted->setHeaderLabels( headersDeleted );

  lstViewDeleted->setIndentation( 0 );
  lstViewDeleted->setAllColumnsShowFocus( true );
  
  lstViewDeleted->sortItems( 0, Qt::AscendingOrder );
  layMain->addWidget( lstViewDeleted );

  KPushButton* btnClearDeleted = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearDeleted->setToolTip( i18nc( "@info:tooltip", "Clear the list of deleted mails" ) );
  connect( btnClearDeleted, SIGNAL( clicked() ), this, SLOT( slotClearDeletedMails() ) );
  btnClearDeleted->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearDeleted );

  //create a separator
  layMain->addWidget( new KSeparator( pgMain ) );

  //view of deleted views
  QLabel* lblMovedMails = new QLabel( i18nc( "@label:listbox list of moved mails (written into mailboxes)", "Moved Mails:"), pgMain );
  layMain->addWidget( lblMovedMails );

  lstViewMoved = new QTreeWidget( pgMain );
  lstViewMoved->setColumnCount( 5 );

  QStringList headersMoved;
  headersMoved.append( i18nc( "@title:column send date", "Date" ) );
  headersMoved.append( i18nc( "@title:column sender of the mail", "Sender" ) );
  headersMoved.append( i18nc( "@title:column account name", "Account" ) );
  headersMoved.append( i18nc( "@title:column name of the mailbox where the mail was written", "Moved To" ) );
  headersMoved.append( i18nc( "@title:column mail subject", "Subject" ) );
  lstViewMoved->setHeaderLabels( headersMoved );
  lstViewMoved->setIndentation( 0 );
  lstViewMoved->setAllColumnsShowFocus( true );
  layMain->addWidget( lstViewMoved );

  KPushButton* btnClearMoved = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearMoved->setToolTip( i18nc( "@info:tooltip", "Clear the list of moved mails" ) );
  connect( btnClearMoved, SIGNAL( clicked() ), this, SLOT( slotClearMovedMails() ) );
  btnClearMoved->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearMoved );

  //now we load the content for the list views
  LogEntryList listDeletedMails = log->getDeletedMails();
  LogEntryList::iterator it;
  for( it = listDeletedMails.begin(); it != listDeletedMails.end(); ++it )
  {
    //create a new list view item
    FilterLogViewDeletedItem* item = new FilterLogViewDeletedItem( lstViewDeleted );
    item->setValues( (*it).getDate(), (*it).getSender(), (*it).getAccount(), (*it).getSubject() );
  }

  LogEntryList listMovedMails = log->getMovedMails();
  for( it = listMovedMails.begin(); it != listMovedMails.end(); ++it )
  {
    //create a new list view item
    FilterLogViewMovedItem* item = new FilterLogViewMovedItem( lstViewMoved );
    item->setValues( (*it).getDate(), (*it).getSender(), (*it).getAccount(), (*it).getMailbox(), (*it).getSubject() );
  }

  //set sorting
  lstViewDeleted->setSortingEnabled( true );
  lstViewMoved->setSortingEnabled( true );

  //get application config object (kshowmailrc)
  config = KGlobal::config();

  //restore Dialog Size
  KConfigGroup configWindowSettings( config, CONFIG_GROUP );
  restoreDialogSize( configWindowSettings );
}


FilterLogView::~FilterLogView()
{
}

void FilterLogView::slotClearDeletedMails( )
{
  log->clearDeletedMailsLog();
  lstViewDeleted->clear();

}

void FilterLogView::slotClearMovedMails( )
{
  log->clearMovedMailsLog();
  lstViewMoved->clear();
}

void FilterLogView::slotButtonClicked( int button )
{
  //save dialog size
  KConfigGroup configWindowSettings( config, CONFIG_GROUP );
  saveDialogSize( configWindowSettings );

  KDialog::slotButtonClicked( button );
}

#include "filterlogview.moc"
