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

#ifndef ACCOUNT_H
#define ACCOUNT_H

//C++ headers
#include <iostream>

//Qt header
#include <QObject>
#include <QList>

//KShowmail headers
#include "mail.h"

using namespace std;

/**
 * This class represents a account
 */
class Account : public QObject
{

	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param name name of this account
		 * @param parent parent object
		 */
		Account( QString name, QObject* parent );
		
		/**
		 * Destructor
		 */
		~Account();
		
		/**
		 * Prints all data of this account to stdout
		 */
		void print() const;
		 
		/**
		 * Returns the account name
		 * @return account name
		 */
		QString getName() const;
		
		/**
		 * Creates a mail object and adds it to the list
		 * @param unid UNID of the mail
		 */
		void addMail( const QString& unid );

    /**
     * Sets the server
     * @param server Server host name
     */
    void setServer( const QString& server );

    /**
     * Returns the server name
     * @return server name
     */
    QString getServer() const;

    /**
     * Returns whether the account is active.
     * @return TRUE - account is active; FALSE - account is not active
     */
    bool isActive() const;

    /**
     * Sets the account active.
     * @param active TRUE - active account; FALSE - deactivate account
     */
    void setActive( bool active );

  protected:

    /**
     * Initiate the account
     */
    void init();
     
		
	private:
		
		/**
		 * Name of this account
		 */
		QString name;
		
		/**
		 * List of mails
		 */
		QList<Mail*> mails;

    /**
     * The Server name
     */
    QString server;

    /**
     * TRUE - account is active; FALSE - account is not active
     */
    bool active;


		
};

#endif // ACCOUNT_H
