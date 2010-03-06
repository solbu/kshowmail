/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "maillist.h"

MailList::MailList( QObject* parent ) : QObject( parent )
{
}

MailList::~MailList(){}

Mail* MailList::addMail( long number, const QString& unid, bool isNew )
{
  //create the mail object
  Mail* mail = new Mail( number, unid, isNew, this );

  //append it to the list
  mails.append( mail );
	
	return mail;
}

void MailList::print() const
{
  QListIterator<Mail*> iter( mails );
  while( iter.hasNext() )
  {
    Mail* mail = iter.next();
    mail->print();
    cout << "------------------" << endl;
  }
}

bool MailList::hasMail(QString uid)
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found

  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();
     
    //compare the uid
    if( mail->getUNID() == uid )
    {
      found = true;
    }
  }
  return found;

}

bool MailList::isNew(QString uid) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  bool newMail = false;                           //at time we have not found it, therefore the return value is false

  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();
    
    //compare the uid
    if( mail->getUNID() == uid )
    {
      found = true;
      newMail = mail->isNew();
    }
  }

  return newMail;
}

void MailList::setSize( long number, long size )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                  //True, when the wanted mail was found

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();
    
    //if the current mail has the given number, decode the mail
    if( mail->getNumber() == number )
    {
      mail->setSize( size );
      found = true;
    }
  }
}

Types::MailNumberList_Type MailList::getNewMails()
{
  MailNumberList_Type numberList;  //contains the numbers of the new mails

  QListIterator<Mail*> it( mails ); //iterator for the mail list

  //iterate over all mails
  while( it.hasNext() )
  {
    Mail* mail = it.next();
    
    //if current mail is new append its number to the mail number list
    if( mail->isNew() )
      numberList.append( mail->getNumber() );
  }

  return numberList;
}

void MailList::setHeader(int number, QStringList header)
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                  //True, when the wanted mail was found

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();
    
    //if the current mail has the given number, set the header
    if( mail->getNumber() == number )
    {
      mail->setHeader( header );
      found = true;
    }
  }

}

QStringList MailList::getUIDsOfOldMails( )
{
  QStringList list;                 //contains the UIDs of the old mails
  QListIterator<Mail*> it( mails ); //iterator for the mail list

  //iterate over all mails
  while( it.hasNext() )
  {
    //get next Mail
    Mail* mail = it.next();
    
    //if current mail is new append its number to the mail number list
    if( !mail->isNew() )
      list.append( mail->getUNID() );
  }

  return list;
}

QStringList MailList::getHeaderOf( QString unid ) throw( CorruptDataException )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list

  //looking for the mail with the UID 'uid'
  while( it.hasNext() )
  {
    //get next mail
    Mail* mail = it.next();
    
    //if the current mail has the given uid, get the header
    if( mail->getUNID() == unid )
    {
      return mail->getHeader();
    }
  }

  //nothing found
  throw CorruptDataException( "No mail with unid " + unid + " found in mail list" );
}

void MailList::setHeader( QString unid, QStringList header )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  //looking for the mail with the UID 'uid'
  while( it.hasNext() )
  {
    //get th next mail
    Mail* mail = it.next();
    
    //if the current mail has the given UID, set the header
    if( mail->getUNID() == unid )
    {
      mail->setHeader( header );
      return;
    }
  }
}

int MailList::getNumberMails() const
{
  return mails.count();
}

Account* MailList::getAccount() const
{
  return dynamic_cast<Account*>( parent() );
}

void MailList::applyHeaderFilter( HeaderFilter * filter, QString account, MailNumberList_Type& deleteList, MailToDownloadMap_Type& downloadList, int& nmbIgnoredMails, FilterLog* log )
{

  MailNumberList_Type mailsToIgnore;  //this list holds the numbers of all mails, which shall be ignored

  //Loop over all mails in this list
  QListIterator<Mail*> it( mails );
  while( it.hasNext() )
  {

    Mail* pElem = it.next();
    
    //apply the filters to the current mail
    QString mailbox;
    FilterAction_Type action = pElem->applyHeaderFilter( filter, account, mailbox, log );

    //do recommend action
    //we don't need to do everything for action MARK, because Mail::applyHeaderFilter() marks the mail entry itself
    struct DownloadActionParams_Type params;
    switch( action )
    {
      case FActDelete     : deleteList.append( pElem->getNumber() ); break;
      case FActMove       : params.action = FActMove;
                            params.mailbox = mailbox;
                            downloadList.insert( pElem->getNumber(), params );
                            break;
      case FActIgnore     : mailsToIgnore.append( pElem->getNumber() ); break;
      case FActSpamcheck  : params.action = FActSpamcheck;
                            downloadList.insert( pElem->getNumber(), params );
      default          : break;
    }
  }

  //remove all mails which shall be ignored from the mail list
  nmbIgnoredMails = mailsToIgnore.count();
  MailNumberList_Type::iterator iter;
  for ( iter = mailsToIgnore.begin(); iter != mailsToIgnore.end(); ++iter )
    removeMail( *iter );

}

