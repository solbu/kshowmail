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

#ifndef FILTERLOGVIEWDELETEDMODEL_H
#define FILTERLOGVIEWDELETEDMODEL_H

//Qt headers
#include <QModelIndex>
#include <QList>

//KShowmail headers
#include "filterlog.h"
#include "types.h"

#define NUMBER_VIEWDELETED_COLUMNS 4

/**
 * @brief Model for the log view of deleted mails.
 */
class FilterLogViewDeletedModel : public QAbstractItemModel
{

  Q_OBJECT

  public:

    /**
     * @brief Constructor
     */
    FilterLogViewDeletedModel( QObject* parent = 0, FilterLog* log = NULL );
    
    /**
     * @brief Overloaded from QAbstractItemModel
     *
     * Returns the data of the given index.
     * @param index the index
     * @param role the role
     */
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    
    /**
     * @brief Overloaded from QAbstractItemModel
     *
     * Doesn't give a parent item, because the model doesn't provide tree structured data.
     * The column count is equal to the number of showed account data.
     * Always returns 6.
     */
    virtual int columnCount( const QModelIndex& ) const;
    
    /**
     * @brief Overloaded from QAbstractItemModel
     *
     * Doesn't give a parent item, because the model doesn't provide tree structured data
     * The row count is equal to the number of accounts.
     * @param parent parent index
     */
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    
    /**
     * @brief Overloaded from QAbstractItemModel
     *
     * Because this model doesn't provide tree structured data this function just returns
     * an invalid index.
     */
    virtual QModelIndex parent( const QModelIndex& ) const;
    
    /**
     * @brief Returns the index of the data in this model specified by the given row, column.
     *
     * The parent index is not used.
     * Overloaded from QAbstractItemModel
     * @param row Row
     * @param column Column
     * @param parent parent index; not used
     */
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    /**
     * @brief Returns the header text of the given section
     * @param section header section
     * @param orientation orientation of the header; just returns the horizontal orientation
     * @param role display role
     */
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    /**
     * Sorts the model by column in the given order.
     * @param column column to sort
     * @param order sort order
     */
    void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );

    /**
     * @brief Sorts the model by the last sort properties.
     */
    void sort();

    /**
     * @brief Reloads the data from the log
     */
    void refresh();
    
  private:

    /**
     * @brief Pointer to the log
     */
    FilterLog* log;

    /**
     * @brief This list contains the items which represents the deleted mails.
     *
     * Will be load from filter log by constructor.
     */
    QList<FilterLogEntry> list;
  
    /**
     * Last sort order
     */
    Qt::SortOrder lastSortOrder;

    /**
     * last sort column
     */
    int lastSortColumn;

};

#endif // FILTERLOGVIEWDELETEDMODEL_H
