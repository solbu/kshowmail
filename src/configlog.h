//
// C++ Interface: configlog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
     */
    ConfigLog( QWidget *parent = 0, const QVariantList & args = QVariantList() );

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
   * Combines the radio buttons to select the kind of removing log entries of deleted mails
   */
  QButtonGroup* grpDelMailsRemove;

  /**
   * Combines the radio buttons to select the kind of removing log entries of moved mails
   */
  QButtonGroup* grpMovMailsRemove;

  /**
   * Spinbox to select the number of days to remove.
   */
  QSpinBox* spbDelDays;

  /**
   * Spinbox to select the number of days to remove.
   */
  QSpinBox* spbMovDays;

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
