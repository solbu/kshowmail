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

#ifndef ACCOUNTVIEWDELEGATE_H
#define ACCOUNTVIEWDELEGATE_H

//qt headers
#include <QItemDelegate>

/**
 * @brief To edit some datas in the account view.
 */
class AccountViewDelegate : public QItemDelegate
{
	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param parent parent object
		 */
	  AccountViewDelegate( QObject *parent = 0 );

		/**
		 * Returns a editor widget to edit the given index
		 * @param parent parent of the returned widget
		 * @param option options for editor widget
		 * @param index index which shall be handled by the editor
		 * @return editor widget
		 */
    QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index ) const;

		/**
		 * Sets the data to be displayed and edited by the 
		 * editor from the data model item specified by the model index.
		 * @param editor editor widget
		 * @param index Index of the model data
		 */
		void setEditorData( QWidget *editor, const QModelIndex &index ) const;
		
		/**
		 * Gets data drom the editor widget and stores it in the 
		 * account view model at the item index.
		 * @param editor Editor which has edited the data
		 * @param model the Account View Model
		 * @param index edited index
		 */
     void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;

		 /**
		  * Updates the geometry of the editor widget
			* @param editor editor widget
			* @param option Options for the editor widget
			* @param index data index
			*/
     void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

};

#endif // ACCOUNTVIEWDELEGATE_H
