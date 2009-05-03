//
// C++ Interface: filtersetupitem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTERSETUPITEM_H
#define FILTERSETUPITEM_H

//KDE headers
#include <klistview.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>

//KShowmail headers
#include "../constants.h"
#include "../types.h"

using namespace Types;

/**
 * @brief This class is used in ConfigFilter to show the available filters in the list view and to store a filter setup.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterSetupItem : public KListViewItem
{

  public:

    /**
     * Column Numbers.
     */
    enum Column{ ColNumber = 0, ColName = 1, ColAction = 2 };

    /**
     * General Constructor
     * @param parent the list view
     */
    FilterSetupItem( KListView* parent );

    /**
     * Constructor
     * @param parent the list view
     * @param num filter number
     */
    FilterSetupItem( KListView* parent, uint num );

    /**
     * Destructor
     */
    ~FilterSetupItem();

    /**
     * Sets the filter name.
     * @param name filter name
     * @see name
     * @see getName
     */
    void setName( QString name );

    /**
     * Returns the name of this filter.
     * @return filter name
     * @see name
     * @see setName
     */
    QString getName() const;

    /**
     * Sets the filter number.
     * @param number filter number.
     * @see number
     * @see getNumber
     */
    void setNumber( uint number );

    /**
     * Returns the filter number.
     * @return filter number
     * @see number
     * @see setNumber
     */
    uint getNumber() const;

    /**
     * Sets the action. See constants.h for valid values (CONFIG_VALUE_FILTER_ACTION...).
     * @param action
     * @see getAction
     * @see action
     */
    void setAction( int action );

    /**
     * Returns the action of this filter.
     * See constants.h for possible values (CONFIG_VALUE_FILTER_ACTION...).
     * @return action code
     * @see setAction
     * @see action
     */
    int getAction() const;

    /**
     * Sets the mailbox of a move action.
     * @param mailbox name of the mailbox
     * @see setMailBox
     * @see mailbox
     */
    void setMailBox( QString mailbox );

    /**
     * Returns the mailbox name of a move action if this action is selected. Otherwise it returns an empty string.
     * @return mailbox name
     * @see setMailBox
     * @see mailbox
     */
    QString getMailBox() const;

    /**
     * Sets the criteria linkage.
     * Valid values are CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL (all criterias have to satisy) or CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY (at least one criteria has to satisy ).
     * @param linkage criteria linkage
     * @see linkage
     */
    void setCriteriaLinkage( int linkage );

    /**
     * Returns the criteria linkage.
     * Possible values are CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL (all criterias have to satisy) or CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY (at least one criteria has to satisy ).
     * @return criteria linkage
     * @see setCriteriaLinkage
     * @see linkage
     */
    int getCriteriaLinkage() const;

    /**
     * Sets the criteria list.
     * @param list criteria list
     * @see criteriaList
     * @see getCriteriaList
     * @see Types::FilterCriteriaList_Type
     */
    void setCriteriaList( FilterCriteriaList_Type list );

    /**
     * Returns the criteria list.
     * @return criteria list
     * @see criteriaList
     * @see setCriteriaList
     * @see Types::FilterCriteriaList_Type
     */
    FilterCriteriaList_Type getCriteriaList() const;

    /**
     * Saves the settings to the configuration file.
     * The group is named CONFIG_GROUP_FILTER + filter number, e.g. Filter12.
     */
    void save();

    /**
     * Reimplemantation of QListViewItem::compare.
     * Compares this list view item to i using the column col in ascending order. Reutrns <0 if this item is less than i,
     * 0 if they are equal and >0 if this item is greater than i. The parameter ascneding will be ignored.
     * @param i pointer to the second view item
     * @param col number of the sorted column
     * @param ascending ignored
     */
    virtual int compare( QListViewItem* i, int col, bool ascending ) const;

    /**
     * Reads the settings from the config file.
     * The group is named CONFIG_GROUP_FILTER + filter number, e.g. Filter12.
     */
    void load();


  private:

    /**
     * filter name
     */
    QString name;

    /**
     * Connector to the configuration file
     */
    KConfig* config;

    /**
     * List of filter criterias.
     */
    FilterCriteriaList_Type criteriaList;

    /**
     * Filter Action
     * See constants.h for valid values (CONFIG_VALUE_FILTER_ACTION...).
     */
    int action;

    /**
     * Contains the mailbox name if the move action is selected.
     */
    QString mailbox;

    /**
     * Filter Number.
     * The filters will be showed and run through in ascending order. The first number is "0".
     */
    uint number;

    /**
     * Criteria Linkage.
     * Valid values are CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL (all criterias have to satisy) or CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY (at least one criteria has to satisy ).
     */
    int linkage;

  protected:

    /**
     * Initializes the object.
     */
    void init();

    /**
     * Updates the text of the rule column.
     */
    void updateActionColumn();




};

#endif
