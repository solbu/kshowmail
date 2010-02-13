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

class MailList;

//Qt headers
#include <QtCore/QObject>

//KDE headers
#include <klocale.h>

//KShowmail headers
#include "mail.h"
#include "types.h"
#include "corruptdataexception.h"
#include "account.h"
#include "headerfilter.h"
#include "filterlog.h"

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

    /**
     * Returns a pointer to the account which this list holds.
     * @return account
     */
    Account* getAccount() const;

    /**
     * Apply the filters to the mails in this list.
     * @param filter pointer to the header filter
     * @param account name of this account
     * @param deleteList reference to a list in which this methode writes the numbers of mails to delete
     * @param downloadList reference to a list in which this methode writes the number of mails to download
     * @param nmbIgnoredMails reference to an integer in which it writes the number of ignored mails
     * @param log pointer to the filter log
     */
    void applyHeaderFilter( HeaderFilter* filter, QString account, MailNumberList_Type& deleteList, MailToDownloadMap_Type& downloadList, int& nmbIgnoredMails, FilterLog* log = NULL );

    /**
     * Removes the mail which has the given number.
     * @param number number of the mail which will be removed
     */
    void removeMail( int number );

    /**
     * Saves all stored mails into the given DOM document inside the
     * given account element.
     * @param doc DOM document in that all options are stored
     * @param parent account element
     */
    void saveMails( QDomDocument& doc, QDomElement& parent );

    /**
     * Returns the number of marked mails.<p>
     * The number is not the number which is given by the mail server.
     * It is the number in order of storage in this list.<p>
     * We need this numbers, because the order of mails in the mail view model is like
     * the order in this list.<p>
     * The first mail in this list is number zero.
     * @return numbers mails marked by filter
     */
    QList<int> getMarkedMails() const;
     
    /**
     * Returns the sender of the mail with the given number.
     * @param number mail number
     * @return sender
     */
    QString getSenderOf( int number ) const;

    /**
     * Returns the date of sent of the mail with the given number.
     * @param number mail number
     * @return date
     */
    QString getDateOf( int number ) const;

    /**
     * Returns the size of the mail with the given number.
     * @param number mail number
     * @return size
     */
    QString getSizeOf( int number ) const;

    /**
     * Returns the subject of the mail with the given number.
     * @param number mail number
     * @return subject
     */
    QString getSubjectOf( int number ) const;

    /**
     * Returns the charset of the mail with the given number.
     * @param number mail number
     * @return charset
     */
    QString getCharsetFromHeaderOf( int number ) const;

    /**
     * Decodes the given mail body by the appropriate mail object.
     * The mail object has to do this job, because
     * it has stored the mail header and therefore it knows the content
     * type and encoding of the mail.
     * It decodes just the text or html part of the body. The rest of it
     * will be rejected.
     * @param body the whole mail (including header)
     * @param number number of the downloaded mail
     * @param preferHTML decode HTML part if present
     * @return decoded mail body
     */
    QStringList decodeMailBody( const QStringList& body, int number, bool preferHTML ) const;

    /**
     * Writes the mail into the Move Log.
     * @param log pointer to the log
     * @param number number of the mail
     * @param account name of the account
     * @param mailbox name of the mailbox
     */
    void writeToMoveLog( FilterLog* log, int number, QString account, QString mailbox );

    /**
     * Writes the mail into the Delete Log.
     * @param log pointer to the log
     * @param number number of the mail
     * @param account name of the account
     */
    void writeToDeleteLog( FilterLog* log, int number, QString account );

    /**
     * The given mail will be marked at the next view refresh.
     * @param number number of the mail
     */
    void setMarkAtNextViewRefresh( int number );



  private:

    /**
     * List of mails
     */
    QList<Mail*> mails;


};

#endif // MAILLIST_H
