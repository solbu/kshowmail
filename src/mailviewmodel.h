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

#ifndef MAILVIEWMODEL_H
#define MAILVIEWMODEL_H

class AccountList;

//Qt Headers
#include <QAbstractItemModel>
#include <QItemSelectionModel>

//KDE headers
#include <KDE/KLocale>
#include <KIcon>
#include <KStandardDirs>
#include <kdatetime.h>

//KShowmail headers
#include "accountlist.h"
#include "mail.h"
#include "account.h"

//constants
#define NUMBER_MAILVIEW_COLUMNS 9

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
		virtual ~MailViewModel();
		
		/**
		 * Returns the index of the mail data in this model specified by the given row, column.
		 * The parent index is not used.
		 * Overloaded from QAbstractItemModel
		 * @param row Row
		 * @param column Column
		 * @param parent parent index; not used
		 */
		virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
		
		/**
		 * Overloaded from QAbstractItemModel
		 * Because this model doesn't provide tree structured data this function just returns
		 * an invalid index.
		 */
		virtual QModelIndex parent( const QModelIndex& ) const;
		
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
		 */
		virtual int columnCount ( const QModelIndex& ) const;
		
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
		
		/**
		 * Returns the header text of the given section
		 * @param section header section
		 * @param orientation orientation of the header; just returns the horizontal orientation
		 * @param role display role
		 */
		QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    /**
     * Returns the flags of the given index.
     * All items are enabled and selectable.
     */
    virtual Qt::ItemFlags flags ( const QModelIndex& ) const;

    /**
     * Emits the dataChanged signal to the views to inform they about changed data
     */
    void refresh();

    /**
		 * Sorts the model by column in the given order.
		 * @param column column to sort
		 * @param order sort order
		 */
		void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
		
		/**
		 * Sorts the model by the last sort properties.
		 */
		void sort();
		
		/**
		 * Returns the mail at the given index
		 * @param index index
		 */
		Mail getMail( const QModelIndex index ) const;
		
		/**
     * Returns the subjects of the selected mails.
     * @param selectModel mail select model
     * @return mail subjects
     */
    QStringList getSelectedSubjects( QItemSelectionModel* selectModel ) const;

    /**
     * Returns the senders of the selected mails.
     * @param selectModel mail select model
     * @return mail senders
     */
    QStringList getSelectedSenders( QItemSelectionModel* selectModel ) const;

    /**
     * Returns the selected mails.
     * @param mailSelectModel selection model of the mail view
     * @return selected mails
     */
    QList<Mail> getSelectedMails( QItemSelectionModel* mailSelectModel ) const;

    /**
     * Returns the indexes of mails which are marked by filter.<p>
     * @return indexes of mails marked by filter
     */
    QModelIndexList getMarkedMails() const;

    /**
     * Saves the setup
     */
    void saveSetup();


		
  private:

    /**
     * Pointer to the account list
     */
    AccountList* accounts;

    /**
     * In this we store the pointer to the mails
     */
    QList<Mail> viewMailList;

    /**
     * Icon for a new mail
     */
    KIcon picNewMail;
    
		/**
		 * Last sort order
		 */
		Qt::SortOrder lastSortOrder;
		
		/**
		 * last sort column
		 */
		int lastSortColumn;


};

#endif // MAILVIEWMODEL_H