void MailList::removeMail( int number )
{
  QMutableListIterator<Mail*> it( mails );   //iterator for the mail list

  //looking for the mail with the number 'number'
  while( it.hasNext() )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, remove it
    if( mail->getNumber() == number )
      it.remove();
  }
}

void MailList::saveMails( QDomDocument& doc, QDomElement& parent )
{
  //Loop over all mail items
  QListIterator<Mail*> it( mails );
  while( it.hasNext() )
  {
    Mail* mail = it.next();
    mail->save( doc, parent );
  }
}


QString MailList::getSenderOf( int number ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  QString sender;                                 //sender of the wanted mail

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();
    
    //if the current mail has the given number, remove it
    if( mail->getNumber() == number )
    {
      sender = mail->getFrom();
      found = true;
    }
  }
  return sender;
}

QString MailList::getDateOf( int number ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  QString date;                                   //sent date of the wanted mail

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, remove it
    if( mail->getNumber() == number )
    {
      date = mail->getDateTime().toString( KDateTime::LocalDate );
      found = true;
    }
  }
  return date;

}

QString MailList::getSizeOf( int number ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  QString size;                                   //size of the wanted mail

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, remove it
    if( mail->getNumber() == number )
    {
      size = mail->getSizeSuffix();
      found = true;
    }
  }
  return size;

}

QString MailList::getSubjectOf( int number ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  QString subject;                                //subject of the wanted mail

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, get the subject
    if( mail->getNumber() == number )
    {
      subject = mail->getSubject();
      found = true;
    }
  }
  return subject;

}

QString MailList::getCharsetFromHeaderOf( int number ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found
  QString charset;                                //subject of the wanted mail

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, get the subject
    if( mail->getNumber() == number )
    {
      charset = mail->getCharsetFromHeader();
      found = true;
    }
  }
  return charset;

}

QStringList MailList::decodeMailBody( const QStringList& body, int number, bool preferHTML ) const
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list

  //looking for the mail with the number 'number'
  while( it.hasNext() )
  {

    Mail* mail = it.next();
    
    //if the current mail has the given number, decode the mail
    if( mail->getNumber() == number )
    {
      return mail->decodeMailBody( body, preferHTML );
    }
  }

  //nothing found
  return QStringList();

}

void MailList::writeToMoveLog( FilterLog * log, int number, QString account, QString mailbox )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
		Mail* mail = it.next();
		
    //if the current mail has the given number, set the header
    if( mail->getNumber() == number )
    {
      mail->writeToMoveLog( log, account, mailbox );
      found = true;
    }
  }
}

void MailList::setMarkAtNextViewRefresh( int number )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
		Mail* mail = it.next();
		
    //if the current mail has the given number, set the header
    if( mail->getNumber() == number )
    {
      mail->setMarkAtNextViewRefresh();
      found = true;
    }
  }
}

void MailList::writeToDeleteLog( FilterLog * log, int number, QString account )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  bool found = false;                             //True, when the wanted mail was found

  //looking for the mail with the number 'number'
  while( it.hasNext() && !found )
  {
    Mail* mail = it.next();

    //if the current mail has the given number, set the header
    if( mail->getNumber() == number )
    {
      mail->writeToDeleteLog( log, account );
      found = true;
    }
  }
}

int MailList::getNumberNewMails( )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  int number = 0;                                 //number of new mails

  while( it.hasNext() )
  {
    Mail* mail = it.next();

    //increment number, if this mail is new
    if( mail->isNew() )
      number++;
  }

  return number;
}

long MailList::getTotalSize( )
{
  QListIterator<Mail*> it( mails );   //iterator for the mail list
  long size = 0;                                   //total size of all mails

  while( it.hasNext() )
  {
    Mail* mail = it.next();

    size += mail->getSize();
  }

  return size;
}


void MailList::readStoredMails( QDomElement& parent )
{
  //clear the list
  mails.clear();

  //get first DOM node (mail)
  QDomNode n = parent.firstChild();

  //iterate over all mail items stored in the given account
  while( !n.isNull() )
  {
    //get element of the current node
    QDomElement e = n.toElement();
		
    //get values
    int number = e.attribute( ATTRIBUTE_MAIL_NUMBER ).toInt();
    int size = e.attribute( ATTRIBUTE_MAIL_SIZE ).toInt();
    QString unid = e.attribute( ATTRIBUTE_MAIL_UID );
    QDomElement subelem = e.namedItem( ITEM_MAIL_HEADER ).toElement();
    QStringList header = subelem.text().split( HEADER_SEPARATOR );

    //create mail
		Mail* mail = addMail( number, unid, false );
		
		//set other values
		mail->setSize( size );
		mail->setHeader( header );

    //get next DOM node
    n = n.nextSibling();
  }
}

QList<Mail*> MailList::getAllMails() const
{
	QList<Mail*> list;
	list.append( mails );
	return list;
}

