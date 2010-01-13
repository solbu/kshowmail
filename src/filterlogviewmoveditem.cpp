//
// C++ Implementation: filterlogviewmoveditem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterlogviewmoveditem.h"

FilterLogViewMovedItem::FilterLogViewMovedItem( QTreeWidget * parent )
  : QTreeWidgetItem( parent )
{
}


FilterLogViewMovedItem::~FilterLogViewMovedItem()
{
}

void FilterLogViewMovedItem::setValues( QDateTime date, QString sender, QString account, QString mailbox, QString subject )
{
  //store values
  this->date = date;
  this->sender = sender;
  this->account = account;
  this->mailbox = mailbox;
  this->subject = subject;

  //set column text
  setText( ColDate, date.toString( Qt::LocalDate ) );
  setText( ColSender, sender );
  setText( ColAccount, account );
  setText( ColMailbox, mailbox );
  setText( ColSubject, subject );
}

QDateTime FilterLogViewMovedItem::getDate( )
{
  return date;
}

