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

void MailList::addMail( const QString& unid )
{
  //create the mail object
  Mail* mail = new Mail( unid, this );

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

