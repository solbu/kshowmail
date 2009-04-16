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
		
	private:
		
		/**
		 * Name of this account
		 */
		QString name;
		
		
		
};

#endif // ACCOUNT_H
