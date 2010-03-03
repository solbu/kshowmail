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

#include "./accountlist.h"

AccountList::AccountList( QObject* parent ) : QObject( parent )
{
  init();
	
}

AccountList::~AccountList(){}

Account* AccountList::addAccount( const QString& name )
{
	//create account object
	Account* acc = new Account( name, this, this );

  //conect the signals with the slots of this list
  connect( acc, SIGNAL( sigRefreshReady( QString ) ), this, SLOT( slotCheckRefreshState( QString ) ) );
  connect( acc, SIGNAL( sigMessageWindowOpened() ), this, SLOT( slotMessageWindowOpened() ) );
  connect( acc, SIGNAL( sigMessageWindowClosed() ), this, SLOT( slotMessageWindowClosed() ) );
  connect( acc, SIGNAL( sigDeleteReady(QString) ), this, SLOT( slotCheckDeletionState(QString) ) );
  connect( acc, SIGNAL( sigShowBodiesReady(QString) ), this, SLOT( slotCheckShowBodiesState(QString) ) );
	
	//append it to the list
	accounts.append( acc );

  return acc;
}

void AccountList::print() const
{
	//print the accounts
	QListIterator<Account*> iter( accounts );
	while( iter.hasNext() )
	{
		Account* acc = iter.next();
		acc->print();
		cout << "**********************" << endl;
	}
}

int AccountList::numberAccounts() const
{
	return accounts.size();
}

Account* AccountList::getAccount( int index ) const
{
  //check the index
  if( index > accounts.size() ) return NULL;

  //return the account if the index is valid
  return accounts.at( index );
}

void AccountList::loadSetup()
{
  //get account names from the config file
  KConfigGroup* confAccounts = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_ACCOUNTS );
  QStringList accountsConfig = confAccounts->readEntry( CONFIG_ENTRY_ACCOUNTS_LIST, QStringList() );

  //remove deleted accounts from the account list
  //accounts are deleted, if they are in AccountList yet, but not in the list of the config file (accounts)
  QList<Account*>::iterator iter;
  iter = accounts.begin();
  while( iter != accounts.end() )
  {
    Account* accDel = *iter;
    QString accName = accDel->getName();

    if( !accountsConfig.contains( accName ) )
    {
      iter = accounts.erase( iter );
      delete accDel;
    }
    else
    {
      iter++;
    }
  }

  //iterate over all entries in the config account list and create a new account if necessary
  QStringListIterator iterAccountsConfig( accountsConfig );
  while( iterAccountsConfig.hasNext() )
  {
    QString accName = iterAccountsConfig.next();

    if( !hasAccount( accName ) )
    {
      addAccount( accName );
    }
  }
	
  //load the setup of all accounts
  iter = accounts.begin();
  while( iter != accounts.end() )
  {
    Account* acc = *iter;
		
		acc->load();
		
		iter++;
  }

  //read the setup of other things
  keepMailsNew = confAccounts->readEntry( CONFIG_ENTRY_KEEP_NEW, DEFAULT_KEEP_NEW );

  //read filter setup
  refreshFilterSetup();
}

bool AccountList::hasAccount( QString accountName ) const
{
  QListIterator<Account*> iter( accounts );
  while( iter.hasNext() )
  {
    Account* acc = iter.next();
    if( acc->getName() == accountName ) return true;
  }
  return false;
}

void AccountList::refreshMailLists( FilterLog* log )
{
  //return, if no accounts available
  if( accounts.count() == 0 )
  {
    emit sigRefreshReady();
    return;
  }

  //clear the map, which contains the names of the accounts,
  //which have gotten an order to show mails
  accountRefreshMap.clear();

  //inserts an item for every account which will get an order to refresh
  //its mail list. The key is the account name and the data is TRUE.
  //it is important to do this in a seperate iteration because this avoids
  //race conditions
  QList<Account*>::const_iterator iter;
  for( iter = accounts.constBegin(); iter != accounts.constEnd(); iter++ )
  {
    Account* account = *iter;
    
    //insert item
    accountRefreshMap.insert( account->getName(), true );
  }

  //iterate over all accounts and order them to refresh the mail lists
  for( iter = accounts.constBegin(); iter != accounts.constEnd(); iter++ )
  {
    Account* account = *iter;

    account->refreshMailList( log );
  }
}

