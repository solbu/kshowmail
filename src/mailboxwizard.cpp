//
// C++ Implementation: mailboxwizard
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mailboxwizard.h"

MailBoxWizard::MailBoxWizard( QWidget* parent )
 : QWizard( parent )
{

  //this is page one
  //in this the user can chosse his mail directory
  //----------------------------------------------
  QWizardPage* page1 = new QWizardPage();
  page1->setTitle( i18n( "Please choose the path to the mailboxes." ) );
  page1->setSubTitle( i18n( "KShowmail supports only MailDir boxes." ) );
  QHBoxLayout* layMain1 = new QHBoxLayout();
  page1->setLayout( layMain1 );

  txtMailDir = new KLineEdit( page1 );
  layMain1->addWidget( txtMailDir );

  btnMailDir = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Press to choose the mail directory" ), i18n( "Press to choose the mail directory" ) ), page1 );
  btnMailDir->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  layMain1->addWidget( btnMailDir );
  connect( btnMailDir, SIGNAL( clicked() ), this, SLOT( slotOpenDirDialog() ) );

  addPage( page1 );


  //this is page two
  //in this the user can choose the mailbox
  //---------------------------------------
  QWizardPage* page2 = new QWizardPage();
  page2->setTitle( i18n( "Please choose the mailbox" ) );
  QHBoxLayout* layMain2 = new QHBoxLayout();
  page2->setLayout( layMain2 );

  lstMailboxes = new QTreeWidget( page2 );
  lstMailboxes->setColumnCount( 1 );
  lstMailboxes->setHeaderLabel( "Mailbox" );
  lstMailboxes->setIndentation( 0 );
  layMain2->addWidget( lstMailboxes );

  addPage( page2 );

  connect( this, SIGNAL( currentIdChanged(int) ), this, SLOT( slotPageChanged( int ) ) );
}


MailBoxWizard::~MailBoxWizard()
{
}

void MailBoxWizard::slotOpenDirDialog( )
{
  //save old path
  QString oldPath = txtMailDir->text();

  //get new path
  QString path = KFileDialog::getExistingDirectory( KUrl::fromPathOrUrl( oldPath ), this, i18n( "Choose the mailbox directory") );

  //put new or old path in the edit line
  if( path == QString::null )
    txtMailDir->setText( oldPath );
  else
    txtMailDir->setText( path );
}

void MailBoxWizard::slotPageChanged( int pageID )
{
  kdDebug() << pageID << endl;
  //just we looking for mailboxes if the page 2 was opened
  if( pageID == 1 )
  {
    //clear all entries
    lstMailboxes->clear();

    //create an directory object to browse the given directory
    QDir mailDir( txtMailDir->text() );
    if( mailDir.isReadable() )  //is the dir readable?
    {
      //get a list of all entries in this directory
      const QStringList entries = mailDir.entryList( QDir::Dirs | QDir::Readable | QDir::Writable | QDir::Hidden, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware );

      for( QStringList::const_iterator it = entries.begin(); it != entries.end(); ++it )
      {
        kdDebug() << *it << endl;
        //add an entry to the mailbox list
        QDir newMailDir( mailDir );
        newMailDir.cd( (*it) );
        if( (*it) != ".." && (*it) != "." && isMailDir( newMailDir ) )
          addMailBoxListItem( *it, mailDir );
      }
    }
  }

}

bool MailBoxWizard::isMailDir( const QDir & path )
{
  //get a list of all subdirectories in this directory
  const QStringList entries = path.entryList( QDir::Dirs | QDir::Readable | QDir::Writable | QDir::Hidden, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware );

  //a maildir folder must contains the folders "cur", "new" and "tmp"
  bool curFound = false;
  bool newFound = false;
  bool tmpFound = false;

  //iterate over all directories and look for the three necessary dirs
  QStringList::const_iterator it = entries.begin();
  while( it != entries.end() && !( curFound && newFound && tmpFound ) )
  {
    if( *it == "tmp" )
      tmpFound = true;
    else if( *it == "cur" )
      curFound = true;
    else if( *it == "new" )
      newFound = true;

    ++it;
  }

  return curFound && newFound && tmpFound;
}

void MailBoxWizard::addMailBoxListItem( QString boxname, QDir path )
{
  //translate some default mailboxes
  QString boxnameTrans;
  if( boxname.toLower() == "inbox" )
    boxnameTrans = i18n( "Inbox" );
  else if( boxname.toLower() == "outbox" )
    boxnameTrans = i18n( "Outbox" );
  else if( boxname.toLower() == "drafts" )
    boxnameTrans = i18n( "Drafts" );
  else if( boxname.toLower() == "sent-mail" )
    boxnameTrans = i18n( "sent-mail" );
  else if( boxname.toLower() == "trash" )
    boxnameTrans = i18n( "Trash" );
  else
    boxnameTrans = boxname;

  //create item
  MailBoxWizardListItem* newItem;
  newItem = new MailBoxWizardListItem( lstMailboxes, boxnameTrans, path.absolutePath() + "/" + boxname + "/" );


}

QString MailBoxWizard::getPath( )
{
  MailBoxWizardListItem* item = (MailBoxWizardListItem*)lstMailboxes->selectedItems().first();

  QString path = QString::null;
  if( item != NULL )
    path = item->getPath();

  return path;
}

#include "mailboxwizard.moc"
