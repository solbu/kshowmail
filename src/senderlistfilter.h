//
// C++ Interface: senderlistfilter
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SENDERLISTFILTER_H
#define SENDERLISTFILTER_H

//Qt headers
#include <qstring.h>
#include <qstringlist.h>

//KDE headers
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <KConfigGroup>

//Kshowmail headers
#include "types.h"
#include "constants.h"

using namespace Types;

/**
 * @brief This class represents both the blacklist and the whitelist.
 * For using create an object of it and call check() to test a sender address.
 * You have to call load() after the settings in the config file was changed.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */

class SenderListFilter{

  public:

    /**
     * Constructor
     */
    SenderListFilter();

    /**
     * Destructor
     */
    ~SenderListFilter();

    /**
     * Checks the given sender whether it is listed in the blacklist or whitelist.
     * @param sender sender which shall be checked
     * @return appointed action (FActPass, FActDelete, FActMark, FActNone)
     */
    FilterAction_Type check( QString sender ) const;

    /**
     * Loads the setup from the application config file.
     */
    void load();

    /**
     * Prints the settings.
     */
    void print();

  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * blacklisted addresses
     */
    QStringList blacklist;

    /**
     * whitelisted addresses
     */
    QStringList whitelist;

    /**
     * appointed action of the blacklist.
     */
    FilterAction_Type blacklistAction;

  protected:

    /**
     * Returns TRUE if the given list contains the sender or a part of it.
     * e.g.: The list contains:
     * Ulrich Weigelt
     * spam@spamhouse.com
     * "Lara Croft" <lara.croft@tombraider.com>
     *
     * This sender strings will cause a return value of TRUE:
     * "Ulrich Weigelt" <ulrich.weigelt@gmx.de>
     * "Spam King" <spam@spamhouse.com>
     * spam@spamhouse.com
     * lara.croft@tombraider.com
     * "Lara Croft" <lara.croft@tombraider.com>
     *
     * @param list blacklist or whitelist
     * @param sender searched sender name
     * @return TRUE - the given sender name is listed. FALSE - is not listed
     */
    bool search( QStringList list, QString sender ) const;
};

#endif
