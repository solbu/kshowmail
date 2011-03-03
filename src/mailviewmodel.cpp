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

#include "./mailviewmodel.h"

MailViewModel::MailViewModel( AccountList* accounts, QObject* parent ) : QAbstractItemModel( parent )
{
  this->accounts = accounts;

  //load pictures
  picNewMail = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/mail-unread-new.png" ) );
  picSenderOnWhitelist = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/senderOnWhitelist.png" ) );

}

MailViewModel::~MailViewModel(){}

QModelIndex MailViewModel::index( int row, int column, const QModelIndex& parent ) const
{
	//returns a invalid index if the parent index is valid
	//because no index has a child
	if( parent.isValid() ) return QModelIndex();

	return createIndex( row, column );
}

QModelIndex MailViewModel::parent( const QModelIndex& ) const
{
	return QModelIndex();
}

int MailViewModel::rowCount ( const QModelIndex & parent ) const
{
	//return 0, if the parent is valid
	if( parent.isValid() ) return 0;

	return viewMailList.size();
}

int MailViewModel::columnCount ( const QModelIndex&  ) const
{
	return NUMBER_MAILVIEW_COLUMNS;
}

QVariant MailViewModel::data ( const QModelIndex & index, int role ) const
{
	//return a empty data if the index is invalid
	if( !index.isValid() ) return QVariant();

	if( index.row() >= rowCount() || index.column() > NUMBER_MAILVIEW_COLUMNS - 1 ) return QVariant();

	if( index.row() > viewMailList.size() - 1 ) return QVariant();

  //get the mail
  Mail mail = viewMailList.at( index.row() );

  //the kind of data we return is dependent on the given role
  switch( role )
  {
    case( Qt::DisplayRole ) :

      switch( index.column() )
      {
        case  0 : return QVariant(); break;
        case  1 : return QVariant( (int)mail.getNumber() ); break;
        case  2 : return QVariant( mail.getAccountName() ); break;
        case  3 : return QVariant( mail.getFrom() ); break;
        case  4 : return QVariant( mail.getTo() ); break;
        case  5 : return QVariant( mail.getSubject() ); break;
        case  6 : return QVariant( mail.getDateTime().toString( KDateTime::LocalDate ) ); break;
        case  7 : return QVariant( mail.getSizeSuffix() ); break;
        case  8 : return QVariant( mail.getContent() ); break;
        default : return QVariant();
      }
      break;

    case( Qt::DecorationRole ):

      switch( index.column() )
      {
        //new mail icon
        case 0  :
          if( mail.isNew() )
          {
            return QVariant( picNewMail );
          }
          else
          {
            return QVariant();
          }
          break;

        //sender
        //we show a icon to indicate the sender is on the white list
        case 3 : {

          if( senderlist.isOnWhitelist( mail.getFrom() ) ) {
            return QVariant( picSenderOnWhitelist );
          } else {
            return QVariant();
          }

          break;
        }

        default : return QVariant();
      }
      break;

    //the tooltip shows the name of the filter which has chatched the mail
    case ( Qt::ToolTipRole ) :

      QString filtername = mail.getFilterName();
      if( filtername.isEmpty() ) {
        return QVariant();
      } else {
        return QVariant( i18nc( "@info:Shows the filter which has catched this mail", "Caught by filter: %1", mail.getFilterName() ) );
      }

  }

	return QVariant();
}

bool MailViewModel::hasChildren ( const QModelIndex & parent ) const
{
  //it is the root index
  if( !parent.isValid() ) return true;

  //it isn't the root index
  return false;
}

QVariant MailViewModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	//we just returns a header text for the display role and a horizontal orientation
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
         return QVariant();

	switch( section )
	{
    case 0  : return QVariant( i18nc( "@title:column state of the mail", "State" ) ); break;
		case 1	:	return QVariant( i18nc( "@title:column mail number on the server", "Number" ) ); break;
		case 2	:	return QVariant( i18nc( "@title:column name of the account", "Account" ) ); break;
		case 3	:	return QVariant( i18nc( "@title:column sender", "From" ) ); break;
		case 4	:	return QVariant( i18nc( "@title:column addressee", "To" ) ); break;
		case 5	:	return QVariant( i18nc( "@title:column mail subject", "Subject" ) ); break;
		case 6	:	return QVariant( i18nc( "@title:column send date", "Date" ) ); break;
		case 7	:	return QVariant( i18nc( "@title:column mail size", "Size" ) ); break;
		case 8	:	return QVariant( i18nc( "@title:column content type", "Content" ) ); break;
		default : return QVariant();
	}
}

Qt::ItemFlags MailViewModel::flags ( const QModelIndex& ) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MailViewModel::refresh()
{
	viewMailList.clear();
	viewMailList.append( accounts->getAllMails() );
	sort();
  reset();
}

