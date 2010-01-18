//
// C++ Implementation: filterlog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterlog.h"

FilterLog::FilterLog()
{
  //get the application config object
  config = KGlobal::config();

  //load the setup
  loadSetup();

  //load stored entries
  load();
}


FilterLog::~FilterLog()
{
}

void FilterLog::addDeletedMail(const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject)
{
  if( logDeletedMails )
    addEntry( FActDelete, dateTime, sender, account, subject, "" );
}

void FilterLog::addMovedMail(const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject, const QString & mailbox)
{
  if( logMovedMails )
    addEntry( FActMove, dateTime, sender, account, subject, mailbox );
}

void FilterLog::addEntry(FilterAction_Type action, const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject, const QString & mailbox)
{
  //create entry
  FilterLogEntry entry = FilterLogEntry( action, dateTime, sender, account, subject, mailbox );

  //add entry to the appropriate list
  switch( action )
  {
    case FActDelete   : listDeletedMails.append( entry ); break;
    case FActMove     : listMovedMails.append( entry ); break;
    default           : kdError() << "FilterLog::addEntry: Could not relate the following mail:" << endl;
                        entry.print();
                        break;
  }
}

void FilterLog::print()
{
  kdDebug() << "Log state:" << endl;
  kdDebug() << "----------" << endl;

  //print all entries about deleted mails
  kdDebug() << "Deleted mails:" << endl;
  LogEntryList::iterator it;
  for ( it = listDeletedMails.begin(); it != listDeletedMails.end(); ++it )
    (*it).print();

  kdDebug() << endl;

  //print all entries about moved mails
  kdDebug() << "Moved mails:" << endl;
  for ( it = listMovedMails.begin(); it != listMovedMails.end(); ++it )
    (*it).print();
}

void FilterLog::clearDeletedMailsLog()
{
  listDeletedMails.clear();
}

void FilterLog::clearMovedMailsLog()
{
  listMovedMails.clear();
}

void FilterLog::save()
{
  //maybe we have to remove old entries, calculate minimum date
  KDateTime minTime = KDateTime::currentLocalDateTime();
  minTime = minTime.addDays( daysStoreDeletedMails * -1 );

  //we need a XML document
  QDomDocument doc( LOG_DOCTYPE );

  //and a root element
  QDomElement rootElem = doc.createElement( LOG_ROOT_ELEMENT );
  doc.appendChild( rootElem );

  //store the entries of the deleted mails list into the document
  //if the user want it
  if( deletedMailsStorageMode != exit )
  {
    LogEntryList::iterator it;
    for ( it = listDeletedMails.begin(); it != listDeletedMails.end(); ++it )
    {
      if( (*it).getDate() >= minTime )
        (*it).save( doc, rootElem );
    }
  }


  //get the name of the file to save
  QString filename = KStandardDirs::locateLocal( "appdata", LOG_FILE );

  //and save
  QFile file( filename );

  if ( file.open( QFile::WriteOnly ) ) //open file
  {
    QTextStream stream( &file );
    doc.save( stream, 2 );
    file.close();
  }
  else
  {
    KMessageBox::error( NULL, i18n( "Could not save the filter log." ) );
  }
}

void FilterLog::load()
{
  //maybe we have to remove old entries, calculate minimum date
  KDateTime minTime = KDateTime::currentLocalDateTime();
  minTime = minTime.addDays( daysStoreDeletedMails * -1 );

  //we need a XML document
  QDomDocument doc( LOG_DOCTYPE );

  //get the name of the file
  QString filename = KStandardDirs::locateLocal( "appdata", LOG_FILE );

  //load the log from file into the DOM document
  QFile file( filename );

  if ( !file.open( QFile::ReadOnly ) )
    return;         //return, if the file can't opened

  if ( !doc.setContent( &file ) ) {
    file.close();   //return, if the content of the file is invalid
    return;
  }

  //the content was loaded, close the file
  file.close();

  //iterate over all DOM elements and generate the log entries
  QDomElement docElem = doc.documentElement();  //get root element

  //return, if the root element is not LOG_ROOT_ELEMENT
  if( docElem.tagName() != LOG_ROOT_ELEMENT ) return;

  QDomNode n = docElem.firstChild();  //get first child (this is the first log entry)
  while( !n.isNull() )
  {
    QDomElement e = n.toElement();   //try to convert the node to an element.
    if( !e.isNull() )
    {
      if( e.tagName() == LOG_ENTRY_ELEMENT )
      {
        //add the read entry to the list of deleted mails
        KDateTime mailTime = KDateTime::fromString( e.attribute( LOG_ENTRY_ATTRIBUTE_DATETIME ), KDateTime::ISODate );
        if( mailTime >= minTime )
          addDeletedMail( mailTime,
                          e.attribute( LOG_ENTRY_ATTRIBUTE_SENDER ),
                          e.attribute( LOG_ENTRY_ATTRIBUTE_ACCOUNT ),
                          e.attribute( LOG_ENTRY_ATTRIBUTE_SUBJECT ) );
      }
    }
    n = n.nextSibling();            //get next child
  }
}

LogEntryList FilterLog::getDeletedMails( )
{
  return listDeletedMails;
}

LogEntryList FilterLog::getMovedMails( )
{
  return listMovedMails;
}

void FilterLog::loadSetup( )
{
  KConfigGroup* configLog = new KConfigGroup( config, CONFIG_GROUP_LOG );

  logDeletedMails = configLog->readEntry( CONFIG_ENTRY_LOG_LOG_DELETED_MAILS, DEFAULT_LOG_LOG_DELETED_MAILS );
  logMovedMails = configLog->readEntry( CONFIG_ENTRY_LOG_LOG_MOVED_MAILS, DEFAULT_LOG_LOG_MOVED_MAILS );

  if( logDeletedMails )
  {
    QString storageMode = configLog->readEntry(CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS, DEFAULT_LOG_REMOVE_DELETED_MAILS );
    if( storageMode == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
      deletedMailsStorageMode = exit;
    else if( storageMode == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
      deletedMailsStorageMode = days;
    else
      deletedMailsStorageMode = days;

    if( deletedMailsStorageMode == days )
      daysStoreDeletedMails = configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_DELETED_MAILS );
    else
      daysStoreDeletedMails = 7;
  }
  else
  {
    deletedMailsStorageMode = days;
    daysStoreDeletedMails = 7;
  }
}

int FilterLog::numberDeletedMails( )
{
  return listDeletedMails.count();
}

int FilterLog::numberMovedMails( )
{
  return listMovedMails.count();
}



