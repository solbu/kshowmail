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

#include "filterlogviewmovedmodel.h"

FilterLogViewMovedModel::FilterLogViewMovedModel( QObject* parent, FilterLog* log ): QAbstractItemModel( parent )
{
  this->log = log;

  //set defaults
  lastSortOrder = Qt::AscendingOrder;
  lastSortColumn = 0;

  //load data
  list.append( log->getMovedMails() );

}

QVariant FilterLogViewMovedModel::data( const QModelIndex& index, int role ) const
{
  //return a empty data if the index is invalid
  if( !index.isValid() ) return QVariant();

  if( index.row() > rowCount() || index.column() > NUMBER_VIEWMOVED_COLUMNS - 1 ) return QVariant();

  if( index.row() > list.size() - 1 ) return QVariant();

  //get log entry
  FilterLogEntry entry = list.at( index.row() );

  switch( role ) {

    case( Qt::DisplayRole ) : {

      switch( index.column() ) {

        case 0 : return QVariant( entry.getDate().toString( KDateTime::LocalDate) ); break;
        case 1 : return QVariant( entry.getSender() ); break;
        case 2 : return QVariant( entry.getAccount() ); break;
        case 3 : return QVariant( entry.getMailbox() ); break;
        case 4 : return QVariant( entry.getSubject() ); break;
        default : return QVariant(); break;
      }
    }

    default : return QVariant();

  }


}

int FilterLogViewMovedModel::columnCount(const QModelIndex& ) const
{
  return NUMBER_VIEWMOVED_COLUMNS;
}

int FilterLogViewMovedModel::rowCount( const QModelIndex& parent ) const
{
  //return 0, if the parent is valid
  if( parent.isValid() ) return 0;

  return list.size();

}

QModelIndex FilterLogViewMovedModel::parent( const QModelIndex& ) const
{
  return QModelIndex();
}

QModelIndex FilterLogViewMovedModel::index(int row, int column, const QModelIndex& parent) const
{
  //returns a invalid index if the parent index is valid
  //because no index has a child
  if( parent.isValid() ) return QModelIndex();

  return createIndex( row, column );

}

QVariant FilterLogViewMovedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  //we just returns a header text for the display role and a horizontal orientation
  if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
         return QVariant();

  switch( section )
  {
    case 0  : return QVariant( i18nc( "@title:column send date", "Date" ) ); break;
    case 1  : return QVariant( i18nc( "@title:column sender of the mail", "Sender" ) ); break;
    case 2  : return QVariant( i18nc( "@title:column account name", "Account" ) ); break;
    case 3  : return QVariant( i18nc( "@title:column name of the mailbox where the mail was written", "Moved To" ) ); break;
    case 4  : return QVariant( i18nc( "@title:column mail subject", "Subject" ) ); break;
    default : return QVariant();
  }
}

void FilterLogViewMovedModel::sort(int column, Qt::SortOrder order)
{

  //save last sort properties
  lastSortOrder = order;
  lastSortColumn = column;

  if( list.isEmpty() ) return;

  //resolve sorting property
  LogViewSort prop;
  switch( column ) {

    case 0 : prop = LogViewSortDate; break;
    case 1 : prop = LogViewSortFrom; break;
    case 2 : prop = LogViewSortAccount; break;
    case 3 : prop = LogViewSortSubject; break;
    default : prop = LogViewSortDate; break;
  }

  //sort the view list
  QList<FilterLogEntry> sortedList;

  QListIterator<FilterLogEntry> itUnsort( list );
  while( itUnsort.hasNext() ) {

    //get next entry
    FilterLogEntry entryUnsorted = itUnsort.next();

    //find a place for it in the temporary list
    if( sortedList.size() == 0 ) {

      sortedList.append( entryUnsorted );

    } else {

      int sizeSortedList = sortedList.size();
      int indexSort = 0;
      bool placed = false;
      while( indexSort < sizeSortedList && !placed ) {

        //get the entry in the sorted List
        FilterLogEntry entrySorted = sortedList.at( indexSort );

        //is the entry from the unsorted list lesser (greater) than the entry from the sorted list insert the first one at this
        //position into the sorted list and break the searching for place
        if( order == Qt::AscendingOrder ) {

          if( entryUnsorted.compare( entrySorted, prop ) <= 0 ) {

            sortedList.insert( indexSort, entryUnsorted );
            placed = true;
          }

        } else {

          if( entryUnsorted.compare( entrySorted, prop ) > 0 ) {

            sortedList.insert( indexSort, entryUnsorted );
            placed = true;
          }
        }

        indexSort++;
      }

      //if the entry could not placed, we append it at the end
      if( !placed )
        sortedList.append( entryUnsorted );
    }
  }

  //switch the lists
  list.clear();
  list.append( sortedList );

  reset();
}

void FilterLogViewMovedModel::sort()
{
  sort( lastSortColumn, lastSortOrder );
}

void FilterLogViewMovedModel::refresh()
{
  list.clear();
  list.append( log->getMovedMails() );
  sort();
  reset();
}

void FilterLogViewMovedModel::saveSetup()
{
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

  conf->writeEntry( CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_MOVED, lastSortColumn );

  if( lastSortOrder == Qt::AscendingOrder ) {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_MOVED, CONFIG_VALUE_SORT_ORDER_ASCENDING );

  } else {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_MOVED, CONFIG_VALUE_SORT_ORDER_DESCENDING );
  }

  conf->sync();


}

