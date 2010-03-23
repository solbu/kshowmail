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

#ifndef FILTERLOGVIEWDELETEDITEM_H
#define FILTERLOGVIEWDELETEDITEM_H

//Qt Headers
#include <QTreeWidget>
#include <QTreeWidgetItem>

//KDE headers
#include <KDateTime>

/**
 * @brief Item of the filter log view of deleted mails
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterLogViewDeletedItem : public QTreeWidgetItem
{

  public:

    /**
     * Column Numbers.
     */
    enum Column{ ColDate = 0, ColSender = 1, ColAccount = 2, ColSubject = 3 };

    /**
     * Constructor
     * @param parent the log view of this items
     */
    FilterLogViewDeletedItem( QTreeWidget* parent );

    /**
     * Destrutor
     */
    ~FilterLogViewDeletedItem();

    /**
     * Sets the column values.
     * @param date date and time at which the mail was sent
     * @param sender sender of the mail
     * @param account account
     * @param subject mail subject
     */
    void setValues( KDateTime date, QString sender, QString account, QString subject );

    /**
     * Returns the date of sent
     * @return date of sent
     */
    KDateTime getDate();


  private:

    /**
     * sent date and time
     */
    KDateTime date;

    /**
     * sender of the mail
     */
    QString sender;

    /**
     * Account
     */
    QString account;

    /**
     * mail subject
     */
    QString subject;
};

#endif
