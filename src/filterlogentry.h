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

#ifndef FILTERLOGENTRY_H
#define FILTERLOGENTRY_H

//Qt headers
#include <qstring.h>
#include <qdom.h>

//KDE headers
#include <kdebug.h>
#include <KDateTime>

//KShowmail headers
#include "types.h"
#include "constants.h"

using namespace Types;

/**
 * @brief An Object of this class contains the datas about a deleted or moved mail.
 * These objects are stored in the lists of the filter log.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */

class FilterLogEntry{

  public:

    /**
     * Default constructor
     */
    FilterLogEntry();

    /**
     * General constructor
     * @param action executed action
     * @param dateTime date and time on which the mail was sent
     * @param sender sender of the mail
     * @param account Account which has received the mail
     * @param subject Subject of the mail
     * @param mailbox mailbox where the mails was moved
     */
    FilterLogEntry( FilterAction_Type action, const KDateTime& dateTime, const QString& sender, const QString& account, const QString& subject, const QString& mailbox = QString::null );

    /**
     * Copy Constructor
     * @param ent source entry
     */
    FilterLogEntry( const FilterLogEntry& ent );

    /**
     * Destructor
     */
    ~FilterLogEntry();

    /**
     * Assignment operator
     * @param ent source entry
     */
    FilterLogEntry& operator=( const FilterLogEntry& ent );

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is equal to te time of the given entry.
     */
    bool operator==( const FilterLogEntry& ent ) const;

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is not equal to the time of the given entry.
     */
    bool operator!=( const FilterLogEntry& ent ) const;

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is later than the time of the given entry.
     */
    bool operator>( const FilterLogEntry& ent ) const;

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is later than or equal to the time of the given entry.
     */
    bool operator>=( const FilterLogEntry& ent ) const;

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is earlier than the time of the given entry.
     */
    bool operator<( const FilterLogEntry& ent ) const;

    /**
     * Compares this entry with the given entry
     * @param ent entry to compare
     * @return TRUE - the time of this entry is earlier than or equal to the time of the given entry.
     */
    bool operator<=( const FilterLogEntry& ent ) const;

    /**
     * Prints the datas of this entry to Stdout.
     */
    void print();

    /**
     * Returns whether the mail of this entry is older than the given number of days.
     * @param days number of days to compare
     * @return TRUE - the mail is older than the given date
     */
    bool isOlder( uint days );

    /**
     * Stores the entry into the given DOM document as child of the given DOM element.
     */
    void save( QDomDocument& doc, QDomElement& parent );

    /**
     * Returns the date and time of send
     * @return send date and time
     */
    KDateTime getDate();

    /**
     * Returns the sender.
     * @return Sender
     */
    QString getSender();

    /**
     * Returns the account.
     * @return Account
     */
    QString getAccount();

    /**
     * Returns the subject
     * @return Subject
     */
    QString getSubject();

    /**
     * Returns the mailbox
     * @return mailbox
     */
    QString getMailbox();

  private:

    /**
     * executed action
     */
    FilterAction_Type act;

    /**
     * Delivery date and time
     */
    KDateTime sentDateTime;

    /**
     * Sender
     */
    QString sender;

    /**
     * Account
     */
    QString account;

    /**
     * Subject
     */
    QString subject;

    /**
     * mailbox if the mails was moved
     */
    QString mailbox;

};

#endif
