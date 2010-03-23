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

#include "filterlogviewmoveditem.h"

FilterLogViewMovedItem::FilterLogViewMovedItem( QTreeWidget * parent )
  : QTreeWidgetItem( parent )
{
}


FilterLogViewMovedItem::~FilterLogViewMovedItem()
{
}

void FilterLogViewMovedItem::setValues( KDateTime date, QString sender, QString account, QString mailbox, QString subject )
{
  //store values
  this->date = date;
  this->sender = sender;
  this->account = account;
  this->mailbox = mailbox;
  this->subject = subject;

  //set column text
  setText( ColDate, date.toString( KDateTime::LocalDate ) );
  setText( ColSender, sender );
  setText( ColAccount, account );
  setText( ColMailbox, mailbox );
  setText( ColSubject, subject );
}

KDateTime FilterLogViewMovedItem::getDate( )
{
  return date;
}

