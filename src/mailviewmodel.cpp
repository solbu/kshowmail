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

#include "./mailviewmodel.h"

MailViewModel::MailViewModel( AccountList* accounts, QObject* parent ) : QAbstractItemModel( parent )
{
  this->accounts = accounts;
}

MailViewModel::~MailViewModel(){}

QModelIndex MailViewModel::index( int row, int column, const QModelIndex& parent ) const
{
	//returns a invalid index if the parent index is valid
	//because no index has a child
	if( parent.isValid() ) return QModelIndex();
	
	return createIndex( row, column );
}

QModelIndex MailViewModel::parent( const QModelIndex& index ) const
{
	return QModelIndex();
}

int MailViewModel::rowCount ( const QModelIndex & parent ) const 
{
	//return 0, if the parent is valid
	if( parent.isValid() ) return 0;

	return accounts->getNumberMails();
}

int MailViewModel::columnCount ( const QModelIndex & parent ) const
{
	return NUMBER_MAILVIEW_COLUMNS;
}

QVariant MailViewModel::data ( const QModelIndex & index, int role ) const
{
	//return a empty data if the index is invalid
	if( !index.isValid() ) return QVariant();
	
	if( index.row() >= rowCount() || index.column() > NUMBER_MAILVIEW_COLUMNS - 1 ) return QVariant();

  //get the mail
  Mail* mail = accounts->getMail( index.row() );
  
  //the kind of data we return is dependent on the given role
  switch( role )
  {
    case( Qt::DisplayRole ) :

      switch( index.column() )
      {
        case  0 : return QVariant( (int)mail->getNumber() );
        case  1 : return QVariant( );
        default : return QVariant( QString( "Data %1 %2").arg( index.row() ).arg( index.column() ) );
      }
      break;
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
		case 0	:	return QVariant( i18n( "Number" ) ); break;
		case 1	:	return QVariant( i18n( "Account" ) ); break;
		case 2	:	return QVariant( i18n( "From" ) ); break;
		case 3	:	return QVariant( i18n( "To" ) ); break;
		case 4	:	return QVariant( i18n( "Subject" ) ); break;
		case 5	:	return QVariant( i18n( "Date" ) ); break;
		case 6	:	return QVariant( i18n( "Size" ) ); break;
		case 7	:	return QVariant( i18n( "Content" ) ); break;
		case 8	:	return QVariant( i18n( "State" ) ); break;
		default : return QVariant();
	}
}

Qt::ItemFlags MailViewModel::flags ( const QModelIndex & index ) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MailViewModel::refresh()
{
  //kdDebug() << "Refresh: " << rowCount() << endl;
  //emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount() - 1, NUMBER_MAILVIEW_COLUMNS - 1 ) );
  reset();

}
