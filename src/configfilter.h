//
// C++ Interface: configfilter
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGFILTER_H
#define CONFIGFILTER_H

//QT headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <QTreeWidget>

//KDE headers
#include <kcmodule.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <kpushbutton.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <KConfigGroup>
#include <KStandardDirs>

//Kshowmail headers
#include "constants.h"
#include "senderlistdialog.h"
#include "filtersetupitem.h"
#include "filtersetupdialog.h"
//#include "mailboxwizard.h"

//radio button IDs
#define ID_COMBO_FILTER_OTHERS_ACTION_PASS       0
#define ID_COMBO_FILTER_OTHERS_ACTION_DELETE     1
#define ID_COMBO_FILTER_OTHERS_ACTION_MARK       2
#define ID_COMBO_FILTER_OTHERS_ACTION_MOVE       3
#define ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK  4
#define ID_COMBO_FILTER_OTHERS_ACTION_IGNORE     5


/**
 * @brief Part of the setup dialog (KShowMailApp::SetupDialog) to configure the filter.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigFilter : public KCModule
{
Q_OBJECT

  public:

    /**
     * Generic constructor
     */
    ConfigFilter( QWidget *parent = 0, const QVariantList & args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigFilter();

    /**
     * Overloaded method of KCModule.
     * Sets the user interface elements to reflect the current settings stored in the
     * config file.
     */
    virtual void load();

    /**
     * Overloaded method of KCModule.
     * Sets the user interface elements to default values.
     * This method is called when the user clicks the "Default" button.
     */
    virtual void defaults();

    /**
     * Overloaded method of KCModule.
     * Stores the config information as shown in the user interface in the
     * config file.
     * Is called when the user clicks "Apply" or "Ok".
     */
    virtual void save();

  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * Check box to activate the filter
     */
    QCheckBox* chkActivateFilter;

    /**
     * Group box for sender lists.
     */
    QGroupBox* gboxSenderLists;

    /**
     * Group box for filter items.
     */
    QGroupBox* gboxFilters;

    /**
     * Group box for other action.
     */
    QGroupBox* gboxOthers;

    /**
     * Button to open the Blacklist.
     */
    KPushButton* btnOpenBlacklist;

    /**
     * Button to open the Whitelist.
     */
    KPushButton* btnOpenWhitelist;

    /**
     * List of filters
     */
    QTreeWidget* listFilters;

    /**
     * Button to move a filter at top.
     */
    KPushButton* btnMoveTop;

    /**
     * Button to move a filter at bottom.
     */
    KPushButton* btnMoveBottom;

    /**
     * Button to move a filter up.
     */
    KPushButton* btnMoveUp;

    /**
     * Button to move a filter down.
     */
    KPushButton* btnMoveDown;

    /**
     * Button to add a new filter.
     */
    KPushButton* btnAdd;

    /**
     * Button to edit a filter.
     */
    KPushButton* btnEdit;

    /**
     * Button to remove a filter.
     */
    KPushButton* btnRemove;

    /**
     * Combobox to select the action for the rest of mails.
     */
    KComboBox* cmbActionOthers;

    /**
     * If selected, the other mails will be moved to the mailbox which is written in this edit line.
     */
    KLineEdit* txtMailbox;

    /**
     * This button opens a wizard which will help to get the right mailbox
     */
    KPushButton* btnMailboxWizard;

    /**
     * The last assigned number of a filter setup item.
     * It is set to zero by the constructor. Therefore the first filter number is 1.
     */
    uint lastFilterNumber;

  private slots:

   /**
    * Connected with the configuration items.
    * Calls the slot changed() of KCModule to notify the dialog about the change.
    */
    void slotChanged();

    /**
     * Connected with the button btnOpenWhitelist.
     * Creates and opens a dialog to edit the whitelist.
     */
    void slotOpenWhitelist();

    /**
     * Connected with the button btnOpenBlacklist.
     * Creates and opens a dialog to edit the blacklist.
     */
    void slotOpenBlacklist();

    /**
     * Connected with button btnAdd.
     * Opens the account setup dialog with an account item pointer of NULL.
     * If the user has clicked OK, the dialog will create a new account item with
     * the entered values and adds it to the list view.
     */
    void slotAdd();

    /**
     * Connected with button btnEdit.
     * Opens the account setup with the selected account item.
     */
    void slotEdit();

    /**
     * Connected with button btnRemove.
     * Removes the selected item.
     */
    void slotRemove();

    /**
     * Connected with combobox cmbActionOthers.
     * Enables or disables additional widgets for action configuration.
     * @param index position (index) of the selected action
     */
    void slotOtherActionChanged( int index );

    /**
     * Connected with button btnMoveTop.
     * Moves the selected filter item at the top of the list, i.e. the number of this item will be the least one.
     */
    void slotMoveTop();

    /**
     * Connected with button btnMoveBottom.
     * Moves the selected filter item at the bottom of the list, i.e. the number of this item will be the greatest one.
     */
    void slotMoveBottom();

    /**
     * Connected with button btnMoveUp.
     * Moves the selected filter item up, i.e. the number of this item will be decreased.
     */
    void slotMoveUp();

    /**
     * Connected with button btnMoveDown.
     * Moves the selected filter item down, i.e. the number of this item will be increased.
     */
    void slotMoveDown();

    /**
     * Connected with chkActivateFilter, signal toggled().
     * Enabled or disabled the other widget of this GUI.
     * @param filterOn TRUE - filter is active; FALSE - filter is not active
     */
    void slotFilterActiveToggled( bool filterOn );

    /**
     * Connected with button btnMailboxWizard.
     * Opens a wizard to configure the mailbox
     */
    void slotOpenMailBoxWizard();


  protected:

    /**
     * Decrease the numbers of these filter setup items whose number is greater than or equals num.
     * @param number start number of renumbering
     */
    void decreaseNumbers( uint number );

    /**
     * Returns the pointer to the filter setup item with number num. Returns 0 if no item found.
     * @param num number of the searched filter item
     * @return pointer to the found item
     */
//    FilterSetupItem* getFilterItem( uint num );

};

#endif
