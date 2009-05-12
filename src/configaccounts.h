//
// C++ Interface: configaccounts
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGACCOUNTS_H
#define CONFIGACCOUNTS_H

//Qt headers
#include <qlayout.h>
#include <QTableWidget>

//KDE headers
#include <kcmodule.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kmessagebox.h>

//kshowmail headers
#include "constants.h"
#include "accountsetupdialog.h"
#include "accountsetupitem.h"

/**
 * @brief Part of the setup dialog (KShowMailApp::SetupDialog) to configure the accounts.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigAccounts : public KCModule
{
Q_OBJECT

  public:

    /**
     * Generic Constructor
     */
    ConfigAccounts( QWidget *parent = 0, const char *name = 0, const QVariantList &args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigAccounts();

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
    KConfig* config;

    /**
     * Account List View
     */
    QTableWidget* AccountListView;

    /**
     * Button to add a new account
     */
    KPushButton* btnAdd;

    /**
     * Button to edit an account
     */
    KPushButton* btnEdit;

    /**
     * Button to remove an account
     */
    KPushButton* btnRemove;

  private slots:

    /**
     * Connected with the configuration items.
     * Calls the slot changed() of KCModule to notify the dialog about the change.
     */
    void slotChanged();

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
};

#endif
