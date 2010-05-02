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

#ifndef KSHOWMAILVIEW_H
#define KSHOWMAILVIEW_H

class AccountViewModel;

//Qt headers
#include <qsplitter.h>
#include <QTreeView>
#include <QModelIndexList>
#include <QItemSelectionModel>

//KDE headers
#include <KAction>

//KShowmail headers
#include "accountviewmodel.h"
#include "mailviewmodel.h"
#include "accountviewdelegate.h"

/**
 * @brief Main view of KShowmail.
 *
 * It contains the account view and the mail view.
 */
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
     */
    void refreshViews( QItemSelectionModel* mailSelectModel );
		
		/**
		 * Saves the column widths
		 */
		void saveSetup();
		
		/**
		 * Loads and sets the column widths
		 */
		void loadSetup();

    /**
     * Adds an action to the account view context menu
     * @param action action to add
     */
    void addActionToAccountList( KAction* action );

    /**
     * Adds an action to the mail view context menu
     * @param action action to add
     */
    void addActionToMailList( KAction* action );
		
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
