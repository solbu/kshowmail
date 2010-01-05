/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef MAILLIST_H
#define MAILLIST_H

//Qt headers
#include <QtCore/QObject>

//KDE headers
#include <klocale.h>

//KSHowmail headers
#include "mail.h"
#include "types.h"
#include "corruptdataexception.h"

using namespace Types;

class MailList : public QObject
{
  Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent object
     */
    MailList( QObject* parent );

    /**
     * Destructor
     */
    ~MailList();

    /**
     * Creates a mail object and adds it to the list
     * @param number number of the mail on the server
     * @param uid Unique ID of the mail
     * @param isNew TRUE - mail is new; FALSE - mail is not new
     */
    void addMail( long number, const QString& unid, bool isNew );

    /**
     * Prints all data of this mail list to stdout
     */
    void print() const;

    /**
     * Returns whether there is mail with the given uid in the list.
     * @param uid UID of the mail
     * @return TRUE - mail is in the list
     * @return FALSE - mail is not in the list
     */
    bool hasMail( QString uid );

    /**
     * Returns whether the given mail is new.
     * @param uid UID of the mail
     * @return TRUE - mail is new
     * @return FALSE - mail is not new or mail doesn't exist
     */
    bool isNew( QString uid ) const;

    /**
     * Sets the size of a mail.
     * @param number number of the mail
     * @param size mail size
     */
    void setSize( long number, long size );

    /**
     * Returns the numbers of mails which are marked as new.
     * @return numbers of the new mails
     */
    Types::MailNumberList_Type getNewMails();

    /**
     * Sets the header of the given mail.
     * @param number number of the mail
     * @param header the header
     */
    void setHeader( int number, QStringList header );

    /**
     * Sets the header of the mail with the given UID.
     * @param unid UID
     * @param header mail header
     */
    void setHeader( QString unid, QStringList header );    

    /**
     * Returns a list of the UIDs of all old mails.
     * @return UIDs of all old mails
     */
    QStringList getUIDsOfOldMails();

    /**
     * Returns the header of the mail with the given UID.
     * @param unid UID
     * @return mail header
     * @throw CorruptDataException
     */
    QStringList getHeaderOf( QString unid ) throw( CorruptDataException );

    /**
     * Returns the number of mails.
     * @return number of mails
     */
    int getNumberMails() const;

    /**
     * Returns the mail with the given number.<p>
     * The number is not the number which is given by the mail server.
     * It is the number in order of storage in this list.<p>
     * @param number number of the desired mail
     * @return pointer to the desired mail
     * @throws CorruptDataException no mail with this number is available
     */
    Mail* getMail( int number ) const throw ( CorruptDataException );

  private:

    /**
     * List of mails
     */
    QList<Mail*> mails;


};

#endif // MAILLIST_H
