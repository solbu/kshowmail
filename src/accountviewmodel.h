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

#ifndef ACCOUNTVIEWMODEL_H
#define ACCOUNTVIEWMODEL_H

class AccountList;

//Qt headers
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QList>

//KDE headers
#include <KDebug>
#include <KDE/KLocale>
#include <KIcon>
#include <KStandardDirs>
#include <KConfigGroup>

//KShowmail headers
#include "accountlist.h"
#include "account.h"
#include "accountviewitem.h"

//constants
#define NUMBER_ACCOUNTVIEW_COLUMNS 6

/**
 * @brief Model for the account view.
 */
class AccountViewModel : public QAbstractItemModel
{
	Q_OBJECT
	
	public:
		
		/**
		 * @brief Constructor
		 * @param accounts pointer to the account list
		 * @param parent parent object
		 */
		AccountViewModel( AccountList* accounts, QObject* parent );
		
		/**
		 * @brief Destructor
		 */
		~AccountViewModel();
		
		/**
		 * @brief Returns the index of the account data in this model specified by the given row, column.
     *
		 * The parent index is not used.
		 * Overloaded from QAbstractItemModel
		 * @param row Row
		 * @param column Column
		 * @param parent parent index; not used
		 */
		virtual QModelIndex index( int row, int column, const QModelIndex& parent ) const;
		
		/**
		 * @brief Overloaded from QAbstractItemModel
     *
		 * Because this model doesn't provide tree structured data this function just returns
		 * an invalid index.
		 */
		virtual QModelIndex parent( const QModelIndex& ) const;
		
		/**
		 * @brief Overloaded from QAbstractItemModel
     *
		 * Doesn't give a parent item, because the model doesn't provide tree structured data
		 * The row count is equal to the number of accounts.
		 * @param parent parent index
		 */
		virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		
		/**
		 * @brief Overloaded from QAbstractItemModel
     *
		 * Doesn't give a parent item, because the model doesn't provide tree structured data.
		 * The column count is equal to the number of showed account data.
		 * Always returns 6.
		 */
		virtual int columnCount ( const QModelIndex& ) const;
		
		/**
		 * @brief Overloaded from QAbstractItemModel
     *
		 * Returns the data of the given index.
		 * @param index the index
		 * @param role the role
		 */
		virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		
		/**
		 * @brief Overloaded from QAbstractItemModel
     *
		 * It returns true if the given index is invalid because a invalid index is the root index.
     * All content is a child of the root index.
     * If the index is valid it always returns false because no data has a child.
		 */
		virtual bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
		
		/**
		 * @brief Returns the flags of the given index
     *
		 * @param index index
		 */
		virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
		
		/**
		 * @brief Returns the header text of the given section
		 * @param section header section
		 * @param orientation orientation of the header; just returns the horizontal orientation
		 * @param role display role
		 */
		QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

		/**
		 * @brief Assign the given value to the account property described by the given index
		 * @param index index
		 * @param value value
		 * @param role role
		 */
		bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    /**
     * @brief Emits the dataChanged signal to the views to inform they about changed data
     */
    void refresh();
    
    /**
		 * @brief Sorts the model by column in the given order.
		 * @param column column to sort
		 * @param order sort order
		 */
		void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
		
		/**
		 * @brief Sorts the model by the last sort properties.
		 */
		void sort();
		
		/**
		 * @brief Saves the setup
		 */
		void saveSetup();
		
		/**
		 * @brief Returns the account at the given index
		 * @param index index
		 */
		Account* getAccount( const QModelIndex index ) const;
		
	private:
		
		/**
		 * @brief Pointer to the account list
		 */
		AccountList* accounts;

    /**
     * @brief In this we store the account view items
     */
    QList<AccountViewItem > viewAccountList;

    /**
     * @brief Picture for a active account
     *
     * Set by the constructor
     */
    KIcon picActive;

    /**
     * @brief Picture for a inactive account
     *
     * Set by the constructor
     */
    KIcon picNotActive;
		
		/**
		 * @brief Last sort order
		 */
		Qt::SortOrder lastSortOrder;
		
		/**
		 * @brief last sort column
		 */
		int lastSortColumn;


		
};

#endif // ACCOUNTVIEWMODEL_H
