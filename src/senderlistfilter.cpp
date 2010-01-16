//
// C++ Implementation: senderlistfilter
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "senderlistfilter.h"

SenderListFilter::SenderListFilter()
{

  //get the application config object
  config = KApplication::kApplication()->config();

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
  config->setGroup( CONFIG_GROUP_FILTER );

  //get lists
  blacklist = config->readListEntry( CONFIG_ENTRY_FILTER_BLACKLIST );
  whitelist = config->readListEntry( CONFIG_ENTRY_FILTER_WHITELIST );

  //get blacklist action
  switch( config->readNumEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, DEFAULT_FILTER_BLACKLIST_ACTION ) )
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
    if( sender.contains( *it, false ) || (*it).contains( sender, false ) )
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
