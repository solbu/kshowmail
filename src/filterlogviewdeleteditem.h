//
// C++ Interface: filterlogviewdeleteditem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
