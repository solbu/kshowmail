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

void MailList::addMail( long number, const QString& unid, bool isNew )
{
  //create the mail object
  Mail* mail = new Mail( number, unid, isNew, this );

  //append it to the list
  mails.append( mail );
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
  kdDebug() << "Set Header of " << number << endl;
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



