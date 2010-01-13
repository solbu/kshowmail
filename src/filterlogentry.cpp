

//
// C++ Implementation: filterlogentry
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterlogentry.h"

FilterLogEntry::FilterLogEntry()
{
  //set default values
  sentDateTime.setDate( QDate( 2007, 11, 7 ) );
  sentDateTime.setTime( QTime( 19, 05 ) );
  act = FActNone;
}

FilterLogEntry::FilterLogEntry( FilterAction_Type action, const QDateTime& dateTime, const QString& sender, const QString& account, const QString& subject, const QString& mailbox)
  : act( action ), sentDateTime( dateTime ), sender( sender ), account( account ), subject( subject ), mailbox( mailbox )
{
}

FilterLogEntry::~FilterLogEntry()
{
}

void FilterLogEntry::print()
{
  QString strAction;
  switch( act )
  {
    case FActPass       : strAction = "Passed"; break;
    case FActDelete     : strAction = "Deleted"; break;
    case FActMark       : strAction = "Marked"; break;
    case FActSpamcheck  : strAction = "forwarded to check for spam"; break;
    case FActMove       : strAction = QString( "moved to %1" ).arg( mailbox); break;
    case FActNone       : strAction = "no Action (THIS IS AN ERROR!)"; break;
    default             : strAction = "ERROR! UNKNOWN ACTION"; break;
  }

  kdDebug() << sentDateTime.toString( Qt::LocalDate ) << ";" << account << ";" << sender << ";" << subject << ";" << strAction << endl;
}

FilterLogEntry::FilterLogEntry(const FilterLogEntry & ent)
{
  this->sentDateTime = ent.sentDateTime;
  this->account = ent.account;
  this->sender = ent.sender;
  this->subject = ent.subject;
  this->act = ent.act;
  this->mailbox = ent.mailbox;
}

FilterLogEntry& FilterLogEntry::operator=( const FilterLogEntry & ent )
{
  if( this == &ent ) return *this;

  this->sentDateTime = ent.sentDateTime;
  this->account = ent.account;
  this->sender = ent.sender;
  this->subject = ent.subject;
  this->mailbox = ent.mailbox;
  this->act = ent.act;

  return *this;
}

bool FilterLogEntry::isOlder( uint days )
{
  return sentDateTime.date().addDays( days ) < QDate::currentDate();
}

bool FilterLogEntry::operator== ( const FilterLogEntry& ent ) const
{
  return sentDateTime == ent.sentDateTime;
}

bool FilterLogEntry::operator!=( const FilterLogEntry& ent ) const
{
  return sentDateTime != ent.sentDateTime;
}

bool FilterLogEntry::operator>( const FilterLogEntry& ent ) const
{
  return sentDateTime > ent.sentDateTime;
}

bool FilterLogEntry::operator>=( const FilterLogEntry& ent ) const
{
  return sentDateTime >= ent.sentDateTime;
}

bool FilterLogEntry::operator<( const FilterLogEntry& ent ) const
{
  return sentDateTime < ent.sentDateTime;
}

bool FilterLogEntry::operator<=( const FilterLogEntry & ent ) const
{
  return sentDateTime <= ent.sentDateTime;
}

void FilterLogEntry::save( QDomDocument& doc, QDomElement& parent )
{
  //create a new element and store the entry
  QDomElement elem = doc.createElement( LOG_ENTRY_ELEMENT );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_DATETIME, sentDateTime.toString( Qt::ISODate) );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_SENDER, sender );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_ACCOUNT, account );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_SUBJECT, subject );

  //add entry element to the log (parent) element
  parent.appendChild( elem );
}

QDateTime FilterLogEntry::getDate( )
{
  return sentDateTime;
}

QString FilterLogEntry::getSender( )
{
  return sender;
}

QString FilterLogEntry::getAccount( )
{
  return account;
}

QString FilterLogEntry::getSubject( )
{
  return subject;
}

QString FilterLogEntry::getMailbox( )
{
  return mailbox;
}
