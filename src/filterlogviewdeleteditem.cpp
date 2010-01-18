//
// C++ Implementation: filterlogviewdeleteditem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterlogviewdeleteditem.h"

FilterLogViewDeletedItem::FilterLogViewDeletedItem( QTreeWidget* parent )
 : QTreeWidgetItem( parent )
{
}


FilterLogViewDeletedItem::~FilterLogViewDeletedItem()
{
}

void FilterLogViewDeletedItem::setValues( KDateTime date, QString sender, QString account, QString subject )
{
  //store values
  this->date = date;
  this->sender = sender;
  this->account = account;
  this->subject = subject;

  //set column text
  setText( ColDate, date.toString( KDateTime::LocalDate ) );
  setText( ColSender, sender );
  setText( ColAccount, account );
  setText( ColSubject, subject );
}


KDateTime FilterLogViewDeletedItem::getDate( )
{
  return date;
}



