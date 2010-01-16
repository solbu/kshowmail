//
// C++ Interface: headerfilter
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HEADERFILTER_H
#define HEADERFILTER_H

//Qt headers
#include <qstring.h>
#include <qlist.h>

//KDE headers
#include <kapplication.h>
#include <kdebug.h>
#include <kconfig.h>

//KShowmail headers
#include "constants.h"
#include "types.h"
#include "filteritem.h"
#include "senderlistfilter.h"

/**
 * @brief This is the mail header filter.
 * Call check() with some header datas of the mail to get the configured action for this mail.
 * If the settings was changed by the config GUI you have to call load().
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class HeaderFilter{

  public:

    /**
     * Constructor
     */
    HeaderFilter();

    /**
     * Destructor
     */
    ~HeaderFilter();

    /**
     * Checks the given mail header.
     * @param from Sender
     * @param to Addressee
     * @param size Size
     * @param subject Subject
     * @param header Header
     * @param account Account
     * @param mailboxName contains the mailbox name after call, if filter action is MOVE
     * @return recommend action
     */
    FilterAction_Type check( QString from, QString to, uint size, QString subject, QString header, QString account, QString& mailboxName ) const;

    /**
     * Loads the settings from the application config file.
     */
    void load();

    /**
     * Prints the settings
     */
    void print();

    /**
     * Returns the filter active state.
     * @return TRUE - filter is active
     */
    bool isActive();

  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * TRUE - filter is active
     */
    bool active;

    /**
     * Default action, if no filter matches
     */
    FilterAction_Type defaultAction;

    /**
     * mailbox name if default action is MOVE
     */
    QString mailbox;

    /**
     * Number of filter items
     */
    uint numberFilterItems;

    /**
     * Black and White lists
     */
    SenderListFilter senderlist;

    /**
     * This list holds the filter items
     */
    QList<FilterItem*> filters;
};

#endif
