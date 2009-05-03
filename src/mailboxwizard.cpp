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

MailBoxWizard::MailBoxWizard( QWidget* parent, const char* name )
 : QWizard( parent, name, true )
{

  //this is page one
  //in this the user can chosse his mail directory
  //----------------------------------------------
  QWidget* page1 = new QWidget( this, "page1" );
  QHBoxLayout* layMain1 = new QHBoxLayout( page1, 0, 10 );

  txtMailDir = new KLineEdit( page1, "txtMailDir" );
  layMain1->addWidget( txtMailDir );

  btnMailDir = new KPushButton( KGuiItem( QString(), QString( "folder" ), i18n( "Press to choose the mail directory" ), i18n( "Press to choose the mail directory" ) ), page1, "btnMailDir" );
  btnMailDir->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  layMain1->addWidget( btnMailDir );
  connect( btnMailDir, SIGNAL( clicked() ), this, SLOT( slotOpenDirDialog() ) );

  title1 = i18n( "Please choose the path to the mailboxes.\nKShowmail supports only MailDir boxes." );
  addPage( page1, title1 );


  //this is page two
  //in this the user can choose the mailbox
  //---------------------------------------
  QWidget* page2 = new QWidget( this, "page2" );
  QHBoxLayout* layMain2 = new QHBoxLayout( page2, 0, 10 );

  lstMailboxes = new KListView( page2, "lstMailboxes" );
  lstMailboxes->addColumn( "Mailbox" );
  lstMailboxes->setRootIsDecorated( true );
  layMain2->addWidget( lstMailboxes );

  title2 = i18n( "Please choose the mailbox" );
  addPage( page2, title2 );
  setFinishEnabled( page2, true );

  connect( this, SIGNAL( selected( const QString& ) ), this, SLOT( slotPageChanged( const QString& ) ) );
}


MailBoxWizard::~MailBoxWizard()
{
}

void MailBoxWizard::slotOpenDirDialog( )
{
  //save old path
  QString oldPath = txtMailDir->text();

  //get new path
  QString path = KFileDialog::getExistingDirectory( oldPath, this, i18n( "Choose the mailbox directory") );

  //put new or old path in the edit line
  if( path == QString::null )
    txtMailDir->setText( oldPath );
  else
    txtMailDir->setText( path );
}

void MailBoxWizard::slotPageChanged( const QString& pageTitle )
{
  //just we looking for mailboxes if the page 2 was opened
  if( pageTitle == title2 )
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
  if( boxname.lower() == "inbox" )
    boxnameTrans = i18n( "Inbox" );
  else if( boxname.lower() == "outbox" )
    boxnameTrans = i18n( "Outbox" );
  else if( boxname.lower() == "drafts" )
    boxnameTrans = i18n( "Drafts" );
  else if( boxname.lower() == "sent-mail" )
    boxnameTrans = i18n( "sent-mail" );
  else if( boxname.lower() == "trash" )
    boxnameTrans = i18n( "Trash" );
  else
    boxnameTrans = boxname;

  //create item
  MailBoxWizardListItem* newItem;
  newItem = new MailBoxWizardListItem( lstMailboxes, boxnameTrans, path.absPath() + "/" + boxname + "/" );


}

QString MailBoxWizard::getPath( )
{
  MailBoxWizardListItem* item = (MailBoxWizardListItem*)lstMailboxes->selectedItem();

  QString path = QString::null;
  if( item != NULL )
    path = item->getPath();

  return path;
}

#include "mailboxwizard.moc"
