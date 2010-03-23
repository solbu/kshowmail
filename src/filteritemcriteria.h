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

#ifndef FILTERITEMCRITERIA_H
#define FILTERITEMCRITERIA_H

//Qt headers
#include <qstring.h>
#include <qregexp.h>

//KDE headers
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <KConfigGroup>

//Kshowmail headers
#include "types.h"
#include "constants.h"

/**
 * @brief This is a criteria of a filter item.
 * For using create an object and call check().
 * It gets its configuration itself from the config file at creation time. You can't reload the setup.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterItemCriteria{

  public:

    /**
     * Constructor
     * @param FilterNr number of the filter item
     * @param CritNr number of this criteria
     */
    FilterItemCriteria( uint FilterNr, uint CritNr );

    /**
     * Destructor
     */
    ~FilterItemCriteria();

    /**
     * Checks for match.
     * @param from Sender
     * @param to Addressee
     * @param size Size
     * @param subject Subject
     * @param header Header
     * @param account Account
     * @return TRUE - the criteria matches; FALSE - the criteria doesn't match
     */
    bool check( QString from, QString to, uint size, QString subject, QStringList header, QString account ) const;

    /**
     * Prints the settings.
     */
    void print() const;

  private:

    /**
     * Types of source.
     */
    enum Source_Type{ SrcFrom, SrcTo, SrcSize, SrcSubject, SrcHeader, SrcAccount };

    /**
     * Source of the criteria.
     */
    Source_Type source;

    /**
     * Text Condition Types
     */
    enum TextCondition_Type{ TxtCondContains, TxtCondNotContains, TxtCondEqual, TxtCondNotEqual, TxtCondRegExpr, TxtCondNotRegExpr };

    /**
     * Numeric Condition Types
     */
    enum NumCondition_Type{ NumCondEqual, NumCondNotEqual, NumCondGreater, NumCondGreaterEqual, NumCondLess, NumCondLessEqual };

    /**
     * Text Condition
     */
    TextCondition_Type txtCondition;

    /**
     * Numeric Condition
     */
    NumCondition_Type numCondition;

    /**
     * Text Value
     */
    QString txtValue;

    /**
     * Numeric Value
     */
    uint numValue;

    /**
     * Case Sensitive; used by text condition
     */
    bool cs;

    /**
     * Filter number. Just for error messages.
     */
    uint FilterNumber;

    /**
     * Criteria number. Just for error messages.
     */
    uint CriteriaNumber;

  protected:

    /**
     * Compares a text value.
     * @param value Value to compare
     * @return TRUE - value matches; FALSE - value doesn't match
     */
    bool checkText( QString value ) const;

    /**
     * Compares a numeric value.
     * @param value Value to compare
     * @return TRUE - value matches; FALSE - value doesn't match
     */
    bool checkNum( uint value ) const;

    /**
     * Compares a text list
     * @param list list to compare
     * @return TRUE - value matches; FALSE - value doesn't match
     */
    bool checkTextList( QStringList list ) const;
};

#endif
