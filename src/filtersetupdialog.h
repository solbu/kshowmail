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

#ifndef FILTERSETUPDIALOG_H
#define FILTERSETUPDIALOG_H

//button IDs
#define ID_BUTTON_LINKAGE_MATCH_ALL 1
#define ID_BUTTON_LINKAGE_MATCH_ANY 2

//Qt headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <QPointer>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <kpushbutton.h>
#include <kcombobox.h>
#include <klineedit.h>

//KShowmail headers
#include "filtersetupitem.h"
#include "filtercriteriawidget.h"
#include "constants.h"
#include "mailboxwizard.h"

//IDs of the action combo box
#define ID_COMBO_FILTER_ACTION_PASS       0
#define ID_COMBO_FILTER_ACTION_DELETE     1
#define ID_COMBO_FILTER_ACTION_MARK       2
#define ID_COMBO_FILTER_ACTION_MOVE       3
#define ID_COMBO_FILTER_ACTION_SPAMCHECK  4
#define ID_COMBO_FILTER_ACTION_IGNORE     5

/**
 * @brief Dialog to setup a filter.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterSetupDialog : public KDialog
{

  Q_OBJECT

  public:
   /**
    * General constructor
    * @param parent parent of the dialog
    * @param item filter to setup;
    */
    explicit FilterSetupDialog( QWidget* parent, FilterSetupItem* item = NULL );

    /**
     * Destructor
     */
    ~FilterSetupDialog();

  private:

   /**
    * filter to setup
    */
    FilterSetupItem* filter;

    /**
     * View which shows the filter items.
     */
    QTreeWidget* listView;

    /**
     * Contains the filter name.
     */
    KLineEdit* txtName;

    /**
     * Combines the radio buttons to select the kind of linkage between the criterias.
     */
    QButtonGroup* grpLinkage;

    /**
     * This widget holds one ore more filter criteria dialogs (class FilterCriteriaWidget).
     */
    QWidget* wdgCriteriasHolding;

    /**
     * Layout for wdgCriterias.
     */
    QVBoxLayout* layCriteriasHolding;

    /**
     * Button to add a criteria dialog.
     */
    KPushButton* btnAddCriteria;

    /**
     * Button to remove the last criteria.
     */
    KPushButton* btnRemoveCriteria;

    /**
     * Combobox to select the action.
     */
    KComboBox* cmbAction;

    /**
     * If selected, the mails will be moved to the mailbox which is written in this edit line.
     */
    KLineEdit* txtMailbox;

    /**
     * This button opens a wizard which will help to get the right mailbox
     */
    KPushButton* btnMailboxWizard;


  protected:

    /**
     * Writes the values of the given filter into the dialog items.
     */
    void fillDialog();

    /**
     * Adds a criteria widget dialog with values for numeric comparison.
     * @param source Object of Comparison. It accepts only numeric objects (e.g. Size) (see constants.h for valid values)
     * @param condition Condition of the comparison (see constants.h for valid values)
     * @param value comparison value
     */
    void addCriteriaWidget( int source, int condition, uint value );

    /**
     * Adds a criteria widget dialog with values for text comparison.
     * @param source Object of Comparison. It accepts only text objects (e.g. Subject) (see constants.h for valid values)
     * @param condition Condition of the comparison (see constants.h for valid values)
     * @param value comparison string
     * @param cs TRUE - comparison is case sensitive; FALSE - comparison is not case sensitive
     */
    void addCriteriaWidget( int source, int condition, QString value, bool cs );


  protected slots:

    /**
     * Overwritten method of KDialog.
     * Called if a button is clocked clicked.
     * Stores the entered values into the given account item.
     * If no item was given it creates a new one.
     * After then it invokes slotButtonClicked() of KDialog.
     * @param button clicked button
     */
    void slotButtonClicked( int button );

    /**
     * Connected with combobox cmbAction.
     * Enables or disables additional widgets for action configuration.
     * @param index position (index) of the selected action
     */
    void slotActionChanged( int index );

    /**
     * Adds a empty critera dialog widget.
     * @return Pointer to the new widget
     */
    FilterCriteriaWidget* slotAddCriteriaWidget();

    /**
     * Removes the last criteria dialog widget.
     */
    void slotRemoveCriteriaWidget();

    /**
     * Connected with button btnMailboxWizard.
     * Opens a wizard to configure the mailbox
     */
    void slotOpenMailBoxWizard();
};

#endif
