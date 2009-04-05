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
}

AccountViewModel::~AccountViewModel(){}

QModelIndex AccountViewModel::index( int row, int column, const QModelIndex& parent ) const
{
  kDebug() << row << " " << column << endl;
  
	//returns a invalid index if the parent index is valid
	//because no index has a child
	if( parent.isValid() ) return QModelIndex();
	
	if( row > 4 || column > 4 ) return QModelIndex();
	
	return createIndex( row, column );
}

QModelIndex AccountViewModel::parent( const QModelIndex& index ) const
{
  kDebug() << endl;
	return QModelIndex();
}

int AccountViewModel::rowCount ( const QModelIndex & parent ) const 
{
	//return 0, if the parent is valid
	if( parent.isValid() ) return 0;
	
	return 5;
}

int AccountViewModel::columnCount ( const QModelIndex & parent ) const
{
	return 5;
}

QVariant AccountViewModel::data ( const QModelIndex & index, int role ) const
{
  kDebug() << index.row() << " " << index.column() << " " << index.isValid() << endl;
  
	//return a empty data if the index is invalid
	if( !index.isValid() ) return QVariant();
	
	if( index.row() > 4 || index.column() > 4 ) return QVariant();

  if( role != Qt::DisplayRole ) return QVariant();
  
	return QVariant( "Data");
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
  
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
