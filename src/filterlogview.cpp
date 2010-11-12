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

  modelDeleted = new FilterLogViewDeletedModel( this, log );
  viewDeleted = new QTreeView( pgMain );
  viewDeleted->setModel( modelDeleted );
  viewDeleted->setIndentation( 0 );
  viewDeleted->setSortingEnabled( true );
  layMain->addWidget( viewDeleted );

  KPushButton* btnClearDeleted = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearDeleted->setToolTip( i18nc( "@info:tooltip", "Clear the list of deleted mails" ) );
  connect( btnClearDeleted, SIGNAL( clicked() ), this, SLOT( slotClearDeletedMails() ) );
  btnClearDeleted->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearDeleted );

  //create a separator
  layMain->addWidget( new KSeparator( pgMain ) );

  //view of moved views
  QLabel* lblMovedMails = new QLabel( i18nc( "@label:listbox list of moved mails (written into mailboxes)", "Moved Mails:"), pgMain );
  layMain->addWidget( lblMovedMails );

  modelMoved = new FilterLogViewMovedModel( this, log );
  viewMoved = new QTreeView( pgMain );
  viewMoved->setModel( modelMoved );
  viewMoved->setIndentation( 0 );
  viewMoved->setSortingEnabled( true );
  layMain->addWidget( viewMoved );

  KPushButton* btnClearMoved = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearMoved->setToolTip( i18nc( "@info:tooltip", "Clear the list of moved mails" ) );
  connect( btnClearMoved, SIGNAL( clicked() ), this, SLOT( slotClearMovedMails() ) );
  btnClearMoved->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearMoved );


  //restore Dialog Size
  KConfigGroup configWindowSettings( KGlobal::config(), CONFIG_GROUP_LOGVIEW );
  restoreDialogSize( configWindowSettings );

  //load setup
  loadSetup();
}


FilterLogView::~FilterLogView()
{
}

void FilterLogView::slotClearDeletedMails( )
{
  log->clearDeletedMailsLog();
  modelDeleted->refresh();

}

void FilterLogView::slotClearMovedMails( )
{
  log->clearMovedMailsLog();
  modelMoved->refresh();
}

void FilterLogView::slotButtonClicked( int button )
{
  //save dialog size
  KConfigGroup configWindowSettings( KGlobal::config(), CONFIG_GROUP_LOGVIEW );
  saveDialogSize( configWindowSettings );

  //save setup
  saveSetup();

  KDialog::slotButtonClicked( button );
}

void FilterLogView::saveSetup()
{
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_LOGVIEW );

  //save column widthes
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_BY, viewDeleted->columnWidth( 0 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_DATE, viewDeleted->columnWidth( 1 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_FROM, viewDeleted->columnWidth( 2 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_ACCOUNT, viewDeleted->columnWidth( 3 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_SUBJECT, viewDeleted->columnWidth( 4 ) );

  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_DATE, viewMoved->columnWidth( 0 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_FROM, viewMoved->columnWidth( 1 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_ACCOUNT, viewMoved->columnWidth( 2 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_MAILBOX, viewMoved->columnWidth( 3 ) );
  conf->writeEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_SUBJECT, viewMoved->columnWidth( 4 ) );

  conf->sync();

  //save the model setup
  modelDeleted->saveSetup();
  modelMoved->saveSetup();

}

void FilterLogView::loadSetup()
{
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_LOGVIEW );

  //load column widthes
  viewDeleted->setColumnWidth( 0, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_BY, DEFAULT_WIDTH_LOGVIEW_DELETED_BY ) );
  viewDeleted->setColumnWidth( 1, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_DATE, DEFAULT_WIDTH_LOGVIEW_DELETED_DATE ) );
  viewDeleted->setColumnWidth( 2, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_FROM, DEFAULT_WIDTH_LOGVIEW_DELETED_FROM ) );
  viewDeleted->setColumnWidth( 3, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_ACCOUNT, DEFAULT_WIDTH_LOGVIEW_DELETED_ACCOUNT ) );
  viewDeleted->setColumnWidth( 4, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_SUBJECT, DEFAULT_WIDTH_LOGVIEW_DELETED_SUBJECT ) );

  viewMoved->setColumnWidth( 0, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_DATE, DEFAULT_WIDTH_LOGVIEW_MOVED_DATE ) );
  viewMoved->setColumnWidth( 1, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_FROM, DEFAULT_WIDTH_LOGVIEW_MOVED_FROM ) );
  viewMoved->setColumnWidth( 2, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_ACCOUNT, DEFAULT_WIDTH_LOGVIEW_MOVED_ACCOUNT ) );
  viewMoved->setColumnWidth( 3, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_MAILBOX, DEFAULT_WIDTH_LOGVIEW_MOVED_MAILBOX ) );
  viewMoved->setColumnWidth( 4, conf->readEntry( CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_SUBJECT, DEFAULT_WIDTH_LOGVIEW_MOVED_SUBJECT ) );

  //load sorting
  KConfigGroup* confSort = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

  QString strSortOrderDel = confSort->readEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_DELETED, DEFAULT_SORT_ORDER );
  int sortColumnDel = confSort->readEntry( CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_DELETED, DEFAULT_SORT_COLUMN_LOGVIEW_DELETED );
  if( strSortOrderDel == CONFIG_VALUE_SORT_ORDER_DESCENDING ) {

    viewDeleted->sortByColumn( sortColumnDel, Qt::DescendingOrder );

  } else {

    viewDeleted->sortByColumn( sortColumnDel, Qt::AscendingOrder );
  }

  QString strSortOderMov = confSort->readEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_MOVED, DEFAULT_SORT_ORDER );
  int sortColumnMov = confSort->readEntry( CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_MOVED, DEFAULT_SORT_COLUMN_LOGVIEW_MOVED );
  if( strSortOderMov == CONFIG_VALUE_SORT_ORDER_DESCENDING ) {

    viewMoved->sortByColumn( sortColumnMov, Qt::DescendingOrder );

  } else {

    viewMoved->sortByColumn( sortColumnMov, Qt::AscendingOrder );
  }

}


#include "filterlogview.moc"
