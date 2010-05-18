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

#ifndef CONFIGACCOUNTS_H
#define CONFIGACCOUNTS_H

//Qt headers
#include <qlayout.h>
#include <QTreeWidget>
#include <QPointer>

//KDE headers
#include <kcmodule.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
#include <kmessagebox.h>
#include <KPluginFactory>

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
     * @param parent parent widget
     * @param args arguments
     */
    explicit ConfigAccounts( QWidget *parent = 0, const QVariantList & args = QVariantList() );

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
    KSharedConfigPtr config;

    /**
     * Account List View
     */
    QTreeWidget* accountListView;

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