void AccountList::slotCheckRefreshState( QString account )
{
  bool accountRefreshing = false;    //set to TRUE if an account is still refreshing
  AccountTaskMap_Type::Iterator it;   //iterator over the account map

  //set the appropriate item in AccountRefreshMap to FALSE
  accountRefreshMap[ account ] = false;

  //iterate over the account map to check whether all accounts
  //are ready
  for ( it = accountRefreshMap.begin(); it != accountRefreshMap.end(); ++it )
  {

    if( *it == true )
      accountRefreshing = true;
  }

  //emit sigRefreshReady if all accounts are ready
  if( !accountRefreshing )
  {
    emit sigRefreshReady();
  }

}

void AccountList::init()
{
  //assume, no window to show a mail is open at beginning
  ctrOpenMessageWindows = 0;
}

void AccountList::slotMessageWindowOpened( )
{
  //increment the window counter
  ctrOpenMessageWindows++;

  //if the counter was incremented from zero
  //(the first window was opened), emit the
  //signal
  if( ctrOpenMessageWindows == 1 )
    emit sigMessageWindowOpened();
}

void AccountList::slotMessageWindowClosed( )
{
  //decrement the window counter
  ctrOpenMessageWindows--;
  if( ctrOpenMessageWindows < 0 )
    ctrOpenMessageWindows = 0;

  //if counter is zero (all windows was closed),
  //emit signal
  if( ctrOpenMessageWindows == 0 )
    emit sigAllMessageWindowsClosed();
}

bool AccountList::keepNew( )
{
  return keepMailsNew;
}

int AccountList::getNumberMails( ) const
{
  QListIterator<Account*> it( accounts );   //to iterate over all accounts
  int number = 0;                             //number of mails

  //iterate over all accounts and sum up the number of mails
  while( it.hasNext() )
  {
    //get Account
    Account* account = it.next();

    //add number of mails of this account
    number += account->getNumberMails();

  }

  return number;
}

Mail* AccountList::getMail( int number ) const throw ( CorruptDataException )
{
  int nrMail = number;
  
  //search for the account
  QListIterator<Account*> it( accounts );
  while( it.hasNext() )
  {
    //get account
    Account* account = it.next();

    //get number of mails in this account
    int nrMails = account->getNumberMails();

    //is it in this account?
    if( nrMail < nrMails )
    {
      //this is the right account
      return account->getMail( nrMail );
    }

    //it is not the right account
    //we have to decrease the given mail number at the number of mails in account
    nrMail = nrMail - account->getNumberMails();
  }

  //we have not found anything
  throw CorruptDataException( i18n( "A mail with number %1 is not available.").arg( number ) );
}

QList<Mail*> AccountList::getSelectedMails( QItemSelectionModel* mailSelectModel ) const
{
  QList<Mail*> list;  //result list

  //get selected rows
  QModelIndexList indexList = mailSelectModel->selectedRows();

  //get the mail of every selected row an store the pointer into the result list
  QListIterator<QModelIndex> it( indexList );
  while( it.hasNext() )
  {
    //get Index
    QModelIndex index = it.next();

    //get mail
    Mail* mail = getMail( index.row() );

    //store mail pointer
    list.append( mail );
  }

  return list;
}

QStringList AccountList::getSelectedSubjects( QItemSelectionModel* mailSelectModel ) const
{
  QStringList list; //result list

  //get the selected mails
  QList<Mail*> mailList = getSelectedMails( mailSelectModel );

  //get the subjects of the selected mails and store their subjects
  QListIterator<Mail*> it( mailList );
  while( it.hasNext() )
  {
    //get Mail
    Mail* mail = it.next();

    //store subject
    list.append( mail->getSubject() );
    
  }

  return list;

}

void AccountList::deleteSelectedMails( QItemSelectionModel* mailSelectModel )
{
  //an account has a list of mails to delete
  //we put the mail number of every selected mail into the list of its account
  QList<Mail*> mailList = getSelectedMails( mailSelectModel );
  if( mailList.isEmpty() )
  {
    emit sigDeleteReady();
    return;
  }
  
  QListIterator<Mail*> it( mailList );
  while( it.hasNext() )
  {
    Mail* mail = it.next();
    mail->getAccount()->addMailToDelete( mail->getNumber() );
  }

  //order the accounts to delete the mails

  QListIterator<Account*> itAcc( accounts );

  //clear the map, which contains the names of the accounts,
  //which have gotten an order to delete
  accountDeletionMap.clear();

  //inserts an item for every account which will get an order to delete
  //its selected mails. The key is the account name and the data is TRUE.
  //it is important to do this in a seperate iteration because this avoids
  //race conditions
  while( itAcc.hasNext() )
  {
    //get Account
    Account* account = itAcc.next();
    
    //insert item
    accountDeletionMap.insert( account->getName(), true );

  }

  //order all accounts to delete its selected mail
  itAcc.toFront();
  while( itAcc.hasNext() )
  {
    //get Account
    Account* account = itAcc.next();

    account->deleteMails();
  }

  
}

