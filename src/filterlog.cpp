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

void FilterLog::addDeletedMail(const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject, KindOfMailDeleting kindDelete, const QString& filter )
{
  if( logDeletedMails )
    addEntry( FActDelete, dateTime, sender, account, subject, "", kindDelete, filter );
}

void FilterLog::addMovedMail(const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject, const QString & mailbox)
{
  if( logMovedMails )
    addEntry( FActMove, dateTime, sender, account, subject, mailbox, DelFilter );
}

void FilterLog::addEntry(FilterAction_Type action, const KDateTime & dateTime, const QString & sender, const QString & account, const QString & subject, const QString & mailbox, KindOfMailDeleting kindDelete, const QString& filter )
{
  //create entry
  FilterLogEntry entry = FilterLogEntry( action, dateTime, sender, account, subject, mailbox, kindDelete, filter );

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
  KDateTime minTimeDelete = KDateTime::currentLocalDateTime();
  minTimeDelete = minTimeDelete.addDays( daysStoreDeletedMails * -1 );
  KDateTime minTimeManualDelete = KDateTime::currentLocalDateTime();
  minTimeManualDelete = minTimeManualDelete.addDays( daysStoreManualDeletedMails * -1 );

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
      if( (*it).getKindOfDeleting() == DelFilter ) {

        if( (*it).getDate() >= minTimeDelete )
          (*it).save( doc, rootElem );

      }
    }
  }

  //store the entries of the manual deleted mails list into the document
  //if the user want it
  if( manualDeletedMailsStorageMode != exit )
  {
    LogEntryList::iterator it;
    for ( it = listDeletedMails.begin(); it != listDeletedMails.end(); ++it )
    {
      if( (*it).getKindOfDeleting() == DelManual ) {
        if( (*it).getDate() >= minTimeManualDelete )
          (*it).save( doc, rootElem );

      }
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
  KDateTime minTimeDeleted = KDateTime::currentLocalDateTime();
  minTimeDeleted = minTimeDeleted.addDays( daysStoreDeletedMails * -1 );
  KDateTime minTimeManualDeleted = KDateTime::currentLocalDateTime();
  minTimeManualDeleted = minTimeManualDeleted.addDays( daysStoreManualDeletedMails * -1 );

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
        //get the kind of deleting
        KindOfMailDeleting kindDel;
        if( e.attribute( LOG_ENTRY_ATTRIBUTE_KIND_DELETE ) == LOG_ENTRY_VALUE_KIND_DELETE_MANUAL ) {
          kindDel = DelManual;
        } else {
          kindDel = DelFilter;
        }

        //add the read entry to the list of deleted mails
        //if the entry is to old, we don't add it.
        KDateTime mailTime = KDateTime::fromString( e.attribute( LOG_ENTRY_ATTRIBUTE_DATETIME ), KDateTime::ISODate );
        if( kindDel == DelManual ) {

          if( mailTime >= minTimeManualDeleted ) {


            addDeletedMail( mailTime,
                            e.attribute( LOG_ENTRY_ATTRIBUTE_SENDER ),
                            e.attribute( LOG_ENTRY_ATTRIBUTE_ACCOUNT ),
                            e.attribute( LOG_ENTRY_ATTRIBUTE_SUBJECT ),
                            kindDel
            );
          }

        } else {

          if( mailTime >= minTimeDeleted ) {


            addDeletedMail( mailTime,
                            e.attribute( LOG_ENTRY_ATTRIBUTE_SENDER ),
                            e.attribute( LOG_ENTRY_ATTRIBUTE_ACCOUNT ),
                            e.attribute( LOG_ENTRY_ATTRIBUTE_SUBJECT ),
                            kindDel,
                            e.attribute( LOG_ENTRY_ATTRIBUTE_FILTER )
            );
          }
        }

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
  logManualDeletedMails = configLog->readEntry( CONFIG_ENTRY_LOG_LOG_MANUAL_DELETED_MAILS, DEFAULT_LOG_LOG_MANUAL_DELETED_MAILS );

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


  if( logManualDeletedMails )
  {
    QString storageMode = configLog->readEntry(CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS, DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS );
    if( storageMode == CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT )
      manualDeletedMailsStorageMode = exit;
    else if( storageMode == CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS )
      manualDeletedMailsStorageMode = days;
    else
      manualDeletedMailsStorageMode = days;

    if( manualDeletedMailsStorageMode == days )
      daysStoreManualDeletedMails = configLog->readEntry( CONFIG_ENTRY_LOG_HOLDDAYS_MANUAL_DELETED_MAILS, DEFAULT_LOG_HOLDDAYS_MANUAL_DELETED_MAILS );
    else
      daysStoreManualDeletedMails = 7;
  }
  else
  {
    manualDeletedMailsStorageMode = days;
    daysStoreManualDeletedMails = 7;
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



