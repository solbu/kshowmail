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

#include "accountsetupdialogcontext.h"

AccountSetupDialogContext::AccountSetupDialogContext( QWidget* parent, QString accountName )
  : KDialog( parent )
{

  //save the account name
  accName = accountName;
  
   //tab widget
   KTabWidget* tabs = new KTabWidget( this );
 
   //pages
   QWidget* pgGeneral = new QWidget( this );
   QWidget* pgSecurity = new QWidget( this );
   setMainWidget( tabs );
 
   //layouts of general page
   QVBoxLayout* layGeneral = new QVBoxLayout( pgGeneral );
   QGridLayout* layTop = new QGridLayout();
   layGeneral->addLayout( layTop );
 
   //layouts of security page
   QVBoxLayout* laySecurity = new QVBoxLayout( pgSecurity );
   laySecurity->setAlignment( Qt::AlignTop );
 
   //upper items
   QLabel* lblAccount = new QLabel( i18nc( "@label:textbox account name", "Account:" ), pgGeneral );
   txtAccount = new KLineEdit( pgGeneral );
   txtAccount->setEnabled( false );
   txtAccount->setFocus();
   lblAccount->setToolTip( i18nc( "@info:tooltip", "Unique Account Name" ) );
   txtAccount->setToolTip( i18nc( "@info:tooltip", "Unique Account Name" ) );
   layTop->addWidget( lblAccount, 0, 0 );
   layTop->addWidget( txtAccount, 0, 1 );
 
   QLabel* lblServer = new QLabel( i18nc( "@label:textbox server host name", "Server:" ), pgGeneral );
   txtServer = new KLineEdit( pgGeneral );
   lblServer->setToolTip( i18nc( "@info:tooltip", "Server Name" ) );
   txtServer->setToolTip( i18nc( "@info:tooltip", "Server Name" ) );
   layTop->addWidget( lblServer, 1, 0 );
   layTop->addWidget( txtServer, 1, 1 );
 
   QLabel* lblProtocol = new QLabel( i18nc( "@label:listbox mail fetching protocol", "Protocol:" ), pgGeneral );
   cboProtocol = new KComboBox( pgGeneral );
   cboProtocol->insertItem( 0, "POP3" );  //currently KShowmail just supports POP3
   lblProtocol->setToolTip( i18nc( "@info:tooltip", "Protocol, which shall be used to get the mails from the server. Currently KShowmail just supports POP3.") );
   cboProtocol->setToolTip( i18nc( "@info:tooltip", "Protocol, which shall be used to get the mails from the server. Currently KShowmail just supports POP3.") );
   layTop->addWidget( lblProtocol, 2, 0 );
   layTop->addWidget( cboProtocol, 2, 1 );
 
   QLabel* lblPort = new QLabel( i18nc( "@label:spinbox TCP port, on which the server is listening", "Port:" ), pgGeneral );
   spbPort = new QSpinBox( pgGeneral );
   spbPort->setRange( 0, 65535 );
   spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 );
   lblPort->setToolTip( i18nc( "@info:tooltip", "Port Number. Normally POP3 uses port 110." ) );
   spbPort->setToolTip( i18nc( "@info:tooltip", "Port Number. Normally POP3 uses port 110." ) );
   layTop->addWidget( lblPort, 3, 0 );
   layTop->addWidget( spbPort, 3, 1 );
 
   QLabel* lblUser = new QLabel( i18nc( "@label:textbox user name to authenticate", "User:" ), pgGeneral );
   txtUser = new KLineEdit( pgGeneral );
   lblUser->setToolTip( i18nc( "@info:tooltip", "To authenticate to the mail server you need an user name." ) );
   txtUser->setToolTip( i18nc( "@info:tooltip", "To authenticate to the mail server you need an user name." ) );
   layTop->addWidget( lblUser, 4, 0 );
   layTop->addWidget( txtUser, 4, 1 );
 
   //password groupbox and layouts
   QGroupBox* gboxPassword = new QGroupBox( i18nc( "@title:group contains password storage options and passwort input textbox", "Password" ), pgGeneral );
   layGeneral->addWidget( gboxPassword );
 
   QVBoxLayout* layPassword = new QVBoxLayout( );
   gboxPassword->setLayout( layPassword ); 
   QGridLayout* layPasswordStorage = new QGridLayout();
   layPassword->addLayout( layPasswordStorage );
   
 
   //radio buttons to set storage of the password
   grpPasswordStorage = new QButtonGroup( NULL );
   connect( grpPasswordStorage, SIGNAL( buttonClicked( int ) ), this, SLOT( slotPasswordStorageChanged( int ) ) );
 
   QRadioButton* btnPasswordDontSave = new QRadioButton( i18nc( "@option:radio don't save the password", "Don't save" ), gboxPassword );
   QRadioButton* btnPasswordSaveFile = new QRadioButton( i18nc( "@option:radio save passwort in config file", "Save password "), gboxPassword );
   QRadioButton* btnPasswordSaveKWallet = new QRadioButton( i18nc( "@option:radio use KWallet to save the password", "Use KWallet" ), gboxPassword );
   grpPasswordStorage->addButton( btnPasswordDontSave, ID_BUTTON_PASSWORD_DONT_SAVE );
   grpPasswordStorage->addButton( btnPasswordSaveFile, ID_BUTTON_PASSWORD_SAVE_FILE );
   grpPasswordStorage->addButton( btnPasswordSaveKWallet, ID_BUTTON_PASSWORD_SAVE_KWALLET );
   btnPasswordDontSave->setToolTip( i18nc( "@info:tooltip", "Don't save password. KShowmail will ask you for it at first server connect." ) );
   btnPasswordSaveFile->setToolTip( i18nc( "@info:tooltip", "Save password in the configuration file. Not recommended, because the password is just lightly encrypted" ) );
   btnPasswordSaveKWallet->setToolTip( i18nc( "@info:tooltip", "Use KWallet to save the password. Maybe you have to type in the KWallet master password at first server connect." ) );
   layPasswordStorage->addWidget( btnPasswordDontSave, 0, 0 );
   layPasswordStorage->addWidget( btnPasswordSaveFile, 0, 1 );
   layPasswordStorage->addWidget( btnPasswordSaveKWallet, 1, 0 );


   //password edit line
   txtPassword = new KLineEdit( gboxPassword );
 	 txtPassword->setPasswordMode( true );
   layPassword->addWidget( txtPassword );
 
   //set password defaults
   QRadioButton* btnToCheck = static_cast<QRadioButton*>( grpPasswordStorage->button( DEFAULT_ACCOUNT_PASSWORD_STORAGE ) );
   btnToCheck->setChecked( true );
   slotPasswordStorageChanged( DEFAULT_ACCOUNT_PASSWORD_STORAGE );
 
   //active check box
   QGridLayout* layActive = new QGridLayout();
   layGeneral->addLayout( layActive );
   layActive->setAlignment( Qt::AlignCenter );
   chkActive = new QCheckBox( i18nc( "@option:check to activate this account", "Active"), pgGeneral );
   chkActive->setToolTip( i18nc( "@info:tooltip", "Select it to activate this account." ) );
   layActive->addWidget( chkActive, 0, 0 );
   chkActive->setChecked( DEFAULT_ACCOUNT_ACTIVE );
 
   //secure transfer groupbox and layouts
   QGroupBox* gboxSecureTransfer = new QGroupBox( i18nc( "@title:group transfer encryption", "Encryption" ), pgSecurity );
   gboxSecureTransfer->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
   laySecurity->addWidget( gboxSecureTransfer );
 
   QHBoxLayout* laySecureTransfer = new QHBoxLayout();
   gboxSecureTransfer->setLayout( laySecureTransfer );
 
   //radio buttons to set secure transfer
   grpSecureTransfer = new QButtonGroup( NULL );
   connect( grpSecureTransfer, SIGNAL( buttonClicked( int ) ), this, SLOT( slotSecureTransferChanged ( int ) ) );
 
   QRadioButton* btnSecureTransferNone = new QRadioButton( i18nc( "@option:radio encryption", "None" ), gboxSecureTransfer );
   QRadioButton* btnSecureTransferSSL = new QRadioButton( i18nc( "@option:radio encryption", "SSL"), gboxSecureTransfer );
   QRadioButton* btnSecureTransferTLS = new QRadioButton( i18nc( "@option:radio encryption", "TLS" ), gboxSecureTransfer );
   grpSecureTransfer->addButton( btnSecureTransferNone, ID_BUTTON_SECTRANSFER_NONE );
   grpSecureTransfer->addButton( btnSecureTransferSSL, ID_BUTTON_SECTRANSFER_SSL );
   grpSecureTransfer->addButton( btnSecureTransferTLS, ID_BUTTON_SECTRANSFER_TLS );
   btnSecureTransferNone->setToolTip( i18nc( "@info:tooltip", "The download of the mail header and body will not be encrypted. Use this, if your provider doesn't make a secure transfer available." ) );
   btnSecureTransferSSL->setToolTip( i18nc( "@info:tooltip", "Secure Sockets Layer (SSL), is a cryptographic protocol that provides secure communications on the Internet." ) );
   btnSecureTransferTLS->setToolTip( i18nc( "@info:tooltip", "Transport Layer Security (TLS) is a cryptographic protocol that provides secure communications on the Internet. It is the successor of SSL." ) );
   laySecureTransfer->addWidget( btnSecureTransferNone );
   laySecureTransfer->addWidget( btnSecureTransferSSL );
   laySecureTransfer->addWidget( btnSecureTransferTLS );
 
   btnToCheck = static_cast<QRadioButton*>( grpSecureTransfer->button( DEFAULT_ACCOUNT_SECTRANSFER ) );
   btnToCheck->setChecked( true );

  //checkbox to allow unsecure transfer
  chkAllowUnsecureLogin = new QCheckBox( i18nc( "@option:check whether login without security shall be allowed", "Allow unsafe login"), pgSecurity );
  chkAllowUnsecureLogin->setToolTip( i18nc( "@info:tooltip", "Select it to allow an unsafe login if necessary.\nThe password will be transmit unencrypted. Maybe someone can read it!" ) );
  laySecurity->addWidget( chkAllowUnsecureLogin );
  chkAllowUnsecureLogin->setChecked( DEFAULT_ACCOUNT_ALLOW_UNSECURE_LOGIN );

 
   //set pages to tab widget
   tabs->addTab( pgGeneral, i18nc( "@title:tab general account options", "General" ) );
   tabs->addTab( pgSecurity, i18nc( "@title:tab account security options", "Security" ) );
 
   //set caption
   setCaption( i18nc( "@title:window title of account setup dialog", "Edit Account <resource>%1</resource>", accName ) );

   //load the setup from the application config
   load();
}