void AccountList::slotCheckDeletionState( QString account )
{
  bool accountDeleting = false;     //set to TRUE if an account is still deleting
  AccountTaskMap_Type::Iterator it; //iterator over the account deletion map

  //set the appropriate item in AccountDeletionMap to FALSE
  accountDeletionMap[ account ] = false;

  //iterate over the account deletion map to check, whether all accounts
  //are ready
  for ( it = accountDeletionMap.begin(); it != accountDeletionMap.end(); ++it )
  {
    if( *it == true )
      accountDeleting = true;
  }

  //emit sigDeleteReady if all accounts are ready
  if( !accountDeleting )
    emit sigDeleteReady();
}

int AccountList::numberDeletedMailsLastRefresh( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list
  int number = 0;

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();

    number += account->numberDeletedMailsLastRefresh();
  }

  return number;
}

int AccountList::numberDeletedMailsStart( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list
  int number = 0;

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();


    number += account->numberDeletedMailsStart();
  }


  return number;
}

int AccountList::numberMovedMailsLastRefresh( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list
  int number = 0;

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();

    number += account->numberMovedMailsLastRefresh();
  }

  return number;
}

int AccountList::numberMovedMailsStart( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list
  int number = 0;

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();

    number += account->numberMovedMailsStart();
  }

  return number;
}

int AccountList::numberIgnoredMails( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list
  int number = 0;

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();

    number += account->numberIgnoredMails();
  }

  return number;
}

void AccountList::refreshFilterSetup( )
{
  QListIterator<Account*> it( accounts ); //iterator for the account list

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();

    //reload filter setup of the current account
    account->reloadFilterSettings();
  }

}


void AccountList::saveOptions ()
{
  //create XML document
  QDomDocument doc( "KShowmail" );

  //create root element
  QDomElement root = doc.createElement( ROOT_ELEMENT );

  //create for every account an element
  //the account saves its mails into this element
  //after that the element will be appended to the root element
  int i = 0;
  QListIterator<Account*> it( accounts ); //iterator for the account list

  //iterate over all accounts
  while( it.hasNext() )
  {
    Account* account = it.next();
    
    //save mails
    QDomElement accElem = doc.createElement( QString( ACCOUNT_ELEMENT ) + QString( "%1" ).arg( i++ ) );
    account->saveOptions( doc, accElem ); //account saves the mails into given XML document and the setup into the application config file
    root.appendChild( accElem );

  }

  //append root element to XML document
  doc.appendChild( root );

  //save XML document
  QString cachefilename = KStandardDirs::locateLocal( "appdata", MAIL_FILE ); //get file path
  QFile file( cachefilename );  //create file

  if( file.error() != QFile::NoError )
  {
    kdError() << "Couldn't save mail cache. " << file.errorString() << endl;
    return;
  }

  //open file
  if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
  {
    kdError() << "Couldn't save mail cache. " << file.errorString() << endl;
    return;    
  }

  //write data
  QTextStream stream( &file );
  doc.save( stream, 2 );
  stream.flush();

  //close file
  file.close();
  
}

QList<int> AccountList::getMarkedMails() const
{
  QList<int> list;
  
  if( accounts.isEmpty() ) return list;

  //this is the start index of an account.
  //The first account has a start index of zero. The first mail in its list has the index zero.
  //If the first account has four mails in its list, the start index of the second account is 4. And so on...
  int start = 0;

  QListIterator<Account*> it( accounts );
  while( it.hasNext() )
  {
    Account* account = it.next();

    //get all marked mails from this account
    QList<int> markedMails = account->getMarkedMails();

    QListIterator<int> itMarked( markedMails );
    while( itMarked.hasNext() )
    {
      //add the start index and store it to return
      list.append( itMarked.next() + start );
    }

    //increase the start index
    start = start + account->getNumberMails();
  }

  
  return list;
}

