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

#include "filterlogviewdeletedmodel.h"

FilterLogViewDeletedModel::FilterLogViewDeletedModel( QObject* parent, FilterLog* log ): QAbstractItemModel( parent )
{
  this->log = log;

  //set defaults
  lastSortOrder = Qt::AscendingOrder;
  lastSortColumn = 0;

  //load pictures
  picManualDeleted = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/deletedManual.png" ) );
  picFilterDeleted = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/deletedFilter.png" ) );


  //load data
  list.append( log->getDeletedMails() );

}

QVariant FilterLogViewDeletedModel::data( const QModelIndex& index, int role ) const
{
  //return a empty data if the index is invalid
  if( !index.isValid() ) return QVariant();

  if( index.row() > rowCount() || index.column() > NUMBER_VIEWDELETED_COLUMNS - 1 ) return QVariant();

  if( index.row() > list.size() - 1 ) return QVariant();

  //get log entry
  FilterLogEntry entry = list.at( index.row() );

  switch( role ) {

    case( Qt::DisplayRole ) : {

      switch( index.column() ) {

        case 1 : return QVariant( entry.getDate().toString( KDateTime::LocalDate) ); break;
        case 2 : return QVariant( entry.getSender() ); break;
        case 3 : return QVariant( entry.getAccount() ); break;
        case 4 : return QVariant( entry.getSubject() ); break;
        default : return QVariant(); break;
      }

      break;
    }

    case( Qt::DecorationRole ) : {

      switch( index.column() )
      {
        case 0  :

          if( entry.getKindOfDeleting() == DelManual )
          {
            return QVariant( picManualDeleted );
          }
          else
          {
            return QVariant( picFilterDeleted );
          }

          break;

        default : return QVariant();
      }

      break;
    }

    case( Qt::ToolTipRole ) : {

      switch( index.column() ) {

        case 0 : {

          if( entry.getKindOfDeleting() == DelManual ) {
            return QVariant( i18nc( "@Info:tooltip Filter-Log: The mail was manually deleted", "This mail was manually deleted.") );
          } else {
            return QVariant( i18nc( "@Info:tooltip Filter-Log: The mail was deleted by the filter", "This mail was deleted by filter.") );
          }

          break;
        }

        default: return QVariant();
      }

      break;
    }

    default : return QVariant();

  }


}

int FilterLogViewDeletedModel::columnCount(const QModelIndex& ) const
{
  return NUMBER_VIEWDELETED_COLUMNS;
}

int FilterLogViewDeletedModel::rowCount( const QModelIndex& parent ) const
{
  //return 0, if the parent is valid
  if( parent.isValid() ) return 0;

  return list.size();

}

QModelIndex FilterLogViewDeletedModel::parent( const QModelIndex& ) const
{
  return QModelIndex();
}

QModelIndex FilterLogViewDeletedModel::index(int row, int column, const QModelIndex& parent) const
{
  //returns a invalid index if the parent index is valid
  //because no index has a child
  if( parent.isValid() ) return QModelIndex();

  return createIndex( row, column );

}

QVariant FilterLogViewDeletedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  //we just returns a header text for the display role and a horizontal orientation
  if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
         return QVariant();

  switch( section )
  {
    case 0  : return QVariant();      //this column shows a small icon about the mails was deleted by filter or manual
    case 1  : return QVariant( i18nc( "@title:column send date", "Date" ) ); break;
    case 2  : return QVariant( i18nc( "@title:column sender of the mail", "Sender" ) ); break;
    case 3  : return QVariant( i18nc( "@title:column account name", "Account" ) ); break;
    case 4  : return QVariant( i18nc( "@title:column mail subject", "Subject" ) ); break;
    default : return QVariant();
  }
}

void FilterLogViewDeletedModel::sort(int column, Qt::SortOrder order)
{

  //save last sort properties
  lastSortOrder = order;
  lastSortColumn = column;

  if( list.isEmpty() ) return;

  //resolve sorting property
  LogViewSort prop;
  switch( column ) {

    case 0 : prop = LogViewSortKind; break;
    case 1 : prop = LogViewSortDate; break;
    case 2 : prop = LogViewSortFrom; break;
    case 3 : prop = LogViewSortAccount; break;
    case 4 : prop = LogViewSortSubject; break;
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

void FilterLogViewDeletedModel::sort()
{
  sort( lastSortColumn, lastSortOrder );
}

void FilterLogViewDeletedModel::refresh()
{
  list.clear();
  list.append( log->getDeletedMails() );
  sort();
  reset();
}

void FilterLogViewDeletedModel::saveSetup()
{
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

  conf->writeEntry( CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_DELETED, lastSortColumn );

  if( lastSortOrder == Qt::AscendingOrder ) {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_DELETED, CONFIG_VALUE_SORT_ORDER_ASCENDING );

  } else {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_LOGVIEW_DELETED, CONFIG_VALUE_SORT_ORDER_DESCENDING );
  }

  conf->sync();

}
