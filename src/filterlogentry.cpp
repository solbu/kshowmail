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

#include "filterlogentry.h"

FilterLogEntry::FilterLogEntry()
{
  //set default values
  sentDateTime.setDate( QDate( 2007, 11, 7 ) );
  sentDateTime.setTime( QTime( 19, 05 ) );
  act = FActNone;
}

FilterLogEntry::FilterLogEntry( FilterAction_Type action, const KDateTime& dateTime, const QString& sender, const QString& account, const QString& subject, const QString& mailbox, KindOfMailDeleting kindDelete, const QString& filter )
  : act( action ), sentDateTime( dateTime ), sender( sender ), account( account ), subject( subject ), mailbox( mailbox ), kindDel( kindDelete ), filter( filter )
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

  kdDebug() << sentDateTime.toString( KDateTime::LocalDate ) << ";" << account << ";" << sender << ";" << subject << ";" << strAction << endl;
}

FilterLogEntry::FilterLogEntry(const FilterLogEntry & ent)
{
  this->sentDateTime = ent.sentDateTime;
  this->account = ent.account;
  this->sender = ent.sender;
  this->subject = ent.subject;
  this->act = ent.act;
  this->mailbox = ent.mailbox;
  this->kindDel = ent.kindDel;
  this->filter = ent.filter;
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
  this->kindDel = ent.kindDel;
  this->filter = ent.filter;

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
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_DATETIME, sentDateTime.toString( KDateTime::ISODate ) );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_SENDER, sender );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_ACCOUNT, account );
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_SUBJECT, subject );
  if( kindDel == DelFilter ) {
    elem.setAttribute( LOG_ENTRY_ATTRIBUTE_KIND_DELETE, LOG_ENTRY_VALUE_KIND_DELETE_FILTER );
  } else {
    elem.setAttribute( LOG_ENTRY_ATTRIBUTE_KIND_DELETE, LOG_ENTRY_VALUE_KIND_DELETE_MANUAL );
  }
  elem.setAttribute( LOG_ENTRY_ATTRIBUTE_FILTER, filter );

  //add entry element to the log (parent) element
  parent.appendChild( elem );
}

KDateTime FilterLogEntry::getDate( ) const
{
  return sentDateTime;
}

QString FilterLogEntry::getSender( ) const
{
  return sender;
}

QString FilterLogEntry::getAccount( ) const
{
  return account;
}

QString FilterLogEntry::getSubject( ) const
{
  return subject;
}

QString FilterLogEntry::getMailbox( ) const
{
  return mailbox;
}

int FilterLogEntry::compare( const FilterLogEntry& other, LogViewSort property ) const
{
  switch( property ) {

    case( LogViewSortKind ) : {

      if( getKindOfDeleting() == other.getKindOfDeleting() ) return 0;
      else if( getKindOfDeleting() > other.getKindOfDeleting() ) return 1;
      else return -1;
    }

    case( LogViewSortDate ) : {

      if( getDate() == other.getDate() ) return 0;
      else if( getDate() > other.getDate() ) return 1;
      else return -1;

    }

    case( LogViewSortFrom ) : {

      return QString::localeAwareCompare( getSender(), other.getSender() );
    }

    case( LogViewSortAccount ) : {

      return QString::localeAwareCompare( getAccount(), other.getAccount() );
    }

    case( LogViewSortSubject ) : {

      return QString::localeAwareCompare( getSubject(), other.getSubject() );
    }

    case( LogViewSortMailbox ) : {

      return QString::localeAwareCompare( getMailbox(), other.getMailbox() );
    }

    default : {

      if( getDate() == other.getDate() ) return 0;
      else if( getDate() > other.getDate() ) return 1;
      else return -1;

    }

  }
}

KindOfMailDeleting FilterLogEntry::getKindOfDeleting() const
{
  return kindDel;
}

QString FilterLogEntry::getFilter() const
{
  return filter;
}