void AccountList::showSelectedMails( QItemSelectionModel* mailSelectModel )
{
  //an account has a list of mails to show
  //we put the mail number of every selected mail into the list of its account
  QList<Mail*> mailList = getSelectedMails( mailSelectModel );
  if( mailList.isEmpty() )
  {
    emit sigShowBodiesReady();
    return;
  }

  QListIterator<Mail*> it( mailList );
  while( it.hasNext() )
  {
    Mail* mail = it.next();
    mail->getAccount()->addMailToShow( mail->getNumber() );
  }

  //order the accounts to delete the mails

  QListIterator<Account*> itAcc( accounts );

  //clear the map, which contains the names of the accounts,
  //which have gotten an order to show its mails
  AccountShowBodiesMap.clear();

  //inserts an item for every account which will get an order to show
  //its selected mails. The key is the account name and the data is TRUE.
  //it is important to do this in a seperate iteration because this avoids
  //race conditions
  while( itAcc.hasNext() )
  {
    //get Account
    Account* account = itAcc.next();

    //insert item
    AccountShowBodiesMap.insert( account->getName(), true );

  }

  //order all accounts to show its selected mail
  itAcc.toFront();
  while( itAcc.hasNext() )
  {
    //get Account
    Account* account = itAcc.next();

    account->showMails();
  }


}

void AccountList::slotCheckShowBodiesState( QString account )
{
  bool accountDownloading = false;    //set to TRUE if an account is downloading mail body yet
  AccountTaskMap_Type::Iterator it;   //iterator over the account map

  //set the appropriate item in AccountShowBodiesMap to FALSE
  AccountShowBodiesMap[ account ] = false;

  //iterate over the account map to check, whether all accounts
  //are ready
  for ( it = AccountShowBodiesMap.begin(); it != AccountShowBodiesMap.end(); ++it )
  {
    if( *it == true )
      accountDownloading = true;
  }

  //emit sigShowBodiesReady if all accounts are ready
  //and assume all windows to show the mails are closed
  if( !accountDownloading )
  {
    emit sigShowBodiesReady();
    ctrOpenMessageWindows = 0;
  }
}

int AccountList::getNumberNewMails( )
{
  QListIterator<Account*> it( accounts );   //to iterate over all accounts
  int number = 0;                             //number of new mails

  //iterate over all accounts and sum up the number of new mails
  while( it.hasNext() )
  {
    Account* account = it.next();
    
    if( account->isActive() )
      number += account->getNumberNewMails();

  }

  return number;
}

void AccountList::readStoredMails( )
{
  //open file
  QString MailFileName = KStandardDirs::locateLocal( "appdata", MAIL_FILE );
  QFile file( MailFileName );
  bool fileOpen = file.open( QFile::ReadOnly );

  //return, if the file could not be opened
  if( !fileOpen )
  {
    kdError() << "AccountList::readStoredMails: File " << MailFileName << " could not be opened." << endl;
    return;
  }

  //create DOM document with the content read from the file
  QDomDocument doc( MAIL_FILE_DOCTYPE );
  QString* errorMsg = new QString();

  bool success = doc.setContent( &file );
  if( !success )
  {
    kdError() << "AccountList::readStoredMails: Invalid content in " << MAIL_FILE << ". " << *errorMsg << endl;
  }

  //get the root element
  QDomElement accs = doc.namedItem ( ROOT_ELEMENT ).toElement();

  //get the first account element
  QDomNode accNode = accs.firstChild();

  //get all account elements
  while( !accNode.isNull() )
  {
    //convert account node to DOM element
    QDomElement accElem = accNode.toElement();

    //get the account name
    QString accName = accElem.attribute( ATTRIBUTE_ACCOUNT_NAME );

    //get the proper account object
    Account* account = getAccount( accName );

		if( account != NULL ) {
		
			//order the account to read its stored mails
			account->readStoredMails( accElem );		
		}

    //get next account node
    accNode = accNode.nextSibling();
  }

  //close file
  file.close();
}

Account* AccountList::getAccount( QString name ) const {

	QListIterator<Account*> it( accounts );   //to iterate over all accounts

  while( it.hasNext() )
  {
    Account* account = it.next();
    
    if( account->getName() == name )
      return account;

  }

  return NULL;
}

QList< Account* > AccountList::getAllAccounts() const
{

  QList<Account*> list;

  list.append( accounts );

  return list;
}

