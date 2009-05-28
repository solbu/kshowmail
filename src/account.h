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

//KDE headers
#include <KConfigGroup>
#include <KConfig>
#include <KGlobal>

//KShowmail headers
#include "mail.h"
#include "types.h"
#include "constants.h"
#include "encryption.h"
#include "kwalletaccess.h"

using namespace std;
using namespace Types;
using namespace Encryption;
using namespace KWalletAccess;

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

    /**
     * Loads the options of this account from the application config.
     */
    void load();

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

    /**
     * Type of the transfer security
     */
    TransferSecurity_Type transferSecurity;

		/**
		 * User name
		 */
		QString user;
		
		/**
		 * Password
		 */
		QString password;
		
		/**
		 * Port
		 */
		int port;
		
		/**
		 * Protocol
		 */
		QString protocol;
		
};

#endif // ACCOUNT_H
