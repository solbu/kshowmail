//
// C++ Implementation: accountsetupdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "accountsetupdialog.h"

AccountSetupDialog::AccountSetupDialog( QWidget* parent, KListView* view, AccountSetupItem* item )
  : KDialogBase( parent, "AccountSetupDialog", true, QString::null, Ok|Cancel, Ok, true )
{
  //save pointer to account and view
  account = item;
  ListView = view;

  //tab widget
  KTabWidget* tabs = new KTabWidget( this, "tabs" );
  tabs->setMargin( 10 );

  //pages
  QWidget* pgGeneral = new QWidget( this );
  QWidget* pgSecurity = new QWidget( this );
  setMainWidget( tabs );

  //layouts of general page
  QVBoxLayout* layGeneral = new QVBoxLayout( pgGeneral, 0, spacingHint() );
  QGridLayout* layTop = new QGridLayout( layGeneral, 5, 2 );

  //layouts of security page
  QVBoxLayout* laySecurity = new QVBoxLayout( pgSecurity, 0, spacingHint() );
  laySecurity->setAlignment( Qt::AlignTop );

  //upper items
  QLabel* lblAccount = new QLabel( i18n( "Account:" ), pgGeneral, "lblAccount" );
  txtAccount = new KLineEdit( pgGeneral, "txtAccount" );
  txtAccount->setFocus();
  QToolTip::add( lblAccount, i18n( "Unique Account Name" ) );
  QToolTip::add( txtAccount, i18n( "Unique Account Name" ) );
  layTop->addWidget( lblAccount, 0, 0 );
  layTop->addWidget( txtAccount, 0, 1 );

  QLabel* lblServer = new QLabel( i18n( "Server:" ), pgGeneral, "lblServer" );
  txtServer = new KLineEdit( pgGeneral, "txtServer" );
  QToolTip::add( lblServer, i18n( "Server Name" ) );
  QToolTip::add( txtServer, i18n( "Server Name" ) );
  layTop->addWidget( lblServer, 1, 0 );
  layTop->addWidget( txtServer, 1, 1 );

  QLabel* lblProtocol = new QLabel( i18n( "Protocol:" ), pgGeneral, "lblProtocol" );
  cboProtocol = new KComboBox( pgGeneral, "cboProtocol" );
  cboProtocol->insertItem( "POP3" );  //currently KShowmail just supports POP3
  QToolTip::add( lblProtocol, i18n( "Protocol, which shall be used to get the mails from the server. Currently KShowmail just supports POP3.") );
  QToolTip::add( cboProtocol, i18n( "Protocol, which shall be used to get the mails from the server. Currently KShowmail just supports POP3.") );
  layTop->addWidget( lblProtocol, 2, 0 );
  layTop->addWidget( cboProtocol, 2, 1 );

  QLabel* lblPort = new QLabel( i18n( "Port:" ), pgGeneral, "lblPort" );
  spbPort = new QSpinBox( 0, 65535, 1, pgGeneral, "spbPort" );
  spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 );
  QToolTip::add( lblPort, i18n( "Port Number. Normally POP3 uses port 110." ) );
  QToolTip::add( spbPort, i18n( "Port Number. Normally POP3 uses port 110." ) );
  layTop->addWidget( lblPort, 3, 0 );
  layTop->addWidget( spbPort, 3, 1 );

  QLabel* lblUser = new QLabel( i18n( "User:" ), pgGeneral, "lblUser" );
  txtUser = new KLineEdit( pgGeneral, "txtUser" );
  QToolTip::add( lblUser, i18n( "To authenticate to the mail server you need an user name." ) );
  QToolTip::add( txtUser, i18n( "To authenticate to the mail server you need an user name." ) );
  layTop->addWidget( lblUser, 4, 0 );
  layTop->addWidget( txtUser, 4, 1 );

  //password groupbox and layouts
  QGroupBox* gboxPassword = new QGroupBox( 0, Qt::Horizontal, i18n( "Password" ), pgGeneral, "gboxPassword" );
  layGeneral->addWidget( gboxPassword );

  QVBoxLayout* layPassword = new QVBoxLayout( gboxPassword->layout(), spacingHint() );
  QGridLayout* layPasswordStorage = new QGridLayout( layPassword, 2, 2, spacingHint() );


  //radio buttons to set storage of the password
  grpPasswordStorage = new QButtonGroup( NULL, "grpPasswordStorage" );
  connect( grpPasswordStorage, SIGNAL( clicked( int ) ), this, SLOT( slotPasswordStorageChanged( int ) ) );

  QRadioButton* btnPasswordDontSave = new QRadioButton( i18n( "Don't save" ), gboxPassword, "btnPasswordDontSave" );
  QRadioButton* btnPasswordSaveFile = new QRadioButton( i18n( "Save password"), gboxPassword, "btnPasswordSaveFile" );
  QRadioButton* btnPasswordSaveKWallet = new QRadioButton( i18n( "Use KWallet" ), gboxPassword, "btnPasswordSaveKWallet" );
  grpPasswordStorage->insert( btnPasswordDontSave, ID_BUTTON_PASSWORD_DONT_SAVE );
  grpPasswordStorage->insert( btnPasswordSaveFile, ID_BUTTON_PASSWORD_SAVE_FILE );
  grpPasswordStorage->insert( btnPasswordSaveKWallet, ID_BUTTON_PASSWORD_SAVE_KWALLET );
  QToolTip::add( btnPasswordDontSave, i18n( "Don't save password. KShowmail will ask you for it at first server connect." ) );
  QToolTip::add( btnPasswordSaveFile, i18n( "Save password in the configuration file. Not recommended, because the password is just lightly encrypted" ) );
  QToolTip::add( btnPasswordSaveKWallet, i18n( "Use KWallet to save the password. Maybe you have to type in the KWallet master password at first server connect." ) );
  layPasswordStorage->addWidget( btnPasswordDontSave, 0, 0 );
  layPasswordStorage->addWidget( btnPasswordSaveFile, 0, 1 );
  layPasswordStorage->addWidget( btnPasswordSaveKWallet, 1, 0 );

  //password edit line
  txtPassword = new KPasswordEdit( gboxPassword, "txtUser" );
  layPassword->addWidget( txtPassword );

  //set password defaults
  grpPasswordStorage->setButton( DEFAULT_ACCOUNT_PASSWORD_STORAGE );
  slotPasswordStorageChanged( DEFAULT_ACCOUNT_PASSWORD_STORAGE );

  //active check box
  QGridLayout* layActive = new QGridLayout( layGeneral, 1, 1 );
  layActive->setAlignment( Qt::AlignCenter );
  chkActive = new QCheckBox( i18n( "Active"), pgGeneral, "chkActive" );
  QToolTip::add( chkActive, i18n( "Select it to activate this account." ) );
  layActive->addWidget( chkActive, 0, 0 );
  chkActive->setChecked( DEFAULT_ACCOUNT_ACTIVE );

  //secure transfer groupbox and layouts
  QGroupBox* gboxSecureTransfer = new QGroupBox( 0, Qt::Horizontal, i18n( "Encryption" ), pgSecurity, "gboxSecureTransfer" );
  gboxSecureTransfer->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
  laySecurity->addWidget( gboxSecureTransfer );

  QHBoxLayout* laySecureTransfer = new QHBoxLayout( gboxSecureTransfer->layout(), spacingHint() );

  //radio buttons to set secure transfer
  grpSecureTransfer = new QButtonGroup( NULL, "grpSecureTransfer" );
  connect( grpSecureTransfer, SIGNAL( clicked( int ) ), this, SLOT( slotSecureTransferChanged ( int ) ) );

  QRadioButton* btnSecureTransferNone = new QRadioButton( i18n( "None" ), gboxSecureTransfer, "btnSecureTransferNone" );
  QRadioButton* btnSecureTransferSSL = new QRadioButton( i18n( "SSL"), gboxSecureTransfer, "btnSecureTransferSSL" );
  QRadioButton* btnSecureTransferTLS = new QRadioButton( i18n( "TLS" ), gboxSecureTransfer, "btnSecureTransferTLS" );
  grpSecureTransfer->insert( btnSecureTransferNone, ID_BUTTON_SECTRANSFER_NONE );
  grpSecureTransfer->insert( btnSecureTransferSSL, ID_BUTTON_SECTRANSFER_SSL );
  grpSecureTransfer->insert( btnSecureTransferTLS, ID_BUTTON_SECTRANSFER_TLS );
  QToolTip::add( btnSecureTransferNone, i18n( "The download of the mail header and body will not be encrypted. Use this, if your provider doesn't make a secure transfer available." ) );
  QToolTip::add( btnSecureTransferSSL, i18n( "Secure Sockets Layer (SSL), is a cryptographic protocol that provides secure communications on the Internet." ) );
  QToolTip::add( btnSecureTransferTLS, i18n( "Transport Layer Security (TLS) is a cryptographic protocol that provides secure communications on the Internet. It is the successor of SSL." ) );
  laySecureTransfer->addWidget( btnSecureTransferNone );
  laySecureTransfer->addWidget( btnSecureTransferSSL );
  laySecureTransfer->addWidget( btnSecureTransferTLS );

  grpSecureTransfer->setButton( DEFAULT_ACCOUNT_SECTRANSFER );

  //set pages to tab widget
  tabs->addTab( pgGeneral, i18n( "General" ) );
  tabs->addTab( pgSecurity, i18n( "Security" ) );

  //set caption
  if( item == NULL )
    setCaption( i18n( "New account" ) );
  else
    setCaption( i18n( "Edit account" ) );

  //write values of the given account into the dialog items
  if( account != NULL )
    fillDialog();


}


