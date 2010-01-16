//
// C++ Implementation: filteritem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filteritem.h"

FilterItem::FilterItem( uint filterNr )
{
  //get the application config object
  config = KApplication::kApplication()->config();

  //save number
  filterNumber = filterNr;

  //set config group
  config->setGroup( QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( filterNr ) );

  //get name
  name = config->readEntry( CONFIG_ENTRY_FILTER_NAME );

  //get number of criterias
  numberCriterias = config->readNumEntry( CONFIG_ENTRY_FILTER_CRITERIA_NUMBER );

  //get criteria linkage
  switch( config->readNumEntry( CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE, DEFAULT_FILTER_CRITERIA_LINKAGE ) )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL : linkage = LinkAll; break;
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY : linkage = LinkAny; break;
    default                                             : kdError() << "FilterItem::FilterItem(): Unknown linkage value read. Set ALL. ( Filter " << filterNumber << ")" << endl;
                                                          linkage = LinkAll;
                                                          break;
  }

  //get action
  switch( config->readNumEntry( CONFIG_ENTRY_FILTER_ACTION, DEFAULT_FILTER_ACTION ) )
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
    mailbox = config->readEntry( CONFIG_ENTRY_FILTER_MOVE_MAILBOX );
    if( mailbox.isNull() || mailbox.isEmpty() )
    {
      kdWarning() << "Filter " << filterNumber << ": No mailbox name found. Set default: " << DEFAULT_FILTER_ACTION_MOVE_MAILBOX << endl;
      mailbox = QString( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
    }
  }

  //now we get the criterias
  criterias.setAutoDelete( true ); //the list shall delete all criterias if it will be deleted itself
  for( uint critNr = 1; critNr <= numberCriterias; critNr++ )
  {
    criterias.append( new FilterItemCriteria( filterNr, critNr ) ); //a new created criteria loads its settings itself
  }
}


FilterItem::~FilterItem()
{
}

FilterAction_Type FilterItem::check( QString from, QString to, uint size, QString subject, QString header, QString account, QString& mailboxName ) const
{
  bool match = false;   //TRUE, if filter matches

  //return NONE if no criterias available
  if( criterias.isEmpty() ) return FActNone;

  //get iterator
  QPtrListIterator<FilterItemCriteria> it( criterias );

  FilterItemCriteria* crit;

  //check criterias
  if( linkage == LinkAll )
  {
    match = true;
    while( ( crit = it.current() ) != NULL && match )
    {
      ++it;

      match = match && crit->check( from, to, size, subject, header, account );
    }
  }
  else if( linkage == LinkAny )
  {
    match = false;
    while( ( crit = it.current() ) != NULL && !match )
    {
      ++it;

      match = crit->check( from, to, size, subject, header, account );
    }
  }
  else
    kdError() << "FilterItem::check(): Unknown linkage (Filter " << filterNumber << ")" << endl;

  //return action if filter matches
  if( match )
  {
    //set mailbox name if neccessary
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
  QPtrListIterator<FilterItemCriteria> it( criterias );
  FilterItemCriteria* crit;
  while( ( crit = it.current() ) != NULL )
  {
    ++it;
    crit->print();
  }
}


