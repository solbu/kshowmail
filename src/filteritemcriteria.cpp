//
// C++ Implementation: filteritemcriteria
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filteritemcriteria.h"

FilterItemCriteria::FilterItemCriteria( uint FilterNr, uint CritNr )
{
  //get the application config object
  config = KApplication::kApplication()->config();

  //save numbers
  FilterNumber = FilterNr;
  CriteriaNumber = CritNr;

  //get setup
  //---------

  //set group
  config->setGroup( QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( FilterNr ) );

  //get source
  switch( config->readNumEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_SOURCE ).arg( CritNr ), DEFAULT_FILTER_CRITERIA_SOURCE ) )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM       : source = SrcFrom; break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO         : source = SrcTo; break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT    : source = SrcSubject; break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE       : source = SrcSize; break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER     : source = SrcHeader; break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT    : source = SrcAccount; break;
    default                                             : source = SrcFrom; break;
  }

  //get condition
  if( source == SrcSize )
  {
    switch( config->readNumEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CONDITION ).arg( CritNr ), DEFAULT_FILTER_CRITERIA_COND_NUM ) )
    {
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL          : numCondition = NumCondEqual; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL      : numCondition = NumCondNotEqual; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER        : numCondition = NumCondGreater; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL  : numCondition = NumCondGreaterEqual; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS           : numCondition = NumCondLess; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL     : numCondition = NumCondLessEqual; break;
      default                                                   : numCondition = NumCondGreater; break;
    }
  }
  else
  {
    switch( config->readNumEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CONDITION ).arg( CritNr ), DEFAULT_FILTER_CRITERIA_COND_TEXT ) )
    {
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : txtCondition = TxtCondContains; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : txtCondition = TxtCondNotContains; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : txtCondition = TxtCondEqual; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : txtCondition = TxtCondNotEqual; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : txtCondition = TxtCondRegExpr; break;
      case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : txtCondition = TxtCondNotRegExpr; break;
      default                                                   : txtCondition = TxtCondContains; break;
    }

    cs = config->readBoolEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_CASESENSITIVE ).arg( CritNr ), DEFAULT_FILTER_CRITERIA_CASE_SENSITIVE );
  }

  //get Value
  if( source == SrcSize )
  {
    numValue = config->readUnsignedNumEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( CritNr ) );
  }
  else
  {
    txtValue = config->readEntry( QString( "%1%2" ).arg( CONFIG_ENTRY_FILTER_CRITERIA_VALUE ).arg( CritNr ) );
  }

}

FilterItemCriteria::~FilterItemCriteria()
{
}

bool FilterItemCriteria::check( QString from, QString to, uint size, QString subject, QString header, QString account ) const
{
  switch( source )
  {
    case SrcFrom    : return checkText( from ); break;
    case SrcTo      : return checkText( to ); break;
    case SrcHeader  : return checkText( header ); break;
    case SrcSize    : return checkNum( size ); break;
    case SrcSubject : return checkText( subject ); break;
    case SrcAccount : return checkText( account ); break;
    default         : return false;
  }

  //this is the default
  return false;
}

bool FilterItemCriteria::checkText( QString value ) const
{
  //return false if the source is not text
  if( source != SrcFrom && source != SrcTo && source != SrcHeader && source != SrcSubject && source != SrcAccount )
  {
    kdError() << "FilterItemCriteria::checkText: The source is not text. (Filter " << FilterNumber << ", Criteria " << CriteriaNumber << ")" << endl;
    return false;
  }

  //return false if given value is not valid
  if( value.isNull() || value.isEmpty() )
  {
    kdError() << "FilterItemCriteria::checkText: The given value is empty or null. (Filter " << FilterNumber << ", Criteria " << CriteriaNumber << ")" << endl;
    return false;
  }

  //return false if the value of this criteria is an empty string
  if( txtValue.isEmpty() || txtValue.isNull() )
  {
    kdWarning() << "There is no value for Filter " << FilterNumber << "/Criteria " << CriteriaNumber << endl;
  }

  switch( txtCondition )
  {
    case TxtCondContains        : return value.contains( txtValue, cs ) > 0; break;

    case TxtCondNotContains     : return value.contains( txtValue, cs ) < 1; break;

    case TxtCondEqual           : if( cs )
                                  {
                                    return value.localeAwareCompare( txtValue ) == 0;
                                  }
                                  else
                                  {
                                    return QString::localeAwareCompare( txtValue.upper(), value.upper() ) == 0;
                                  }
                                  break;

    case TxtCondNotEqual        : if( cs )
                                    return value.localeAwareCompare( txtValue ) != 0;
                                  else
                                    return value.localeAwareCompare( txtValue.upper(), value.upper() ) != 0;
                                  break;

    case TxtCondRegExpr         : return value.find( QRegExp( txtValue ) ) != -1; break;

    case TxtCondNotRegExpr      : return value.find( QRegExp( txtValue ) ) == -1; break;

    default                     : return false;
  }

  //default
  return false;
}