AccountSetupDialogContext::~AccountSetupDialogContext()
{
}

void AccountSetupDialogContext::slotPasswordStorageChanged( int id )
{
  if( id == ID_BUTTON_PASSWORD_DONT_SAVE )
  {
    txtPassword->setEnabled( false );
    txtPassword->clear();
  }
  else
    txtPassword->setEnabled( true );
}

void AccountSetupDialogContext::slotButtonClicked( int button )
{
  //call the original slot if the clicked button is not Ok
  if( button != KDialog::Ok )
  {
    KDialog::slotButtonClicked( button );
    return;
  }

  //check for necessary values
  if( txtServer->text().isEmpty() )
  {
    KMessageBox::error( this, i18nc( "@info error message: no server was entered", "Please enter an server." ) );
    return;
  }

  if( txtUser->text().isEmpty() )
  {
    KMessageBox::error( this, i18nc( "@info error message: no user name was entered", "Please enter an user name." ) );
    return;
  }

  save();

  
  //call slot of super class to close the dialog
  KDialog::slotButtonClicked( KDialog::Ok );
}

void AccountSetupDialogContext::load()
{
  //get application config
  KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), accName );

  //fill dialog
  txtAccount->setText( accName );

  txtServer->setText( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_SERVER, DEFAULT_ACCOUNT_SERVER ) );

  QString strProto = configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, DEFAULT_ACCOUNT_PROTOCOL ).toUpper();
  if( strProto == CONFIG_VALUE_ACCOUNT_PROTOCOL_POP3 ) {
    cboProtocol->setCurrentIndex( 0 );
  }

  spbPort->setValue( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_PORT, DEFAULT_ACCOUNT_PORT_POP3 ) );
  
  txtUser->setText( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_USER, DEFAULT_ACCOUNT_USER ) );

  int type = configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, DEFAULT_ACCOUNT_PASSWORD_STORAGE );
  if( type != CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE && type != CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE && type != CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    type = DEFAULT_ACCOUNT_PASSWORD_STORAGE;

  QRadioButton* btnToCheck;
  switch( type )
  {
    case CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE    : btnToCheck = static_cast<QRadioButton*>( grpPasswordStorage->button( ID_BUTTON_PASSWORD_DONT_SAVE ) );
                                                      txtPassword->setEnabled( false );
                                                      txtPassword->clear();
                                                      break;
    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE    : btnToCheck = static_cast<QRadioButton*>( grpPasswordStorage->button( ID_BUTTON_PASSWORD_SAVE_FILE ) );
                                                      txtPassword->setEnabled( true );
                                                      txtPassword->setText( decrypt( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, DEFAULT_ACCOUNT_PASSWORD ) ) );
                                                      break;
    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET : btnToCheck = static_cast<QRadioButton*>( grpPasswordStorage->button( ID_BUTTON_PASSWORD_SAVE_KWALLET ) );
                                                      txtPassword->setEnabled( true );
                                                      txtPassword->setText( KWalletAccess::getPassword( accName ) );
                                                      break;
    default                                         : btnToCheck = static_cast<QRadioButton*>( grpPasswordStorage->button( 1 ) );
                                                      txtPassword->clear();
  }
  btnToCheck->setChecked( true );

  chkActive->setChecked( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, DEFAULT_ACCOUNT_ACTIVE ) );

  int transferSecurity = configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, DEFAULT_ACCOUNT_SECTRANSFER );
  if( transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE && transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL && transferSecurity != CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS )
    transferSecurity = DEFAULT_ACCOUNT_SECTRANSFER;

  switch( transferSecurity )
  {
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE : btnToCheck = static_cast<QRadioButton*>( grpSecureTransfer->button( ID_BUTTON_SECTRANSFER_NONE ) ); break;
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL  : btnToCheck = static_cast<QRadioButton*>( grpSecureTransfer->button( ID_BUTTON_SECTRANSFER_SSL ) ); break;
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS  : btnToCheck = static_cast<QRadioButton*>( grpSecureTransfer->button( ID_BUTTON_SECTRANSFER_TLS ) ); break;
    default                                    : btnToCheck = static_cast<QRadioButton*>( grpSecureTransfer->button( ID_BUTTON_SECTRANSFER_NONE ) ); break;
  }
  btnToCheck->setChecked( true );

  chkAllowUnsecureLogin->setChecked( configAcc->readEntry( CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN, DEFAULT_ACCOUNT_ALLOW_UNSECURE_LOGIN ) );

	//enable or disable the unsecure login checkbox  
  enableLoginCheckbox();

}

