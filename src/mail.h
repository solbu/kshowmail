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

#ifndef MAIL_H
#define MAIL_H

//C++ headers
#include <iostream>

//Qt header
#include <QObject>
#include <QStringList>

using namespace std;

/**
 * This class represents a mail
 */
class Mail : public QObject
{

	Q_OBJECT
	
	public:
		
		/**
		 * Constructor
		 * @param unid UNID
		 * @param parent parent object
		 */
		Mail( QString unid, QObject* parent );
		
		/**
		 * Destructor
		 */
		~Mail();
		
		/**
		 * Sets the size
		 * @param size mail size (Bytes)
		 */
		void setSize( long size );
		
		/**
		 * Sets the number.
		 * @param number the number
		 */
		void setNumber( int number );
		
		/**
		 * Prints the data of this mail to stdout
		 */
		void print() const;
		
		/**
		 * Returns the subject
		 * @return subject
		 */
		QString getSubject() const;
		
		/**
		 * Returns the UNID.
		 * @return UNID
		 */
		QString getUNID() const;
		
		/**
		 * Returns the size of this mail
		 * @return mail size (Bytes)
		 */
		long getSize() const;
		
		/**
		 * Returns the number of this mail on the server
		 * @return number
		 */
		int getNumber() const;
		
	private:
		
		/**
		 * UNID of this mail
		 */
		QString unid;
		
		/**
		 * Subject
		 */
		QString subject;
		
		/**
		 * Header
		 */
		QStringList header;
		
		/**
		 * Size (Bytes)
		 */
		int size;
		
		/**
		 * Number of this mail on the server
		 */
		long number;
		
};

#endif // MAIL_H
