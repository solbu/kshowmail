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

#ifndef ACCOUNTVIEWDELEGATE_H
#define ACCOUNTVIEWDELEGATE_H

//qt headers
#include <QItemDelegate>

//KDE headers
#include <KComboBox>
#include <KDE/KLocale>
#include <KIcon>
#include <KStandardDirs>
#include <KDebug>

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
			*/
     void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& ) const;
		 
    /**
     * Picture for a active account used by the combo box to switch the account state
     * Set by the constructor
     */
    KIcon picActive;

    /**
     * Picture for a inactive account used by the combo box to switch the account state
     * Set by the constructor
     */
    KIcon picNotActive;



};

#endif // ACCOUNTVIEWDELEGATE_H
