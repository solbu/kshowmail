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

#ifndef FILTERITEM_H
#define FILTERITEM_H

//Qt headers
#include <qstring.h>
#include <qlist.h>

//KDE headers
#include <kapplication.h>
#include <kdebug.h>
#include <kconfig.h>
#include <KConfigGroup>

//KShowmail headers
#include "filteritemcriteria.h"
#include "constants.h"
#include "types.h"

using namespace Types;

/**
 * @brief This is a filter item. It contains a list of criterias (class FilterItemCriteria).
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterItem{

  public:

    /**
     * Constructor
     * Loads the settings from the config file
     * @param filterNr Number of the filter
     */
    FilterItem( uint filterNr );

    /**
     * Destructor
     */
    ~FilterItem();

    /**
     * Prints the settings.
     */
    void print() const;

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
    FilterAction_Type check( QString from, QString to, uint size, QString subject, QStringList header, QString account, QString& mailboxName ) const;

    /**
     * Returns the name of this filter
     *
     * @return name
     */
    QString getName() const;

  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * Filter number. Just for messages.
     */
    uint filterNumber;

    /**
     * Filter name. Just for messages.
     */
    QString name;

    /**
     * Number of criterias
     */
    uint numberCriterias;

    /**
     * Type of criteria linkage
     */
    enum Linkage_Type{ LinkAll, LinkAny };

    /**
     * Criteria Linkage
     */
    Linkage_Type linkage;

    /**
     * Filter Action
     */
    FilterAction_Type action;

    /**
     * Mailbox Name for move action
     */
    QString mailbox;

    /**
     * This list holds the criterias.
     */
    QList<FilterItemCriteria*> criterias;
};

#endif
