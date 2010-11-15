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

#include "senderlistfilter.h"

SenderListFilter::SenderListFilter()
{

  //get the application config object
  config = KGlobal::config();

  //load the setup
  load();

}


SenderListFilter::~SenderListFilter()
{
}

FilterAction_Type SenderListFilter::check( QString sender ) const
{
  //return with none action if the given string is empty
  if( sender.isEmpty() ) return FActNone;

  //check the whitelist first
  if( search( whitelist, sender ) ) return FActPass;

  //check blacklist
  if( search( blacklist, sender ) ) return blacklistAction;

  //this is the default
  return FActNone;
}

void SenderListFilter::load( )
{
  //set group
  KConfigGroup* configFilter = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //get lists
  blacklist = configFilter->readEntry( CONFIG_ENTRY_FILTER_BLACKLIST, QStringList() );
  whitelist = configFilter->readEntry( CONFIG_ENTRY_FILTER_WHITELIST, QStringList() );

  //get blacklist action
  switch( configFilter->readEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, DEFAULT_FILTER_BLACKLIST_ACTION ) )
  {
    case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE  : blacklistAction = FActDelete; break;
    case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK    : blacklistAction = FActMark; break;
    default                                           : blacklistAction = FActMark; break;
  }
}

bool SenderListFilter::search( QStringList list, QString sender ) const
{
  //return with FALSE if the list or the search string are empty
  if( list.isEmpty() || sender.isEmpty() ) return false;

  //iterate over the list to search for the sender
  bool found = false;
  for( QStringList::Iterator it = list.begin(); it != list.end() && found == false; ++it )
  {
    if( sender.contains( *it, Qt::CaseInsensitive ) || (*it).contains( sender, Qt::CaseInsensitive ) )
      found = true;
  }

  return found;
}

void SenderListFilter::print( )
{
  kdDebug() << "Blacklist:" << endl;
  for( QStringList::Iterator it = blacklist.begin(); it != blacklist.end(); ++it )
  {
    kdDebug() << *it << endl;
  }

  switch( blacklistAction )
  {
    case FActDelete   : kdDebug() << "Blacklist Action: DELETE" << endl; break;
    case FActMark     : kdDebug() << "Blacklist Action: MARK " << endl; break;
    default           : kdDebug() << "Blacklist Action: Unknown" << endl; break;
  }

  kdDebug() << endl;

  kdDebug() << "Whitelist:" << endl;
  for( QStringList::Iterator it = whitelist.begin(); it != whitelist.end(); ++it )
  {
    kdDebug() << *it << endl;
  }

}

bool SenderListFilter::isOnWhitelist(const QString& sender) const
{
  return search( whitelist, sender );
}

