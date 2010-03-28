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

#ifndef CONFIGSPAMCHECK_H
#define CONFIGSPAMCHECK_H

//C-headers
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Qt headers
#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qtooltip.h>
#include <QVariantList>
#include <QPointer>

//KDE headers
#include <kcmodule.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <kmessagebox.h>

//Kshowmail headers
#include "constants.h"
#include "mailboxwizard.h"

//radio button IDs
#define ID_SPAM_ACTION_BUTTONS_DELETE     0
#define ID_SPAM_ACTION_BUTTONS_MARK       1
#define ID_SPAM_ACTION_BUTTONS_MOVE       2


/**
 * @brief Part of the setup dialog (KShowMailApp::SetupDialog) to configure the spam check.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigSpamcheck : public KCModule
{

Q_OBJECT


  public:
    /**
     * Generic constructor
     */
    explicit ConfigSpamcheck( QWidget* parent = 0, const QVariantList& args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigSpamcheck();

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
     * Button to test for a running SpamAssassin
     */
    KPushButton* btnTest;

    /**
     * Combobox to select the action for spam.
     */
    KComboBox* cmbAction;

    /**
     * If selected, the spam mails will be moved to the mailbox which is written in this edit line.
     */
    KLineEdit* txtMailbox;

    /**
     * This button opens a wizard which will help to get the right mailbox
     */
    KPushButton* btnMailboxWizard;

    /**
     * Contains the widgets of action configuration.
     */
    QGroupBox* gboxAction;

  private slots:

   /**
    * Connected with the configuration items.
    * Calls the slot changed() of KCModule to notify the dialog about the change.
    */
    void slotChanged();

    /**
     * Connected with combobox cmbAction.
     * Enables or disables additional widgets for action configuration.
     * @param index position (index) of the selected action
     */
    void slotActionChanged( int index );

    /**
     * Connected with button btnMailboxWizard.
     * Opens a wizard to configure the mailbox
     */
    void slotOpenMailBoxWizard();

    /**
     * Connected with btnTest.
     * Tests whether SpamAssassin is running and shows a message box containing the result.
     * Enables or disables the action widgets.
     */
    void slotTestSpamAssassin();

  protected:

    /**
     * Looks for a running spamd daemon of SpamAssassin.
     * @return TRUE - SpamAssassin is running
     */
    bool isSpamAssassinRunning() const;

};

#endif
