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

#ifndef ACCOUNTSETUPDIALOG_H
#define ACCOUNTSETUPDIALOG_H

//Qt headers
#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <QTreeWidget>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <KPasswordDialog>
#include <ktabwidget.h>

//KShowmail headers
#include "accountsetupitem.h"
#include "constants.h"

//radio button IDs
#define ID_BUTTON_PASSWORD_DONT_SAVE 1
#define ID_BUTTON_PASSWORD_SAVE_FILE 2
#define ID_BUTTON_PASSWORD_SAVE_KWALLET 3
#define ID_BUTTON_SECLOGIN_NONE 1
#define ID_BUTTON_SECLOGIN_APOP 2
#define ID_BUTTON_SECLOGIN_SASL 3
#define ID_BUTTON_SECTRANSFER_NONE 1
#define ID_BUTTON_SECTRANSFER_SSL 2
#define ID_BUTTON_SECTRANSFER_TLS 3

/**
 * @brief Dialog to setup an account.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class AccountSetupDialog : public KDialog
{
Q_OBJECT

  public:

    /**
     * @brief General constructor
     * @param parent parent of the dialog
     * @param view view which shows the items
     * @param item account to setup; if NULL, a new item will be created
     */
    AccountSetupDialog( QWidget* parent, QTreeWidget* view, AccountSetupItem* item = NULL );

    /**
     * @brief Destructor
     */
    ~AccountSetupDialog();

  private:

    /**
     * @brief account to setup
     */
    AccountSetupItem* account;

    /**
     * @brief View which shows the account items.
     */
    QTreeWidget* listView;

    /**
     * @brief Edit line which contains the unique account name.
     */
    KLineEdit* txtAccount;

    /**
     * @brief Edit line which contains the server name.
     */
    KLineEdit* txtServer;

    /**
     * @brief Combo box to choose the protocol.
     */
    KComboBox* cboProtocol;

    /**
     * @brief Spinbox to choose the tcp port.
     */
    QSpinBox* spbPort;

    /**
     * @brief Edit line which contains the user name.
     */
    KLineEdit* txtUser;

    /**
     * @brief Edit line which contains the password.
     */
    KLineEdit* txtPassword;

    /**
     * @brief Check box to select whether the account is active or not.
     */
    QCheckBox* chkActive;

    /**
     * @brief Combines the radio buttons of password storage.
     */
    QButtonGroup* grpPasswordStorage;

    /**
     * @brief Combines the radio buttons of secure transfer.
     */
    QButtonGroup* grpSecureTransfer;

    /**
     * @brief Check box to select whether unsecure login is allowed
     */
    QCheckBox* chkAllowUnsecureLogin;

    

  protected slots:

    /**
     * @brief Connected with the password button group.
     *
     * Enabled or disabled the password edit line.
     * @param id button identifier
     */
    void slotPasswordStorageChanged( int id );

    /**
     * @brief Connected with the secure transfer button group.
     *
     * Changes the port number.
     * @param id button identifier
     */
    void slotSecureTransferChanged( int id );

  protected Q_SLOTS:
    /**
     * @brief Overwritten method of KDialog.
     * Called if a button was clicked.
     *
     * If OK was  clicked it stores the entered values into the given account item.
     * If no item was given it creates a new one.
     * @param button is the type KDialog::ButtonCode
     */
    virtual void slotButtonClicked( int button );

  protected:

    /**
     * @brief Writes the values of the account into the dialog items.
     */
    void fillDialog();
		
		/**
		 * @brief Enables or disables the unsecure login checkbox depending on the
		 * chosen security.
		 */
		void enableLoginCheckbox();

};

#endif
