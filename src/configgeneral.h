//
// C++ Interface: configgeneral
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGGENERAL_H
#define CONFIGGENERAL_H

//Qt headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <qlabel.h>

//KDE headers
#include <kcmodule.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <KConfigGroup>

//KShowmail headers
#include "constants.h"

/**
 * @brief Part of the setup dialog (KShowMailApp::SetupDialog) to configure general things.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigGeneral : public KCModule
{
Q_OBJECT

  public:

    /**
     * Generic Constructor
     * @param parent parent widget
     * @param args arguments
     */
    ConfigGeneral( QWidget *parent = 0, const QVariantList & args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigGeneral();

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
     * Check box to select whether window closing must be confirmed.
     */
    QCheckBox* chkConfirmClose;

    /**
     * Check box to select whether deleting must be confirmed.
     */
    QCheckBox* chkConfirmDelete;

    /**
     * Check box to select whether the main window will be minimized at start.
     */
    QCheckBox* chkStartMinimized;

    /**
     * Check box to select whether the close button leave the application running in tray.
     */
    QCheckBox* chkCloseToTray;

    /**
     * Check box to select whether connection errors will be shown.
     */
    QCheckBox* chkShowConnectionErrors;

    /**
     * Check box to select whether mails will be kept as new until termination.
     */
    QCheckBox* chkKeepNew;

    /**
     * Check box to select automatic refresh
     */
    QCheckBox* chkAutomaticRefresh;

    /**
     * Spin box to set the initial timer.
     */
    QSpinBox* spbInitial;

    /**
     * Spin box to set the interval timer.
     */
    QSpinBox* spbInterval;

    /**
     * Label for interval timer
     */
    QLabel* lblTimerInterval;

    /**
     * Label for initial timer
     */
    QLabel* lblTimerInitial;

    /**
     * Spin box to set the time out timer.
     */
    QSpinBox* spbTimeout;

  public slots:

    /**
     * Connected with the configuration items.
     * Calls the slot changed() of KCModule to notify the dialog about the change.
     */
    void slotChanged();

    /**
     * Connected with the check box of automatic refresh.<p>
     * Enables or disables the refresh timer spin boxes
     * @param checked TRUE - box is checked; FALSE - box is not checked
     */
    void slotAutomaticRefreshChanged( bool checked );

};

#endif
