//
// C++ Interface: accountsetupdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
     * General constructor
     * @param parent parent of the dialog
     * @param view view which shows the items
     * @param item account to setup; if NULL, a new item will be created
     */
    AccountSetupDialog( QWidget* parent, QTreeWidget* view, AccountSetupItem* item = NULL );

    /**
     * Destructor
     */
    ~AccountSetupDialog();

  private:

    /**
     * account to setup
     */
    AccountSetupItem* account;

    /**
     * View which shows the account items.
     */
    QTreeWidget* listView;

    /**
     * Edit line which contains the unique account name.
     */
    KLineEdit* txtAccount;

    /**
     * Edit line which contains the server name.
     */
    KLineEdit* txtServer;

    /**
     * Combo box to choose the protocol.
     */
    KComboBox* cboProtocol;

    /**
     * Spinbox to choose the tcp port.
     */
    QSpinBox* spbPort;

    /**
     * Edit line which contains the user name.
     */
    KLineEdit* txtUser;

    /**
     * Edit line which contains the password.
     */
    KLineEdit* txtPassword;

    /**
     * Check box to select whether the account is active or not.
     */
    QCheckBox* chkActive;

    /**
     * Combines the radio buttons of password storage.
     */
    QButtonGroup* grpPasswordStorage;

    /**
     * Combines the radio buttons of secure transfer.
     */
    QButtonGroup* grpSecureTransfer;

  protected slots:

    /**
     * Connected with the password button group.
     * Enabled or disabled the password edit line.
     * @param id button identifier
     */
    void slotPasswordStorageChanged( int id );

    /**
     * Connected with the secure transfer button group.
     * Changes the port number.
     * @param id button identifier
     */
    void slotSecureTransferChanged( int id );

    /**
     * Overwritten method of KDialogBase.
     * Called if OK was clicked.
     * Stores the entered values into the given account item.
     * If no item was given it creates a new one.
     * After then it invokes slotOk() of KDialogBase.
     */
    void slotOk();

  protected:

    /**
     * Writes the values of the account into the dialog items.
     */
    void fillDialog();

};

#endif
