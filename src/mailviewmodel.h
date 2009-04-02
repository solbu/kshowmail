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

#ifndef MAILVIEWMODEL_H
#define MAILVIEWMODEL_H

//Qt Headers
#include <QAbstractItemModel>

//KShowmail headers
#include "accountlist.h"

class MailViewModel : public QAbstractItemModel
{
	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param accounts pointer to the account list
		 * @param parent parent object
		 */
		MailViewModel( AccountList* accounts, QObject* parent );
		
		/**
		 * Destructor
		 */
		~MailViewModel();
		
		/**
		 * Returns the index of the mail data in this model specified by the given row, column.
		 * The parent index is not used.
		 * Overloaded from QAbstractItemModel
		 * @param row Row
		 * @param column Column
		 * @param parent parent index; not used
		 */
		virtual QModelIndex index( int row, int column, const QModelIndex& parent ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Because this model doesn't provide tree structured data this function just returns
		 * an invalid index.
		 * @param index child of the searched index
		 */
		virtual QModelIndex parent( const QModelIndex& index ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Doesn't give a parent item, because the model doesn't provide tree structured data
		 * The row count is equal to the number of mails.
		 * @param parent parent index
		 */
		virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Doesn't give a parent item, because the model doesn't provide tree structured data.
		 * The column count is equal to the number of showed mail data
		 * @param parent parent index
		 */
		virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Returns the data of the given index.
		 * @param index the index
		 * @param role the role
		 */
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Because this model doesn't provides tree structured data, this methode always returns false
		 */
		bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
		


};

#endif // MAILVIEWMODEL_H