void MailViewModel::sort( int column, Qt::SortOrder order ) {

  //save last sort properties
  lastSortOrder = order;
  lastSortColumn = column;

  if( viewMailList.empty() ) return;


  MailSort_Type prop;
  switch( column ) {

    case 0  : prop = MailSortState; break;
    case 1  : prop = MailSortNumber; break;
    case 2  : prop = MailSortAccount; break;
    case 3  : prop = MailSortFrom; break;
    case 4  : prop = MailSortTo; break;
    case 5  : prop = MailSortSubject; break;
    case 6  : prop = MailSortDate; break;
    case 7  : prop = MailSortSize; break;
    case 8  : prop = MailSortContent; break;
    default : prop = MailSortAccount;
  }

  //sort the view list
  QList<Mail> sortedList;

  QListIterator<Mail> itUnsort( viewMailList );
  while( itUnsort.hasNext() ) {

    //get next account
    Mail mailUnsorted = itUnsort.next();

    //find a place for it in the temporary list
    if( sortedList.size() == 0 ) {

      sortedList.append( mailUnsorted );

    } else {

      int sizeSortedList = sortedList.size();
      int indexSort = 0;
      bool placed = false;
      while( indexSort < sizeSortedList && !placed ) {

        //get the mail in the sorted List
        Mail mailSorted = sortedList.at( indexSort );

        //is the mail from the unsorted list lesser (greater) than the mail from the sorted list insert the first one at this
        //position into the sorted list and break the searching for place
        if( order == Qt::AscendingOrder ) {

          if( mailUnsorted.compare( mailSorted, prop ) <= 0 ) {

            sortedList.insert( indexSort, mailUnsorted );
            placed = true;
          }

        } else {

          if( mailUnsorted.compare( mailSorted, prop ) > 0 ) {

            sortedList.insert( indexSort, mailUnsorted );
            placed = true;
          }
        }

        indexSort++;
      }

      //if the mail could not placed, we append it at the end
      if( !placed )
        sortedList.append( mailUnsorted );
    }
  }

  //switch the lists
  viewMailList.clear();
  viewMailList.append( sortedList );

	reset();
}

void MailViewModel::sort()
{
	sort( lastSortColumn, lastSortOrder );
}

Mail MailViewModel::getMail(const QModelIndex index) const
{
	return viewMailList.at( index.row() );
}

QStringList MailViewModel::getSelectedSubjects( QItemSelectionModel* selectModel ) const
{
  QStringList subjects;

  if( !selectModel->hasSelection() ) return subjects;

    //get selected rows
  QModelIndexList indexList = selectModel->selectedRows();

  //get the mail of every selected row an store the subject in the result list
  QListIterator<QModelIndex> it( indexList );
  while( it.hasNext() )
  {
    //get Index
    QModelIndex index = it.next();

    //get mail
    Mail mail = getMail( index );

    //store subject
    subjects.append( mail.getSubject() );
  }

  return subjects;
}

QStringList MailViewModel::getSelectedSenders( QItemSelectionModel* selectModel ) const
{
  QStringList senders;

  if( !selectModel->hasSelection() ) return senders;

    //get selected rows
  QModelIndexList indexList = selectModel->selectedRows();

  //get the mail of every selected row an store the subject in the result list
  QListIterator<QModelIndex> it( indexList );
  while( it.hasNext() )
  {
    //get Index
    QModelIndex index = it.next();

    //get mail
    Mail mail = getMail( index );

    //store subject
    senders.append( mail.getFrom() );
  }

  return senders;
}

QList<Mail> MailViewModel::getSelectedMails( QItemSelectionModel* mailSelectModel ) const
{
  QList<Mail> list;  //result list

  //get selected rows
  QModelIndexList indexList = mailSelectModel->selectedRows();

  //get the mail of every selected row an store the pointer into the result list
  QListIterator<QModelIndex> it( indexList );
  while( it.hasNext() )
  {
    //get Index
    QModelIndex index = it.next();

    //get mail
    Mail mail = getMail( index );

    //store mail pointer
    list.append( mail );
  }

  return list;
}

QModelIndexList MailViewModel::getMarkedMails() const
{
  QModelIndexList indexList;

  for( int i = 0; i < rowCount(); i++ ) {

    //get index
    QModelIndex ix = index( i, 0 );

    //get mail
    Mail mail = getMail( ix );

    //add the index to the list if this mail is marked
    if( mail.isMarkedByFilter() ) {

      indexList.append( ix );
    }
  }

  return indexList;
}

void MailViewModel::saveSetup()
{
  KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );

  conf->writeEntry( CONFIG_ENTRY_SORT_COLUMN_MESSAGE, lastSortColumn );

  if( lastSortOrder == Qt::AscendingOrder ) {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_MESSAGE, CONFIG_VALUE_SORT_ORDER_ASCENDING );

  } else {

    conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_MESSAGE, CONFIG_VALUE_SORT_ORDER_DESCENDING );
  }

  conf->sync();
}

void MailViewModel::loadSetup()
{
  senderlist.load();
}


