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

#ifndef KSHOWMAILVIEW_H
#define KSHOWMAILVIEW_H

class AccountViewModel;

//Qt headers
#include <qsplitter.h>
#include <QTreeView>

//KShowmail headers
#include "accountviewmodel.h"
#include "mailviewmodel.h"
#include "accountviewdelegate.h"

class KShowmailView : public QSplitter
{
	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param accountModel model for the account view
		 * @param mailModel model for the mail view
     * @param mailSelectModel selection model of the mail view model
     * @param accountSelectModel selection model of the account view model
		 * @param parent pointer to parent widget
		 */
		KShowmailView( AccountViewModel* accountModel, MailViewModel* mailModel, QItemSelectionModel* mailSelectModel, QItemSelectionModel* accountSelectModel, QWidget* parent = 0 );
		
		/**
		 * Destructor
		 */
		~KShowmailView();

    /**
     * Refreshes the account and mail view. Marks filtered mails.
     * @param mailSelectModel mail selection model
     * @param markedMails row numbers of the filtered mails which shall be marked
     */
    void refreshViews( QItemSelectionModel* mailSelectModel, QList<int> markedMails );
		
		/**
		 * Saves the column widths
		 */
		void saveSetup();
		
		/**
		 * Loads and sets the column widths
		 */
		void loadSetup();

    void addActionToAccountList( KAction* action );
		
	private:
		
		/**
		 * account list
		 */
		QTreeView* viewAccounts;
		
		/**
		 * mail list
		 */
		QTreeView* viewMails;
		
		/**
		 * Mail view model
		 */
		MailViewModel* mailModel;
		
		/**
		 * Account view model
		 */
		AccountViewModel* accountModel;
};

#endif // KSHOWMAILVIEW_H
