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

#include "headerfilter.h"

HeaderFilter::HeaderFilter()
{
  //get the application config object
  config = KGlobal::config();

  //load settings
  load();
}


HeaderFilter::~HeaderFilter()
{
  //delete all filters
  QListIterator<FilterItem*> it( filters );
  while( it.hasNext() )
  {
    FilterItem* item = it.next();
    delete item;
  }
}

FilterAction_Type HeaderFilter::check( QString from, QString to, uint size, QString subject, QStringList header, QString account, QString& mailboxName ) const
{
  //return PASS, if filter is not active
  if( !active )
    return FActPass;
  
  //check for matching with blacklist or whitelist
  FilterAction_Type action = senderlist.check( from );
  if( action != FActNone ) return action;

  //check for matching with filters
  QListIterator<FilterItem*> it( filters );
  while( it.hasNext() )
  {
    FilterItem* filter = it.next();
    action = filter->check( from, to, size, subject, header, account, mailboxName );

    if( action != FActNone ) return action;
  }

  //no matching; return default action
  if( defaultAction == FActMove )
  {
    mailboxName.remove( 0, mailboxName.length() );
    mailboxName.append( mailbox );
  }
  return defaultAction;

}

void HeaderFilter::load( )
{
  //order sender list to load its settings
  senderlist.load();

  //set group
  KConfigGroup* configFilter = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //get filter active state
  active = configFilter->readEntry( CONFIG_ENTRY_FILTER_ACTIVE, DEFAULT_FILTER_ACTIVE );

  //get number of filter items
  numberFilterItems = configFilter->readEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, 0 );

  //get default action
  switch( configFilter->readEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ) )
  {
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : defaultAction = FActPass; break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : defaultAction = FActDelete; break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : defaultAction = FActMark; break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : defaultAction = FActMove; break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : defaultAction = FActIgnore; break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : defaultAction = FActSpamcheck; break;
    default                                           : kdError() << "Header Filter: Unknown default filter action. Set PASS." << endl;
                                                        defaultAction = FActPass;
                                                        break;
  }

  //get mailbox name if default action is MOVE
  if( defaultAction == FActMove )
    mailbox = configFilter->readEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX, DEFAULT_FILTER_ACTION_MOVE_MAILBOX );

  //now we get the filters
  //first clear the list
  filters.clear();

  for( uint filterNr = 1; filterNr <= numberFilterItems; filterNr++ )
  {
    filters.append( new FilterItem( filterNr ) ); //a new created filter item loads its settings itself
  }
}

void HeaderFilter::print( )
{
  kdDebug() << "Header Filter Settings:" << endl;
  kdDebug() << "-----------------------" << endl;

  //print active state
  if( active )
    kdDebug() << "Header filter is active." << endl;
  else
    kdDebug() << "Header filter is not active." << endl;

  //print settings of black and white list
  senderlist.print();

  //print filters
  kdDebug() << endl;
  kdDebug() << "Number of filters: " << numberFilterItems << endl << endl;

  QListIterator<FilterItem*> it( filters );
  while( it.hasNext() )
  {
    FilterItem* filter = it.next();
    filter->print();
    kdDebug() << endl;
  }

  //print default action for not matched mails
  switch( defaultAction )
  {
    case FActPass       : kdDebug() << "Default action for other mails: PASS" << endl; break;
    case FActDelete     : kdDebug() << "Default action for other mails: DELETE" << endl; break;
    case FActMark       : kdDebug() << "Default action for other mails: MARK" << endl; break;
    case FActIgnore     : kdDebug() << "Default action for other mails: IGNORE" << endl;
    case FActMove       : kdDebug() << "Default action for other mails: MOVE to " << mailbox << endl; break;
    case FActSpamcheck  : kdDebug() << "Default action for other mails: SPAMCHECK" << endl; break;
    default             : kdDebug() << "Unknown default action for other mails" << endl; break;
  }


}

bool HeaderFilter::isActive()
{
  return active;
}


