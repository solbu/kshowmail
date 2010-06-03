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

#include "filtersetupitem.h"

FilterSetupItem::FilterSetupItem( QTreeWidget* parent )
 : QTreeWidgetItem( parent )
{
  //set default number
  number = 0;

  init();
}

FilterSetupItem::FilterSetupItem( QTreeWidget* parent, uint num )
  : QTreeWidgetItem( parent )
{
  //store the given filter number
  number = num;

  init();
}

FilterSetupItem::~FilterSetupItem()
{
}

void FilterSetupItem::init( )
{
  //get application config object (kshowmailrc)
  config = KGlobal::config();

  //set default values
  setName( i18n( DEFAULT_FILTER_NAME ) );
  setCriteriaLinkage( DEFAULT_FILTER_CRITERIA_LINKAGE );
  setAction( DEFAULT_FILTER_ACTION );

  //show number in column
  setText( ColNumber, QString("%1").arg( number ) );
}

void FilterSetupItem::setName( QString name )
{
  this->name = name;

  //show name in the list view
  setText( ColName, name );
}

QString FilterSetupItem::getName( ) const
{
  return name;
}

void FilterSetupItem::setNumber( uint number )
{
  this->number = number;

  //show number in column
  setText( ColNumber, QString("%1").arg( number ) );
}

void FilterSetupItem::setAction( int action )
{
  if( action == CONFIG_VALUE_FILTER_ACTION_PASS ||
      action == CONFIG_VALUE_FILTER_ACTION_DELETE ||
      action == CONFIG_VALUE_FILTER_ACTION_MARK ||
      action == CONFIG_VALUE_FILTER_ACTION_MOVE ||
      action == CONFIG_VALUE_FILTER_ACTION_IGNORE ||
      action == CONFIG_VALUE_FILTER_ACTION_SPAMCHECK )
  {
    this->action = action;
  }
  else
  {
    kdError() << "FilterSetupItem::setAction: Invalid parameter for action given. Parameter: " << action << endl;
    this->action = DEFAULT_FILTER_ACTION;
    return;
  }

  //update the text of the rule column
  updateActionColumn();
}

void FilterSetupItem::save( )
{
  //build group name
  QString group;
  group = QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( number );

  //first delete the group to clear it of old criteria entries
  config->deleteGroup( group );

  //set group
  KConfigGroup* configFilter = new KConfigGroup( config, group );

  //write entries
  configFilter->writeEntry( CONFIG_ENTRY_FILTER_NAME, name );

  if( linkage == CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL ||
      linkage == CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY )
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE, linkage );
  }
  else
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE, DEFAULT_FILTER_CRITERIA_LINKAGE );
  }

  if( action == CONFIG_VALUE_FILTER_ACTION_PASS ||
      action == CONFIG_VALUE_FILTER_ACTION_DELETE ||
      action == CONFIG_VALUE_FILTER_ACTION_MARK ||
      action == CONFIG_VALUE_FILTER_ACTION_MOVE ||
      action == CONFIG_VALUE_FILTER_ACTION_IGNORE ||
      action == CONFIG_VALUE_FILTER_ACTION_SPAMCHECK )
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_ACTION, action );
  }
  else
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_ACTION, DEFAULT_FILTER_ACTION );
  }

  //write action parameter
  switch( action )
  {
    case CONFIG_VALUE_FILTER_ACTION_MOVE  :
      configFilter->writeEntry( CONFIG_ENTRY_FILTER_MOVE_MAILBOX, mailbox );
      break;

    default: break;
  }

  //write criteria list and number of criterias
  if( !criteriaList.empty() )
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_CRITERIA_NUMBER, (int)criteriaList.size() );

    int ctr = 0; //number of the current criteria
    FilterCriteriaList_Type::iterator it;
    for( it = criteriaList.begin(); it != criteriaList.end(); ++it )
    {
      ctr++;

      configFilter->writeEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_SOURCE ).arg( ctr ), (*it).source );
      configFilter->writeEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CONDITION ).arg( ctr ), (*it).condition );

      switch( (*it).source )
      {
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  :   configFilter->writeEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( ctr ), (*it).txtValue );
                                                              configFilter->writeEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CASESENSITIVE ).arg( ctr ), (*it).cs );
                                                              break;

        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE     :   configFilter->writeEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( ctr ), (*it).numValue );
                                                              break;
      }
    }
  }
  else
  {
    configFilter->writeEntry( CONFIG_ENTRY_FILTER_CRITERIA_NUMBER, 0 );
  }

}

void FilterSetupItem::setCriteriaLinkage( int linkage )
{
  if( linkage == CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL ||
      linkage == CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY )
  {
    this->linkage = linkage;
  }
  else
  {
    kdError() << "FilterSetupItem::setCriteriaLinkage: Invalid parameter for criteria linkage given. Parameter: " << linkage << endl;
    this->linkage = DEFAULT_FILTER_CRITERIA_LINKAGE;
    return;
  }

}

void FilterSetupItem::setCriteriaList( FilterCriteriaList_Type list )
{
  criteriaList = list;
}

void FilterSetupItem::setMailBox( QString mailbox )
{
  this->mailbox = mailbox;

  //update the text of the rule column
  updateActionColumn();
}

int FilterSetupItem::getCriteriaLinkage( ) const
{
  return linkage;
}

int FilterSetupItem::getAction( ) const
{
  return action;
}

QString FilterSetupItem::getMailBox( ) const
{
  if( action == CONFIG_VALUE_FILTER_ACTION_MOVE )
    return mailbox;
  else
    return QString( "" );
}

