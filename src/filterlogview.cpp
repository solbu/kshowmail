//
// C++ Implementation: filterlogview
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterlogview.h"

FilterLogView::FilterLogView( QWidget *parent, FilterLog* log )
  : KDialog( parent )
{
  //save the log pointer
  this->log = log;

  //set caption
  setCaption( i18n( "Filter Log View" ) );

  //main widget
  QWidget* pgMain = new QWidget( this );
  setMainWidget( pgMain );

  //basic layout
  QVBoxLayout* layMain = new QVBoxLayout();
  pgMain->setLayout( layMain );

  //view of deleted views
  QLabel* lblDeletedMails = new QLabel( i18n( "Deleted Mails:"), pgMain );
  layMain->addWidget( lblDeletedMails );

  lstViewDeleted = new QTreeWidget( pgMain );
  lstViewDeleted->setColumnCount( 4 );

  QStringList headersDeleted;
  headersDeleted.append( i18n( "Date" ) );
  headersDeleted.append( i18n( "Sender" ) );
  headersDeleted.append( i18n( "Account" ) );
  headersDeleted.append( i18n( "Subject" ) );
  lstViewDeleted->setHeaderLabels( headersDeleted );

  lstViewDeleted->setIndentation( 0 );
  lstViewDeleted->setAllColumnsShowFocus( true );
  
  lstViewDeleted->sortItems( 0, Qt::AscendingOrder );
  layMain->addWidget( lstViewDeleted );

  KPushButton* btnClearDeleted = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearDeleted->setToolTip( i18n( "Clear the list of deleted mails" ) );
  connect( btnClearDeleted, SIGNAL( clicked() ), this, SLOT( slotClearDeletedMails() ) );
  btnClearDeleted->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearDeleted );

  //create a seperator
  layMain->addWidget( new KSeparator( pgMain ) );

  //view of deleted views
  QLabel* lblMovedMails = new QLabel( i18n( "Moved Mails:"), pgMain );
  layMain->addWidget( lblMovedMails );

  lstViewMoved = new QTreeWidget( pgMain );
  lstViewMoved->setColumnCount( 5 );

  QStringList headersMoved;
  headersMoved.append( i18n( "Date" ) );
  headersMoved.append( i18n( "Sender" ) );
  headersMoved.append( i18n( "Account" ) );
  headersMoved.append( i18n( "Moved To" ) );
  headersMoved.append( i18n( "Subject" ) );
  lstViewMoved->setHeaderLabels( headersMoved );
  lstViewMoved->setIndentation( 0 );
  lstViewMoved->setAllColumnsShowFocus( true );
  layMain->addWidget( lstViewMoved );

  KPushButton* btnClearMoved = new KPushButton( KStandardGuiItem::clear(), pgMain );
  btnClearMoved->setToolTip( i18n( "Clear the list of moved mails" ) );
  connect( btnClearMoved, SIGNAL( clicked() ), this, SLOT( slotClearMovedMails() ) );
  btnClearMoved->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layMain->addWidget( btnClearMoved );

  //now we load the content for the list views
  LogEntryList listDeletedMails = log->getDeletedMails();
  LogEntryList::iterator it;
  for( it = listDeletedMails.begin(); it != listDeletedMails.end(); it++ )
  {
    //create a new list view item
    FilterLogViewDeletedItem* item = new FilterLogViewDeletedItem( lstViewDeleted );
    item->setValues( (*it).getDate(), (*it).getSender(), (*it).getAccount(), (*it).getSubject() );
  }

  LogEntryList listMovedMails = log->getMovedMails();
  for( it = listMovedMails.begin(); it != listMovedMails.end(); it++ )
  {
    //create a new list view item
    FilterLogViewMovedItem* item = new FilterLogViewMovedItem( lstViewMoved );
    item->setValues( (*it).getDate(), (*it).getSender(), (*it).getAccount(), (*it).getMailbox(), (*it).getSubject() );
  }

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