bool FilterItemCriteria::checkNum( uint value ) const
{
  //return false if the source is not numeric
  if ( source != SrcSize )
  {
    kdError() << "FilterItemCriteria::checkNum: The source is not numeric." << endl;
    return false;
  }

  switch( numCondition )
  {
    case NumCondEqual         : return value == numValue; break;
    case NumCondNotEqual      : return value != numValue; break;
    case NumCondGreater       : return value > numValue; break;
    case NumCondGreaterEqual  : return value >= numValue; break;
    case NumCondLess          : return value < numValue; break;
    case NumCondLessEqual     : return value <= numValue; break;
    default                   : kdError() << "FilterItemCriteria::checkNum: Unknown numeric condiction. Return false." << endl;
  }

  //default
  return false;
}

void FilterItemCriteria::print( ) const
{
  QString output;

  output.append( QString( "Criteria %1 of Filter %2: Source: " ).arg( CriteriaNumber ).arg( FilterNumber ) );

  switch( source )
  {
    case SrcFrom      : output.append( "From" ); break;
    case SrcTo        : output.append( "To" ); break;
    case SrcAccount   : output.append( "Account" ); break;
    case SrcHeader    : output.append( "Header" ); break;
    case SrcSize      : output.append( "Size" ); break;
    case SrcSubject   : output.append( "Subject" ); break;
    default           : output.append( "unknown" ); break;
  }

  output.append( "; Condition: " );

  if( source == SrcSize )
  {
    switch( numCondition )
    {
      case NumCondEqual         : output.append( QString( "equals %1" ).arg( numValue ) ); break;
      case NumCondGreater       : output.append( QString( "greater than %1" ).arg( numValue ) ); break;
      case NumCondGreaterEqual  : output.append( QString( "greater than or equals %1" ).arg( numValue ) ); break;
      case NumCondLess          : output.append( QString( "less than %1" ).arg( numValue ) ); break;
      case NumCondLessEqual     : output.append( QString( "less than or equals %1" ).arg( numValue ) ); break;
      case NumCondNotEqual      : output.append( QString( "not equals %1" ).arg( numValue ) ); break;
      default                   : output.append( QString( "unknown; Value: %1" ).arg( numValue ) ); break;
    }
  }
  else
  {
    switch( txtCondition )
    {
      case TxtCondContains    : output.append( QString( "contains %1" ).arg( txtValue ) ); break;
      case TxtCondEqual       : output.append( QString( "equals %1" ).arg( txtValue ) ); break;
      case TxtCondNotContains : output.append( QString( "not contains %1" ).arg( txtValue ) ); break;
      case TxtCondNotEqual    : output.append( QString( "not equals %1" ).arg( txtValue ) ); break;
      case TxtCondRegExpr     : output.append( QString( "matches to regular expression %1" ).arg( txtValue ) ); break;
      case TxtCondNotRegExpr  : output.append( QString( "doesn't match to regular expression %1" ).arg( txtValue ) ); break;
      default                 : output.append( QString( "unknown; Value: %1" ).arg( txtValue ) ); break;
    }

    if( txtCondition != TxtCondRegExpr && txtCondition != TxtCondNotRegExpr )
    {
      if( cs )
        output.append( " (case sensitive)" );
      else
        output.append( " (not case sensitive)" );
    }
  }

  kdDebug() << output << endl;
}



