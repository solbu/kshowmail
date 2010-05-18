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

#ifndef FILTERCRITERIAWIDGET_H
#define FILTERCRITERIAWIDGET_H

//Qt headers
#include <qwidget.h>
#include <qlayout.h>
#include <qcheckbox.h>

//KDE headers
#include <klineedit.h>
#include <kseparator.h>
#include <kdialog.h>
#include <kcombobox.h>
#include <klocale.h>
#include <knuminput.h>
#include <kpushbutton.h>
#include <kregexpeditorinterface.h>
#include <ktrader.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kparts/componentfactory.h>

//KShowmail headers
#include <constants.h>
#include <types.h>

//ComboBox IDs
#define ID_COMBO_SOURCE_FROM    0
#define ID_COMBO_SOURCE_TO      1
#define ID_COMBO_SOURCE_SIZE    2
#define ID_COMBO_SOURCE_SUBJECT 3
#define ID_COMBO_SOURCE_HEADER  4
#define ID_COMBO_SOURCE_ACCOUNT 5

#define ID_COMBO_COND_TEXT_CONTAINS       0
#define ID_COMBO_COND_TEXT_NOT_CONTAINS   1
#define ID_COMBO_COND_TEXT_EQUALS         2
#define ID_COMBO_COND_TEXT_NOT_EQUALS     3
#define ID_COMBO_COND_TEXT_REGEXPR        4
#define ID_COMBO_COND_TEXT_NOT_REGEXPR    5

#define ID_COMBO_COND_NUM_EQUAL         0
#define ID_COMBO_COND_NUM_NOT_EQUAL     1
#define ID_COMBO_COND_NUM_GREATER       2
#define ID_COMBO_COND_NUM_GREATER_EQUAL 3
#define ID_COMBO_COND_NUM_LESS          4
#define ID_COMBO_COND_NUM_LESS_EQUAL    5

#define FILTER_CRITERIA_WIDGET_NAME "FilterCriteriaWidg"

using namespace Types;

/**
 * @brief Part of the filter setup dialog (class FilterSetupDialog) to show a criteria of the selected filter.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterCriteriaWidget : public QWidget
{

Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent widget
     */
    FilterCriteriaWidget( QWidget *parent = 0 );

    /**
     * Destructor
     */
    ~FilterCriteriaWidget();

    /**
     * Sets the values of a numeric criteria.
     * @param source Object of Comparison (see constants.h for valid values)
     * @param condition Condition of the comparison (see constants.h for valid values)
     * @param value comparison value
     */
    void setNumCriteria( int source, int condition, uint value );

    /**
     * Sets the values of a text criteria.
     * @param source Object of Comparison (see constants.h for valid values)
     * @param condition Condition of the comparison (see constants.h for valid values)
     * @param value comparison string
     * @param cs TRUE - comparison is case sensitive; FALSE - comparison is not case sensitive
     */
    void setTextCriteria( int source, int condition, QString value, bool cs );

    /**
     * Sets the values of a text list criteria.
     * @param source Object of Comparison (see constants.h for valid values)
     * @param condition Condition of the comparison (see constants.h for valid values)
     * @param value comparison string
     * @param cs TRUE - comparison is case sensitive; FALSE - comparison is not case sensitive
     */
    void setTextListCriteria( int source, int condition, QString value, bool cs );

    /**
     * Returns the values of the criteria.
     * @return criteria values
     */
    FilterCriteria_Type getValues();

  private:

    /**
     * Combobox to select the source of the criteria.
     */
    KComboBox* cmbSource;

    /**
     * Combobox to select the condition of a text source (From, To, ...)
     */
    KComboBox* cmbConditionText;

    /**
     * Combobox to select the condition of a text list source (Header)
     */
    KComboBox* cmbConditionTextList;

    /**
     * Combobox to select the condition of a numeric source (Size, ...)
     */
    KComboBox* cmbConditionNum;

    /**
     * Line for a text comparative value.
     */
    KLineEdit* txtCompValueText;

    /**
     * Spin Box for a numeric comparative value.
     */
    KIntNumInput* spbCompValueNum;

    /**
     * Checkbox to select case sensitive.
     */
    QCheckBox* chkCaseSensitive;

    /**
     * Button to open the regular expression editor.
     */
    KPushButton* btnOpenRegExpEditor;

    /**
     * TRUE - KRegExpEditor is installed and available.
     */
    bool kRegExpEditorAvailable;


  private slots:

    /**
     * Connected with combobox cmbSource.
     * Hides or shows the widgets dependent on the criteria source (text or numeric).
     */
    void slotSetWidgets();

    /**
     * Opens the editor for regular expressions.
     */
    void slotOpenRegExpEditor();
};

#endif
