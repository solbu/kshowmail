/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "./accountviewmodel.h"

AccountViewModel::AccountViewModel( AccountList* accounts, QObject* parent ) : QAbstractItemModel( parent )
{
	this->accounts = accounts;

  //load pictures for a active or inactive account
  picActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountActive.png" ) );
  picNotActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountNotActive.png" ) );

}

AccountViewModel::~AccountViewModel(){}

QModelIndex AccountViewModel::index( int row, int column, const QModelIndex& parent ) const
{
	//returns a invalid index if the parent index is valid
	//because no index has a child
	if( parent.isValid() ) return QModelIndex();
	
	return createIndex( row, column );
}

QModelIndex AccountViewModel::parent( const QModelIndex& index ) const
{
	return QModelIndex();
}

int AccountViewModel::rowCount ( const QModelIndex & parent ) const 
{
	//return 0, if the parent is valid
	if( parent.isValid() ) return 0;

	return accounts->numberAccounts();
}

int AccountViewModel::columnCount ( const QModelIndex & parent ) const
{
	return NUMBER_ACCOUNTVIEW_COLUMNS;
}

QVariant AccountViewModel::data ( const QModelIndex & index, int role ) const
{  
	//return a empty data if the index is invalid
	if( !index.isValid() ) return QVariant();
	
	if( index.row() > rowCount() || index.column() > NUMBER_ACCOUNTVIEW_COLUMNS - 1 ) return QVariant();

  //get the account object
  Account* acc = accounts->getAccount( index.row() );

  //return a empty data if the pointer to the account is NULL
  if( acc == NULL ) return QVariant();

  //the kind of data we return is dependent on the given role
  switch( role )
  {
    case( Qt::DisplayRole ) :

      switch( index.column() )
      {
				case 0  : return QVariant(); break;
        case 1  : return QVariant( acc->getName() ); break;
        case 2  : return QVariant( acc->getHost() ); break;
        case 3  : return QVariant( acc->getUser() ); break;
        case 4  : return QVariant( acc->getNumberMails() ); break;
        case 5  : return QVariant( acc->getTotalSizeUnit() ); break;
        default : return QVariant(); break;
      }
      break;

    case( Qt::DecorationRole ):

      switch( index.column() )
      {
        case 0  :
          if( acc->isActive() )
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
					if( acc->isActive() )
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
		case 0	:	return QVariant( i18n( "Active" ) ); break;
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
	
	//get account
  Account* acc = accounts->getAccount( index.row() );
  if( acc == NULL ) return false;
	
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
  reset();
  
}

