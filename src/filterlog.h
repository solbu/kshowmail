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

#ifndef FILTERLOG_H
#define FILTERLOG_H

//Qt headers
#include <qlist.h>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>

//KDE headers
#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <kconfig.h>
#include <KConfigGroup>
#include <KDateTime>

//KShowmail headers
#include "filterlogentry.h"
#include "constants.h"

/**
 * @brief This is the log of the filters.
 * It holds two lists of entry objects (class FilterLogEntry). One for the deleted mails and the other one for
 * the moved mails.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */

typedef QList<FilterLogEntry> LogEntryList;


class FilterLog{


  public:

    /**
     * Default constructor
     */
    FilterLog();

    /**
     * Destructor
     */
    ~FilterLog();

    /**
     * Adds an entry about a deleted mail.
     * @param dateTime date and time on which the mail was sent
     * @param sender sender of the mail
     * @param account Account which has received the mail
     * @param subject Subject of the mail
     */
    void addDeletedMail( const KDateTime& dateTime, const QString& sender, const QString& account, const QString& subject );

    /**
     * Adds an entry about a moved mail.
     * @param dateTime date and time on which the mail was sent
     * @param sender sender of the mail
     * @param account Account which has received the mail
     * @param subject Subject of the mail
     * @param mailbox mailbox
     */
    void addMovedMail( const KDateTime& dateTime, const QString& sender, const QString& account, const QString& subject, const QString& mailbox );

    /**
     * Prints the log state.
     */
    void print();

    /**
     * Clears the log of deleted mails.
     */
    void clearDeletedMailsLog();

    /**
     * Clears the log of moved mails.
     */
    void clearMovedMailsLog();

    /**
     * Saved the log of deleted mails as XML document.
     */
    void save();

    /**
     * Loads the log of deleted mails from the XML document and get settings.
     */
    void load();

    /**
     * Returns a copy of the list of deleted mails.
     * @return copy of the deleted mails list
     */
    LogEntryList getDeletedMails();

    /**
     * Retruns a copy of the list of moved mails.
     * @return copy of the moved mails list
     */
    LogEntryList getMovedMails();

    /**
     * Loads the settings
     */
    void loadSetup();

    /**
     * Returns the number of logged deleted mails.
     * @return number of logged deleted mails
     */
    int numberDeletedMails();

    /**
     * Returns the number of logged moved mails.
     * @return number of logged moved mails
     */
    int numberMovedMails();


  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * List of entries about deleted mails.
     */
    LogEntryList listDeletedMails;

    /**
     * List of entries about moved mails.
     */
    LogEntryList listMovedMails;

    /**
     * TRUE - the log accepts orders to log deleted mails
     */
    bool logDeletedMails;

    /**
     * TRUE - the log accepts orders to log moved mails
     */
    bool logMovedMails;

    /**
     * exit - hold log of deleted mails until application exit
     * days - hold log some days
     */
    enum{ exit, days } deletedMailsStorageMode;

    /**
     * Time (days) a entry of a deleted mail will be stored.
     */
    unsigned int daysStoreDeletedMails;

  protected:

    /**
     * adds an entry.
     * The target list will be coose on the basis of the given filter action.
     */
    void addEntry( FilterAction_Type action, const KDateTime& dateTime, const QString& sender, const QString& account, const QString& subject, const QString& mailbox = QString() );

};


#endif