AccountSetupDialog::~AccountSetupDialog()
{
}

void AccountSetupDialog::slotPasswordStorageChanged( int id )
{
  if( id == ID_BUTTON_PASSWORD_DONT_SAVE )
  {
    txtPassword->setEnabled( false );
    txtPassword->clear();
  }
  else
    txtPassword->setEnabled( true );
}

void AccountSetupDialog::slotOk( )
{
  //check for necessary values
  if( txtAccount->text() == "" )
  {
    KMessageBox::error( this, i18n( "Please enter an account name." ) );
    return;
  }
  if( txtServer->text() == "" )
  {
    KMessageBox::error( this, i18n( "Please enter an server." ) );
    return;
  }

  if( txtUser->text() == "" )
  {
    KMessageBox::error( this, i18n( "Please enter an user name." ) );
    return;
  }

  //test for unique account name
  //we will only test if the account is new or if the
  //name in the dialog is different from the name of the account object
  bool test = false;
  if( account == NULL )
    test = true;
  else if( account != NULL && account->getAccountName() != txtAccount->text() )
    test = true;
  else
    test = false;

  if( test )
  {
    //OK, we want to test
    AccountSetupItem* item = NULL;
    int index = 0;
    bool equality = false;

    do
    {
      item = (AccountSetupItem*)( ListView->itemAtIndex( index ) );
      if( item != NULL )
      {
        index++;
        equality = item->getAccountName() == txtAccount->text();
      }
    } while( item != NULL && !equality );

    //exit method if we have found an account with the same name
    if( equality )
    {
      KMessageBox::error( this, i18n( "There is already an account named %1. Please choose another name." ).arg( txtAccount->text() ) );
      return;
    }

  }


  //create a new account item if necessary
  if( account == NULL )
    account = new AccountSetupItem( ListView );

  //show a warning if the account name was changend
  if( account->getAccountName() != DEFAULT_ACCOUNT_NAME && account->getAccountName() != txtAccount->text() )
    KMessageBox::information( this, i18n( "You have changed the account name. The account will lose all downloaded mail headers. Please perform a refresh." ) );

  //set column text
  account->setText( 0, txtAccount->text() );

  //write values into account item
  account->setAccountName( txtAccount->text() );
  account->setServer( txtServer->text() );
  account->setProtocol( cboProtocol->currentText() );
  account->setPort( spbPort->value() );
  account->setUser( txtUser->text() );

  //get the password
  //the class KPasswordEdit doesn't have a method to set the password
  //therefore we use setText(). But if we use this method, KPasswordEdit::password()
  //will return an empty string. If the user has typed in a new password, KPasswordEdit::password()
  //will return the correct password
  QString pass;
  if( txtPassword->password() == "" || txtPassword->password() == QString::null )
    pass = txtPassword->text();
  else
    pass = txtPassword->password();

  switch( grpPasswordStorage->selectedId() )
  {
    case ID_BUTTON_PASSWORD_DONT_SAVE    : account->setPasswordStorageType( CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE );
                                           account->setPassword( QString::null );
                                           break;
    case ID_BUTTON_PASSWORD_SAVE_FILE    : account->setPasswordStorageType( CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE );
                                           account->setPassword( pass );
                                           break;
    case ID_BUTTON_PASSWORD_SAVE_KWALLET : account->setPasswordStorageType( CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET );
                                           account->setPassword( pass );
                                           break;
    default                              : account->setPasswordStorageType( -1 );
                                           account->setPassword( QString::null );
  }

  account->setActive( chkActive->isChecked() );

  switch( grpSecureTransfer->selectedId() )
  {
    case ID_BUTTON_SECTRANSFER_NONE : account->setTransferSecurity( CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE ); break;
    case ID_BUTTON_SECTRANSFER_SSL  : account->setTransferSecurity( CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL ); break;
    case ID_BUTTON_SECTRANSFER_TLS  : account->setTransferSecurity( CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS ); break;
    default                         : account->setTransferSecurity( DEFAULT_ACCOUNT_SECTRANSFER ); break;
  }

  //call slot of super class to close the dialog
  KDialogBase::slotOk();
}

