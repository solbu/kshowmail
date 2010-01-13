//
// C++ Interface: filterlogviewmoveditem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTERLOGVIEWMOVEDITEM_H
#define FILTERLOGVIEWMOVEDITEM_H

//Qt Headers
#include <qdatetime.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>

//KDE headers

/**
 * @brief Item of the filter log view of moved mails
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterLogViewMovedItem : public QTreeWidgetItem
{

  public:

  /**
   * Column Numbers.
   */
    enum Column{ ColDate = 0, ColSender = 1, ColAccount = 2, ColMailbox = 3, ColSubject = 4 };

    /**
     * Constructor
     * @param parent the log view of this items
     */
    FilterLogViewMovedItem( QTreeWidget* parent);

    /**
     * Destructor
     */
    ~FilterLogViewMovedItem();

    /**
     * Sets the column values.
     * @param date date and time at which the mail was sent
     * @param sender sender of the mail
     * @param account account
     * @param mailbox mailbox to which the mail was moved
     * @param subject mail subject
     */
    void setValues( QDateTime date, QString sender, QString account, QString mailbox, QString subject );

    /**
     * Returns the date of sent
     * @return date of sent
     */
    QDateTime getDate();


  private:

    /**
   * sent date and time
     */
    QDateTime date;

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

    /**
     * mailbox
     */
    QString mailbox;
};

#endif
