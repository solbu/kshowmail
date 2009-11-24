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
#include <QDateTime>

//KDE headers
#include <kmime_codec_base64.h>

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
     * @param number number of the mail on the server
     * @param uid Unique ID of the mail
     * @param isNew TRUE - mail is new; FALSE - mail is not new
		 * @param parent parent object
		 */
		Mail( long number, const QString& unid, bool isNew, QObject* parent );
		
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
		void setNumber( long number );
		
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
		long getNumber() const;

    /**
     * Sets whether the mail is new or not.
     * @param isnew TRUE - mail is new; FALSE - mail is not new
     */
    void setNew( bool isnew );

    /**
     * Returns whether the mail is new or not.
     * @return TRUE - mail is new
     * @return FALSE - mail is not new
     */
    bool isNew() const;

    /**
     * Sets the mail header and extracts From, To, Subject, Date and Content Type.
     * All parts of the header are coded according RFC 2047.
     * @param header the mail header
     */
    void setHeader( const QStringList& header );
    
    /**
     * Returns the header
     * @return mail header
     */
    QStringList getHeader() const;

    /**
     * Sets the given string as subject
     * @param subject the mail subject
     */
    void setSubject( const QString& subject );

    /**
     * Sets the given string as sender address
     * @param from sender address
     */
    void setFrom( const QString& from );

    /**
     * Sets the given string as recipient address
     * @param to recipient address
     */
    void setTo( const QString& to );

    /**
     * Converts the given string to a date-time value and stores it.
     * @param date the date
     */
    void setDate( const QString& date );

    /**
     * Sets the given string as content type.
     * @param content the content type
     */
    void setContent( const QString& content );

    
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
		long size;
		
		/**
		 * Number of this mail on the server
		 */
		long number;

    /**
     * It is set to TRUE when the mail is new.
     */
    bool _new;

    /**
     * The sender address
     */
    QString from;

    /**
     * The addressee
     */
    QString to;

    /**
     * The date on which the mail was sent
     */
    QDateTime sendDate;
    
    /**
     * The content type
     */
    QString contentType;


	protected:
		
		/**
		 * Initiates the object
		 */
		void init();

    /**
     * Searches in the header for a line which starts with the
     * given item.
     * For example:
     * scanHeader( "From") will find the line
     * "From: Ulrich Weigelt <ulrich.weigelt@gmx.de>"
     * and returns "Ulrich Weigelt <ulrich.weigelt@gmx.de>"
     * Returns an empty string (""), if nothing was found.
     * @param item the search item
     * @return the content of the found line
     */
    QString scanHeader( const QString& item ) const;

    
		
};

#endif // MAIL_H
