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

//Qt headers
#include <qsplitter.h>

//KDE headers
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
		 * @param parent pointer to parent widget
		 */
		KShowmailView( AccountViewModel* accountModel, MailViewModel* mailModel, QWidget* parent = 0 );
		
		/**
		 * Destructor
		 */
		~KShowmailView();
		
	private:
		
		/**
		 * account list
		 */
		QTreeView* viewAccounts;
		
		/**
		 * mail list
		 */
		QTreeView* viewMails;
};

#endif // KSHOWMAILVIEW_H
