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

#ifndef CONFIGLOG_H
#define CONFIGLOG_H

//button IDs
#define ID_BUTTON_REMOVE_AT_EXIT    1
#define ID_BUTTON_REMOVE_AFTER_DAYS 2

//qt headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qspinbox.h>
#include <QStyle>

//KDE headers
#include <kcmodule.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <kconfiggroup.h>

//Kshowmail headers
#include "constants.h"


/**
 * @brief Part of the setup dialog (KShowMailApp::SetupDialog) to configure the log.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigLog : public KCModule
{
Q_OBJECT

  public:

    /**
     * Generic constructor
     * @param parent parent widget
     * @param args arguments
     */
    explicit ConfigLog( QWidget *parent = 0, const QVariantList & args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigLog();

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
   * Checkbox to activate log of deleted mails
   */
  QCheckBox* chkLogDeletedMails;

  /**
   * Checkbox to activate log of moved mails
   */
  QCheckBox* chkLogMovedMails;

  /**
   * Checkbox to activate log of manual deleted mails
   */
  QCheckBox* chkLogManualDeletedMails;

  /**
   * Combines the radio buttons to select the kind of removing log entries of deleted mails
   */
  QButtonGroup* grpDelMailsRemove;

  /**
   * Combines the radio buttons to select the kind of removing log entries of moved mails
   */
  QButtonGroup* grpMovMailsRemove;

  /**
   * Combines the radio buttons to select the kind of removing log entries of manual deleted mails
   */
  QButtonGroup* grpManualDelMailsRemove;

  /**
   * Spinbox to select the number of days to remove.
   */
  QSpinBox* spbDelDays;

  /**
   * Spinbox to select the number of days to remove.
   */
  QSpinBox* spbMovDays;

  /**
   * Spinbox to select the number of days to remove the logs of manual deleted mails.
   */
  QSpinBox* spbManualDelDays;

  /**
   * Radio button to select the removing of entries about deleted mails at application exit.
   */
  QRadioButton* btnDelMailsRemoveExit;

  /**
   * Radio button to select the removing of entries about deleted mails after some days.
   */
  QRadioButton* btnDelMailsRemoveDays;

  /**
   * Radio button to select the removing of entries about moved mails at application exit.
   */
  QRadioButton* btnMovMailsRemoveExit;

  /**
   * Radio button to select the removing of entries about moved mails after some days.
   */
  QRadioButton* btnMovMailsRemoveDays;

  /**
   * Radio button to select the removing of entries about manual deleted mails at application exit.
   */
  QRadioButton* btnManualDelMailsRemoveExit;

  /**
   * Radio button to select the removing of entries about manual deleted mails after some days.
   */
  QRadioButton* btnManualDelMailsRemoveDays;

private slots:

  /**
   * Connected with the configuration items.
   * Calls the slot changed() of KCModule to notify the dialog about the change.
   */
    void slotChanged();

  /**
   * Connected with the checkboxes and radiobuttons.
   * Enables or disables the dialog items.
   */
  void slotChangeItems();
};

#endif