void AccountSetupDialog::fillDialog( )
{
  //check for valid account pointer
  if( account == NULL )
  {
    kdError() << "AccountSetupDialog::fillDialog: invalid pointer to account item." << endl;
    return;
  }

  txtAccount->setText( account->getAccountName() );
  txtServer->setText( account->getServer() );
  cboProtocol->setCurrentText( account->getProtocol().upper() );

  if( account->getPort() >= 0 && account->getPort() <= 65535 )
    spbPort->setValue( account->getPort() );
  else
    spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 );

  txtUser->setText( account->getUser() );

  int type = account->getPasswordStorageType();
  if( type != CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE && type != CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE && type != CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    type = DEFAULT_ACCOUNT_PASSWORD_STORAGE;

  switch( type )
  {
    case CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE    : grpPasswordStorage->setButton( ID_BUTTON_PASSWORD_DONT_SAVE );
                                                      txtPassword->setEnabled( false );
                                                      txtPassword->clear();
                                                      break;
    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE    : grpPasswordStorage->setButton( ID_BUTTON_PASSWORD_SAVE_FILE );
                                                      txtPassword->setEnabled( true );
                                                      txtPassword->setText( account->getPassword() );
                                                      break;
    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET : grpPasswordStorage->setButton( ID_BUTTON_PASSWORD_SAVE_KWALLET );
                                                      txtPassword->setEnabled( true );
                                                      txtPassword->setText( account->getPassword() );
                                                      break;
    default                                         : grpPasswordStorage->setButton( 1 );
                                                      txtPassword->clear();
  }

  chkActive->setChecked( account->getActive() );

  int transferSecurity = account->getTransferSecurity();
  if( transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE && transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL && transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS )
    transferSecurity = DEFAULT_ACCOUNT_SECTRANSFER;

  switch( transferSecurity )
  {
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE : grpSecureTransfer->setButton( ID_BUTTON_SECTRANSFER_NONE ); break;
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL  : grpSecureTransfer->setButton( ID_BUTTON_SECTRANSFER_SSL ); break;
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS  : grpSecureTransfer->setButton( ID_BUTTON_SECTRANSFER_TLS ); break;
    default                                    : grpSecureTransfer->setButton( ID_BUTTON_SECTRANSFER_NONE ); break;
  }
}

void AccountSetupDialog::slotSecureTransferChanged( int id )
{
  switch( id )
  {
    case ID_BUTTON_SECTRANSFER_NONE   : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 ); break;
    case ID_BUTTON_SECTRANSFER_SSL    : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3SSL ); break;
    case ID_BUTTON_SECTRANSFER_TLS    : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 ); break;
  }
}



#include "accountsetupdialog.moc"
