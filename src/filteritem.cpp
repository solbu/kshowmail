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

#include "filteritem.h"

FilterItem::FilterItem( uint filterNr )
{
  //save number
  filterNumber = filterNr;

  //get config group
  KConfigGroup* confFilter = new KConfigGroup( KGlobal::config(), QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( filterNr ) );

  //get name
  name = confFilter->readEntry( CONFIG_ENTRY_FILTER_NAME );

  //get number of criterias
  numberCriterias = confFilter->readEntry( CONFIG_ENTRY_FILTER_CRITERIA_NUMBER, 0 );

  //get criteria linkage
  switch( confFilter->readEntry( CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE, DEFAULT_FILTER_CRITERIA_LINKAGE ) )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL : linkage = LinkAll; break;
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY : linkage = LinkAny; break;
    default                                             : kdError() << "FilterItem::FilterItem(): Unknown linkage value read. Set ALL. ( Filter " << filterNumber << ")" << endl;
                                                          linkage = LinkAll;
                                                          break;
  }

  //get action
  switch( confFilter->readEntry( CONFIG_ENTRY_FILTER_ACTION, DEFAULT_FILTER_ACTION ) )
  {
    case CONFIG_VALUE_FILTER_ACTION_PASS      : action = FActPass; break;
    case CONFIG_VALUE_FILTER_ACTION_DELETE    : action = FActDelete; break;
    case CONFIG_VALUE_FILTER_ACTION_MARK      : action = FActMark; break;
    case CONFIG_VALUE_FILTER_ACTION_MOVE      : action = FActMove; break;
    case CONFIG_VALUE_FILTER_ACTION_IGNORE    : action = FActIgnore; break;
    case CONFIG_VALUE_FILTER_ACTION_SPAMCHECK : action = FActSpamcheck; break;
    default                                   : kdError() << "FilterItem::FilterItem(): Unknown filter action read. Set PASS. (Filter " << filterNumber << ")" << endl;
                                                action = FActPass;
                                                break;
  }

  //get mailbox name if filter action is move
  if( action == FActMove )
  {
    mailbox = confFilter->readEntry( CONFIG_ENTRY_FILTER_MOVE_MAILBOX );
    if( mailbox.isNull() || mailbox.isEmpty() )
    {
      kdWarning() << "Filter " << filterNumber << ": No mailbox name found. Set default: " << DEFAULT_FILTER_ACTION_MOVE_MAILBOX << endl;
      mailbox = QString( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
    }
  }

  //now we get the criterias
  for( uint critNr = 1; critNr <= numberCriterias; critNr++ )
  {
    criterias.append( new FilterItemCriteria( filterNr, critNr ) ); //a new created criteria loads its settings itself
  }
}


FilterItem::~FilterItem()
{

  //remove all criterias
  QListIterator<FilterItemCriteria*> it( criterias );
  while( it.hasNext() )
  {
    delete it.next();
  }
}

FilterAction_Type FilterItem::check( QString from, QString to, uint size, QString subject, QStringList header, QString account, QString& mailboxName ) const
{
  bool match = false;   //TRUE, if filter matches

  //return NONE if no criterias available
  if( criterias.isEmpty() ) return FActNone;

  //get iterator
  QListIterator<FilterItemCriteria*> it( criterias );


  //check criterias
  if( linkage == LinkAll )
  {
    match = true;
    while( it.hasNext() && match )
    {
      FilterItemCriteria* crit = it.next();

      match = match && crit->check( from, to, size, subject, header, account );
    }
  }
  else if( linkage == LinkAny )
  {
    match = false;
    while( it.hasNext() && !match )
    {
      FilterItemCriteria* crit = it.next();

      match = crit->check( from, to, size, subject, header, account );
    }
  }
  else
    kdError() << "FilterItem::check(): Unknown linkage (Filter " << filterNumber << ")" << endl;

  //return action if filter matches
  if( match )
  {
    //set mailbox name if necessary
    if( action == FActMove )
    {
      mailboxName.remove( 0, mailboxName.length() );
      mailboxName.append( mailbox );
    }

    return action;
  }

  //default return value, filter doesn't match
  return FActNone;
}

void FilterItem::print( ) const
{
  kdDebug() << "Settings of filter " << filterNumber << ":" << endl;
  kdDebug() << "Name: " << name << endl;
  kdDebug() << "Number of criterias: " << numberCriterias << endl;

  switch( linkage )
  {
    case LinkAll  : kdDebug() << "Criteria Linkage: ALL (AND)" << endl; break;
    case LinkAny  : kdDebug() << "Criteria Linkage: ANY (OR)" << endl; break;
    default       : kdDebug() << "Unknown Criteria LInkage" << endl; break;
  }

  switch( action )
  {
    case FActPass       : kdDebug() << "Action: PASS" << endl; break;
    case FActDelete     : kdDebug() << "Action: DELETE" << endl; break;
    case FActMark       : kdDebug() << "Action: MARK" << endl; break;
    case FActMove       : kdDebug() << "Action: MOVE to " << mailbox << endl; break;
    case FActSpamcheck  : kdDebug() << "Action: SPAMCHECK" << endl; break;
    case FActIgnore     : kdDebug() << "Action: IGNORE" << endl; break;
    default             : kdDebug() << "Unknown Action" << endl; break;
  }

  kdDebug() << "Criterias:" << endl;
  QListIterator<FilterItemCriteria*> it( criterias );
  while( it.hasNext() )
  {
    FilterItemCriteria* crit = it.next();
    crit->print();
  }
}

QString FilterItem::getName() const
{
  return name;
}