void AccountSetupDialogContext::slotSecureTransferChanged( int id )
{
  switch( id )
  {
    case ID_BUTTON_SECTRANSFER_NONE   : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 ); break;
    case ID_BUTTON_SECTRANSFER_SSL    : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3SSL ); break;
    case ID_BUTTON_SECTRANSFER_TLS    : spbPort->setValue( DEFAULT_ACCOUNT_PORT_POP3 ); break;
  }
  
	//enable or disable the unsecure login checkbox  
  enableLoginCheckbox();

}

void AccountSetupDialogContext::save()
{
  //get application config
  KConfigGroup* configAcc = new KConfigGroup( KGlobal::config(), accName );

  //server
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_SERVER, txtServer->text() );

  //protocol
  if( cboProtocol->currentIndex() == 0 ) {

    configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, CONFIG_VALUE_ACCOUNT_PROTOCOL_POP3 );
  }

  //port
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PORT, spbPort->value() );

  //user
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_USER, txtUser->text() );

  //password
  QString pass = txtPassword->text();

  KUrl url; //this is to crypt the password if desired
  url.setUser( txtUser->text() );
  url.setHost( txtServer->text() );
  url.setPass( pass );


  switch( grpPasswordStorage->checkedId() )
  {
    case ID_BUTTON_PASSWORD_DONT_SAVE    : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE );
                                           configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, "" );
                                           break;
    case ID_BUTTON_PASSWORD_SAVE_FILE    : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE );
                                           configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, crypt( url ) );
                                           break;
    case ID_BUTTON_PASSWORD_SAVE_KWALLET : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET );
                                           KWalletAccess::savePassword( accName, pass );
                                           break;
    default                              : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE );
                                           configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, "" );
  }

  //active
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, chkActive->isChecked() );

  //transfer security
  switch( grpSecureTransfer->checkedId() ) {

    case ID_BUTTON_SECTRANSFER_NONE : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE ); break;
    case ID_BUTTON_SECTRANSFER_SSL  : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL ); break;
    case ID_BUTTON_SECTRANSFER_TLS  : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS ); break;
    default                         : configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, DEFAULT_ACCOUNT_SECTRANSFER );
  }

  //unsecure login
  configAcc->writeEntry( CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN, chkAllowUnsecureLogin->isChecked() );

  //write to disk
  configAcc->sync();
}


void AccountSetupDialogContext::enableLoginCheckbox()
{
	//get the current transfer security
	int transSec = grpSecureTransfer->checkedId();
	
	if( transSec == ID_BUTTON_SECTRANSFER_NONE ) {
	
		chkAllowUnsecureLogin->setEnabled( true );
	
	} else {
	
		chkAllowUnsecureLogin->setEnabled( false );
	}

}


#include "accountsetupdialogcontext.moc"
