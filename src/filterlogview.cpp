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
  KConfigGroup configWindowSettings( config, CONFIG_GROUP );
  saveDialogSize( configWindowSettings );

  KDialog::slotButtonClicked( button );
}

#include "filterlogview.moc"
