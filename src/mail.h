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

#ifndef MAIL_H
#define MAIL_H

class Mail;

//C++ headers
#include <iostream>

//Qt header
#include <QObject>
#include <QStringList>
#include <QRegExp>
#include <QTextCodec>
#include <QDomElement>
#include <QPointer>

//KDE headers
#include <kmime/kmime_codecs.h>
#include <kmime/kmime_headers.h>
#include <KDateTime>

//KShowmail headers
#include "maillist.h"
#include "account.h"

using namespace std;

//separator to join the header to a single string to save it on disk
#define HEADER_SEPARATOR "§$%&"

/**
 * @brief This class represents a mail
 */
class Mail : public QObject
{

	Q_OBJECT

	public:


		/**
		 * Constructor
     * @param number number of the mail on the server
     * @param unid Unique ID of the mail
     * @param isNew TRUE - mail is new; FALSE - mail is not new
     * @param account pointer to the parent account
		 * @param parent parent object
		 */
		Mail( long number, const QString& unid, bool isNew, QPointer<Account> account, QObject* parent );

    /**
     * Copy Constructor
     * @param mail source mail
     */
    Mail( const Mail& mail );

		/**
		 * Destructor
		 */
		~Mail();

    /**
     * Assign operator
     * @param other other mail
     */
    Mail& operator=( const Mail& other );

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
     * Returns the datetime of mail send
     */
    KDateTime getDateTime() const;

    /**
     * Sets the given string as content type.
     * @param content the content type
     */
    void setContent( const QString& content );

    /**
     * Returns the content type.
     * @return content type
     */
    QString getContent() const;

    /**
     * Returns a pointer to the account which contains this mail.
     * @return account
     */
    QPointer<Account> getAccount() const;

    /**
     * Returns the sender address.
     * @return sender address
     */
    QString getFrom() const;

    /**
     * Returns the addressee.
     * @return the addressee
     */
    QString getTo() const;

    /**
     * Returns the mails size as string with suffix
     */
    QString getSizeSuffix() const;

    /**
     * Looks for a filter matching the mail header and returns the action recommend by this filter.
     * If the action is MARK, it set markAtViewRefresh to True so the related view entry will be marked
     * at the next view refresh.
     * @param filter pointer to the header filter
     * @param account name of the account
     * @param mailbox reference to a QString object in which the mailbox name shall be written if the action is MOVE
     * @param log pointer to the filter log
     * @return recommend action
     * @see FilterAction_Type
     * @see HeaderFilter
     * @see markAtViewRefresh
     */
    FilterAction_Type applyHeaderFilter( HeaderFilter* filter, QString account, QString& mailbox, FilterLog* log = NULL );

    /**
     * Creates a new DOM element in the given DOM document and add it to given
     * DOM parent element
     * @param doc the DOM document in that all options are stored
     * @param parent the parent element (account) of the mail
     */
    void save( QDomDocument& doc, QDomElement& parent );

    /**
     * Returns whether this mail was marked by filter.
     * @return TRUE - marked; FALSE - not marked
     */
    bool isMarkedByFilter() const;

    /**
     * Decodes the given mail body.
     * The mail object has to do this job, because
     * it has stored the mail header and therefore it knows the content
     * type and encoding of the mail.
     * It decodes just the text or html part of the body. The rest of it
     * will be rejected.
     * @param body the encoded mail (including header)
     * @param preferHTML decode HTML part if present
     * @param isHTML will be set on true, if the returned body is HTML formatted; otherwise false
     * @return decoded mail body
     */
    QStringList decodeMailBody( const QStringList& body, bool preferHTML, bool& isHTML ) const;

    /**
     * Returns the boundary, if the mail has a multi part body.
     * Otherwise it returns an empty string.
     * @return boundary
     */
    QString getBoundary() const;

    /**
     * Returns the char set of the content (e.g. iso-8859-1) from the header.
     * If no char set is denoted, it will returns an empty string.
     * @return charset
     */
    QString getCharsetFromHeader() const;

    /**
     * Returns the transfer encoding string from the header if existing.
     * Otherwise it returns an empty string.
     * @return transfer encoding
     */
    QString getTransferEncodingFromHeader() const;

    /**
     * Writes this mail into the Move Log.
     * @param log pointer to the log
     * @param account name of the account
     * @param mailbox name of the mailbox
     */
    void writeToMoveLog( FilterLog* log, QString account, QString mailbox );

    /**
     * Writes this mail into the Delete Log.
     * @param log pointer to the log
     * @param account name of the account
     * @param kindDelete kind of mail deleting (by filter or manual)
     */
     void writeToDeleteLog( FilterLog* log, QString account, KindOfMailDeleting kindDelete );

    /**
     * The mail will be marked at the next view refresh.
     */
    void setMarkAtNextViewRefresh();

    /**
     * Compares this mail with <i>other</i> and returns an integer less than,
     * equal to, or greater than zero if this mail is less than, equal to,
     * or greater than <i>other</i>.
     * @param other other mail
     * @param property the mail property which will be compared
     */
    int compare( const Mail& other, MailSort_Type property );

    /**
     * Returns the name of the parent account.
     * @return name of the parent account
     * @see accountName
     */
    QString getAccountName() const;

    /**
     * Returns the name of the filter which has caught this mail.
     *
     * @return name of the filter which has caught this mail
     */
    QString getFilterName() const;

    /**
     * Sets the name of the filter which has caught this mail
     *
     * @param filter name of the filter
     */
    void setFilterName( QString filter );


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
		int number;

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
    KDateTime sendDate;

    /**
     * The content type
     */
    QString contentType;

    /**
     * TRUE - the mail was marked by the filter.
     * It will be set to True by applyHeaderFilter() if the recommend filter action is MARK.
     */
    bool markedByFilter;

    /**
     * Pointer to the parent account
     */
    QPointer<Account> acc;

    /**
     * Name of the parent account
     */
    QString accountName;

    /**
     * The name of the filter which has caught this mail
     */
    QString filterName;


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

    /**
     * Searches in the given body part for a line which starts with the
     * given item.
     * For example:
     * scanBodyPart( "Content-Transfer-Encoding") will find the line
     * "Content-Transfer-Encoding: 7bit"
     * and returns "7bit"
     * Returns an empty string (""), if nothing was found.
     * @param part the body part
     * @param item the search item
     * @return the content of the found line
     */
    QString scanBodyPart( const QStringList& part, const QString& item ) const;

    /**
     * @brief Decodes a rfc2047 encoded string
     * @param text text to encode
     */
    QString decodeRfc2047( const QString& text ) const;

    /**
     * Returns the charset of the given header or body part.<p>
     * It looks for the charset tag and returns the string behind this.
     * @param text body part or header
     * @return charset string
     */
    QString getCharset( const QStringList& text ) const;

    /**
     * Returns the transfer encoding of the given header or body part.<p>
     * It looks for the transfer encoding tag and returns the string behind this.
     * @param text body part or header
     * @return charset string
     */
    QString getTransferEncoding( const QStringList& text ) const;

    /**
     * Removes all empty lines at begin and at end of <em>text</em>
     * @param text the text which shall be trimmed
     * @return the text without empty lines at begin and at end
     */
    QStringList trim( QStringList text ) const;

    /**
     * Prints the given text to StdOut.
     * @param text Text to print
     */
    void print( QStringList text ) const;

};

#endif // MAIL_H
