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

#include "accountviewitem.h"

AccountViewItem::AccountViewItem( bool active, const QString& name, const QString& server, const QString& user, int numberMails, long totalSize, QPointer<Account> account )
  : active( active ), name( name ), server( server ), user( user ), numberMails( numberMails ), totalSize( totalSize ), account( account )
{
}

bool AccountViewItem::isActive() const
{
  return active;
}

QString AccountViewItem::getName() const
{
  return name;
}

QString AccountViewItem::getServer() const
{
  return server;
}

QPointer< Account > AccountViewItem::getAccount() const
{
  return account;
}

int AccountViewItem::getNumberMails() const
{
  return numberMails;
}

QString AccountViewItem::getTotalSizeUnit() const
{
  QString strSize;

  if( totalSize >= 1024 * 1024 )
  {
    //prefix is mega
    strSize = QString( "%L1M" ).arg( ( (double)totalSize / ( 1024 * 1024 ) ), 0, 'f', 1 );
  }
  else if( totalSize >= 1024 )
  {
    //prefix is kilo
    strSize = QString( "%L1K" ).arg( ( (double)totalSize / 1024 ), 0, 'f', 1 );
  }
  else
    //no prefix
    strSize = QString( "%L1" ).arg( totalSize );

  return strSize;

}

QString AccountViewItem::getUser() const
{
  return user;
}

int AccountViewItem::compare(const AccountViewItem& other, AccountSort_Type property)
{
  switch( property ) {

    //compare by active state
    case AccSortActive : {

      if( other.isActive() == isActive() ) return 0;
      else if( isActive() == false ) return -1;
      else return 1;
      break;

    }

    //compare by name
    case AccSortName : {

      return QString::localeAwareCompare( getName(), other.getName() );
    }

    //compare by server
    case AccSortServer : {

      return QString::localeAwareCompare( getServer(), other.getServer() );
    }

    //compare by user
    case AccSortUser : {

      return QString::localeAwareCompare( getUser(), other.getUser() );
    }

    //compare by number of messages
    case AccSortNrMess : {

      if( getNumberMails() == other.getNumberMails() ) return 0;
      else if( getNumberMails() < other.getNumberMails() ) return -1;
      else return 1;
    }

    //compare by total size of messages
    case AccSortSize : {

      if( getTotalSize() == other.getTotalSize() ) return 0;
      else if( getTotalSize() < other.getTotalSize() ) return -1;
      else return 1;
    }

    default: {
      return QString::localeAwareCompare( getName(), other.getName() );
    }
  }

}

long int AccountViewItem::getTotalSize() const
{
  return totalSize;
}
