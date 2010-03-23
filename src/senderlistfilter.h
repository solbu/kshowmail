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
