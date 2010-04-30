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

#include "./accountviewmodel.h"

AccountViewModel::AccountViewModel( AccountList* accounts, QObject* parent ) : QAbstractItemModel( parent )
{
	this->accounts = accounts;

  //load pictures for a active or inactive account
  picActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountActive.png" ) );
  picNotActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountNotActive.png" ) );

  //store all account pointers into the view list
  refresh();

}

AccountViewModel::~AccountViewModel(){}

QModelIndex AccountViewModel::index( int row, int column, const QModelIndex& parent ) const
{
	//returns a invalid index if the parent index is valid
	//because no index has a child
	if( parent.isValid() ) return QModelIndex();
	
	return createIndex( row, column );
}

QModelIndex AccountViewModel::parent( const QModelIndex& ) const
{
	return QModelIndex();
}

int AccountViewModel::rowCount ( const QModelIndex & parent ) const 
{
	//return 0, if the parent is valid
	if( parent.isValid() ) return 0;

	return viewAccountList.size();
}

int AccountViewModel::columnCount ( const QModelIndex & ) const
{
	return NUMBER_ACCOUNTVIEW_COLUMNS;
}

QVariant AccountViewModel::data ( const QModelIndex & index, int role ) const
{  
	//return a empty data if the index is invalid
	if( !index.isValid() ) return QVariant();
	
	if( index.row() > rowCount() || index.column() > NUMBER_ACCOUNTVIEW_COLUMNS - 1 ) return QVariant();

  if( index.row() > viewAccountList.size() - 1 ) return QVariant();
  
  //get the account view item
  AccountViewItem acc = viewAccountList.at( index.row() );


  //the kind of data we return is dependent on the given role
  switch( role )
  {
    case( Qt::DisplayRole ) :

      switch( index.column() )
      {
				case 0  : return QVariant(); break;
        case 1  : return QVariant( acc.getName() ); break;
        case 2  : return QVariant( acc.getServer() ); break;
        case 3  : return QVariant( acc.getUser() ); break;
        case 4  : return QVariant( acc.getNumberMails() ); break;
        case 5  : return QVariant( acc.getTotalSizeUnit() ); break;
        default : return QVariant(); break;
      }
      break;

    case( Qt::DecorationRole ):

      switch( index.column() )
      {
        case 0  :
          if( acc.isActive() )
          {
            return QVariant( picActive );
          }
          else
          {
            return QVariant( picNotActive );
          }
          break;

        default : return QVariant();
      }
			break;
			
		case( Qt::EditRole ):
		
			switch( index.column() )
			{
				case 0 :
					if( acc.isActive() )
						return QVariant( true );
					else
						return QVariant( false );
					
					break;
					
				default :
					return QVariant();
					break;
			}
			break;
    
  }
  
	return QVariant();
}

bool AccountViewModel::hasChildren ( const QModelIndex & parent ) const
{
  //it is the root index
  if( !parent.isValid() ) return true;

  //it isn't the root index
	return false;
}

Qt::ItemFlags AccountViewModel::flags ( const QModelIndex & index ) const
{
  if( !index.isValid() ) return 0;
  
	//the first column is editable, the others are just readable
	if( index.column() == 0 )
	{
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant AccountViewModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	//we just returns a header text for the display role and a horizontal orientation
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
         return QVariant();

	switch( section )
	{
		case 0	:	return QVariant( i18nc( "whether the account is active or not", "Active" ) ); break;
		case 1	:	return QVariant( i18n( "Account" ) ); break;
		case 2	:	return QVariant( i18n( "Server" ) ); break;
		case 3	:	return QVariant( i18n( "User" ) ); break;
		case 4	:	return QVariant( i18n( "Messages" ) ); break;
		case 5	:	return QVariant( i18n( "Size" ) ); break;
		default : return QVariant();
	}
}

bool AccountViewModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	if( role != Qt::EditRole ) return false;
	if( !index.isValid() ) return false;
	if( index.column() != 0 ) return false;
	if( index.row() > rowCount( ) ) return false;
	
  if( index.row() > viewAccountList.size() - 1 ) return false;

  //get the account object
  AccountViewItem item = viewAccountList.at( index.row() );
  QPointer<Account> acc = item.getAccount();
  
  if( acc.isNull() ) return false;
	
	//set active state of the account
	acc->setActive( value.toBool() );

  //write it into the application config
  KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), acc->getName() );
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, value.toBool() );
  configAcc->sync();
	
	//emit signal of changed data
	emit dataChanged( index, index );
	
	//successful return
	return true;


}

void AccountViewModel::refresh()
{
  viewAccountList.clear();
  viewAccountList.append( accounts->getAllAccountViewItems() );
	sort();
  reset(); 
}

void AccountViewModel::sort( int column, Qt::SortOrder order ) {

	//save last sort properties
	lastSortOrder = order;
	lastSortColumn = column;
	
  if( viewAccountList.empty() ) return;

  
	AccountSort_Type prop;
	switch( column ) {
	
		case 0	: prop = AccSortActive; break;
		case 1	: prop = AccSortName; break;
		case 2	: prop = AccSortServer; break;
		case 3	: prop = AccSortUser; break;
		case 4	: prop = AccSortNrMess; break;
		case 5	: prop = AccSortSize; break;
		default : prop = AccSortName;
	}
	
  //sort the view list
  QList<AccountViewItem> sortedList;

  QListIterator<AccountViewItem > itUnsort( viewAccountList );
  while( itUnsort.hasNext() ) {

    //get next account
    AccountViewItem accUnsorted = itUnsort.next();

    //find a place for it in the temporary list
    if( sortedList.size() == 0 ) {

      sortedList.append( accUnsorted );

    } else {

      int sizeSortedList = sortedList.size();
      int indexSort = 0;
      bool placed = false;
      while( indexSort < sizeSortedList && !placed ) {
        
        //get the Account in the sorted List
        AccountViewItem accSorted = sortedList.at( indexSort );

        //is the account from the unsorted list lesser (greater) than the account from the sorted list insert the first one at this
        //position into the sorted list and break the searching for place
        if( order == Qt::AscendingOrder ) {

          if( accUnsorted.compare( accSorted, prop ) <= 0 ) {

            sortedList.insert( indexSort, accUnsorted );
            placed = true;
          }

        } else {

          if( accUnsorted.compare( accSorted, prop ) > 0 ) {

            sortedList.insert( indexSort, accUnsorted );
            placed = true;
          }
        }

        indexSort++;
      }

      //if the account could not placed, we append it at the end
      if( !placed )
        sortedList.append( accUnsorted );
    }
  }

  //switch the lists
  viewAccountList.clear();
  viewAccountList.append( sortedList );
  
	reset();
}

void AccountViewModel::sort()
{
	sort( lastSortColumn, lastSortOrder );
}

void AccountViewModel::saveSetup()
{
	KConfigGroup* conf = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_VIEW );
	
	conf->writeEntry( CONFIG_ENTRY_SORT_COLUMN_ACCOUNT, lastSortColumn );
	
	if( lastSortOrder == Qt::AscendingOrder ) {
	
		conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_ACCOUNT, CONFIG_VALUE_SORT_ORDER_ASCENDING );
	
	} else {
	
		conf->writeEntry( CONFIG_ENTRY_SORT_ORDER_ACCOUNT, CONFIG_VALUE_SORT_ORDER_DESCENDING );
	}
	
	conf->sync();
}

Account* AccountViewModel::getAccount(const QModelIndex index) const
{
  AccountViewItem item = viewAccountList.at( index.row() );
	return item.getAccount();
}