FilterCriteriaList_Type FilterSetupItem::getCriteriaList( ) const
{
  return criteriaList;
}

uint FilterSetupItem::getNumber( ) const
{
  return number;
}

int FilterSetupItem::compare( FilterSetupItem * i, int col, bool ascending ) const
{
  if( col == ColNumber )
  {
    return text( ColNumber ).toInt() - i->text( ColNumber ).toInt();
  }
  else
  {
    if( ascending )
    {
      return text( col ).compare( text( col ) );
    }
    else
    {
      return text( col ).compare( text( col ) ) * -1;
    }
  }
    

}

void FilterSetupItem::updateActionColumn( )
{
  QString strAction;   //filter action

  //build action
  switch( action )
  {
    case CONFIG_VALUE_FILTER_ACTION_PASS      : strAction = i18nc( "@item:intable show the filtered mail in the view", "Show" ); break;
    case CONFIG_VALUE_FILTER_ACTION_DELETE    : strAction = i18nc( "@item:intable delete the filtered mail", "Delete" ); break;
    case CONFIG_VALUE_FILTER_ACTION_MARK      : strAction = i18nc( "@item:intable mark the filtered mail", "Mark" ); break;
    case CONFIG_VALUE_FILTER_ACTION_MOVE      : strAction = i18nc( "@item:intable move the filtered mail into a mailbox", "Move to <resource>%1</resource>", mailbox ); break;
    case CONFIG_VALUE_FILTER_ACTION_IGNORE    : strAction = i18nc( "@item:intable ignore the filtered mail", "Ignore"); break;
    case CONFIG_VALUE_FILTER_ACTION_SPAMCHECK : strAction = i18nc( "@item:intable check the filtered mail for spam", "Check for spam" ); break;
    default                                   : strAction = i18nc( "@item:intable unknown action", "Unknown action" ); break;
  }

  //set text
  setText( ColAction, QString( " %1" ).arg( strAction ) );
}

void FilterSetupItem::load( )
{
    //build group name
  QString group;
  group = QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( number );


  //set group
  //set group
  KConfigGroup* configFilter = new KConfigGroup( config, group );

  //get name
  setName( configFilter->readEntry( CONFIG_ENTRY_FILTER_NAME, DEFAULT_FILTER_NAME ) );

  //get linkage
  setCriteriaLinkage( configFilter->readEntry( CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE, DEFAULT_FILTER_CRITERIA_LINKAGE ) );
  if( linkage != CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL && linkage != CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY )
    setCriteriaLinkage( DEFAULT_FILTER_CRITERIA_LINKAGE );

  //get action
  setAction( configFilter->readEntry( CONFIG_ENTRY_FILTER_ACTION, DEFAULT_FILTER_ACTION ) );
  if( action != CONFIG_VALUE_FILTER_ACTION_PASS &&
      action != CONFIG_VALUE_FILTER_ACTION_DELETE &&
      action != CONFIG_VALUE_FILTER_ACTION_MARK &&
      action != CONFIG_VALUE_FILTER_ACTION_MOVE &&
      action != CONFIG_VALUE_FILTER_ACTION_IGNORE &&
      action != CONFIG_VALUE_FILTER_ACTION_SPAMCHECK )
    setAction( DEFAULT_FILTER_ACTION );

  //get action parameter
  switch( action )
  {
    case CONFIG_VALUE_FILTER_ACTION_MOVE : setMailBox( configFilter->readEntry( CONFIG_ENTRY_FILTER_MOVE_MAILBOX, DEFAULT_FILTER_ACTION_MOVE_MAILBOX ) ); break;
    default                              : setMailBox( QString() );
  }

  //get number of criterias
  uint numCrit = configFilter->readEntry( CONFIG_ENTRY_FILTER_CRITERIA_NUMBER, 0 );

  //get criterias
  for( uint ctr = 1; ctr <= numCrit; ctr++ )
  {
    struct FilterCriteria_Type crit;

    crit.source = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_SOURCE ).arg( ctr ), DEFAULT_FILTER_CRITERIA_SOURCE );
    if( crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM &&
        crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO &&
        crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT &&
        crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER  &&
        crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  &&
        crit.source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE )
      crit.source = DEFAULT_FILTER_CRITERIA_SOURCE;

    switch( crit.source )
    {
      case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE   : crit.condition = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CONDITION ).arg( ctr ), DEFAULT_FILTER_CRITERIA_COND_NUM );
                                                        if( crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL &&
                                                            crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL &&
                                                            crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER &&
                                                            crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL &&
                                                            crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS &&
                                                            crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL )
                                                          crit.condition = DEFAULT_FILTER_CRITERIA_COND_NUM;
                                                        crit.numValue = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( ctr ), DEFAULT_FILTER_CRITERIA_SIZE );
                                                        break;

      default                                         :  crit.condition = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CONDITION ).arg( ctr ), DEFAULT_FILTER_CRITERIA_COND_TEXT );
                                                         if( crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS &&
                                                             crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS &&
                                                             crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS &&
                                                             crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS &&
                                                             crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR &&
                                                             crit.condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR )
                                                           crit.condition = DEFAULT_FILTER_CRITERIA_COND_TEXT;
                                                         crit.txtValue = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( ctr ) );
                                                         crit.cs = configFilter->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CASESENSITIVE ).arg( ctr ), DEFAULT_FILTER_CRITERIA_CASE_SENSITIVE );
                                                         break;
    }

    criteriaList.push_back( crit );

  }


}



