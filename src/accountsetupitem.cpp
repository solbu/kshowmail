//
// C++ Implementation: accountsetupitem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "accountsetupitem.h"

AccountSetupItem::AccountSetupItem( QTreeWidget* parent, const QString & name )
  : QTreeWidgetItem( parent )
{
  init();

  //save account name
  _account = name;

  //set column text
  setText( 0, name );

}

AccountSetupItem::~AccountSetupItem()
{
}

void AccountSetupItem::init( )
{
  //set default values
  _account          = DEFAULT_ACCOUNT_NAME;
  _server           = DEFAULT_ACCOUNT_SERVER;
  _protocol         = DEFAULT_ACCOUNT_PROTOCOL;
  _port             = DEFAULT_ACCOUNT_PORT_POP3;
  _user             = DEFAULT_ACCOUNT_USER;
  _password         = DEFAULT_ACCOUNT_PASSWORD;
  _passwordStorage  = DEFAULT_ACCOUNT_PASSWORD_STORAGE;
  _active           = DEFAULT_ACCOUNT_ACTIVE;
  _transferSecurity = DEFAULT_ACCOUNT_SECTRANSFER;

  //get application config object (kshowmailrc)
  config = KGlobal::config();

  //set Icon
  KIcon picIcon = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/account.svgz" ) );
  setIcon( 0, picIcon );
}

void AccountSetupItem::setAccountName( const QString & name )
{
  _account = name;
}

QString AccountSetupItem::getAccountName( ) const
{
  return _account;
}

void AccountSetupItem::setServer( const QString & server )
{
  _server = server;
}

QString AccountSetupItem::getServer( ) const
{
  return _server;
}

void AccountSetupItem::setProtocol( const QString & protocol )
{
  _protocol = protocol;
}

QString AccountSetupItem::getProtocol( ) const
{
  return _protocol;
}

void AccountSetupItem::setPort( int port )
{
  if( port >= 0 && port <= 65535 )
    _port = port;
  else
    _port = DEFAULT_ACCOUNT_PORT_POP3;
}

int AccountSetupItem::getPort( ) const
{
  return _port;
}

void AccountSetupItem::setUser( const QString & user )
{
  _user = user;
}

QString AccountSetupItem::getUser( ) const
{
  return _user;
}

void AccountSetupItem::setPassword( const QString & password )
{
  _password = password;
}

QString AccountSetupItem::getPassword( ) const
{
  return _password;
}

void AccountSetupItem::setPasswordStorageType( int type )
{
  if( type == CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE || type == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE || type == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    _passwordStorage = type;
  else
    _passwordStorage = DEFAULT_ACCOUNT_PASSWORD_STORAGE;
}

int AccountSetupItem::getPasswordStorageType( ) const
{
  return _passwordStorage;
}

void AccountSetupItem::setActive( bool active )
{
  _active = active;
}

bool AccountSetupItem::getActive( ) const
{
  return _active;
}

void AccountSetupItem::save() const
{
  KConfigGroup* accountConfig = new KConfigGroup( config, getAccountName() );

  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_NAME, getAccountName() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_SERVER, getServer() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, getProtocol().toUpper() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PORT, getPort() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_USER, getUser() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, getPasswordStorageType() );

  //save crypted password
  KUrl url;
  url.setUser( getUser() );
  url.setHost( getServer() );
  url.setPass( getPassword() );

  if( getPasswordStorageType() == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
    accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, crypt( url ) );
  else
    accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, QVariant() );

  //save password in KWallet if desired
  if( getPasswordStorageType() == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    KWalletAccess::savePassword( getAccountName(), getPassword() );

  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, getActive() );
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, getTransferSecurity() );

}

void AccountSetupItem::load( )
{
/*  config->setGroup( getAccountName() );

  _server = config->readEntry( CONFIG_ENTRY_ACCOUNT_SERVER, DEFAULT_ACCOUNT_SERVER );
  _protocol = config->readEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, DEFAULT_ACCOUNT_PROTOCOL );
  _port = config->readNumEntry( CONFIG_ENTRY_ACCOUNT_PORT, DEFAULT_ACCOUNT_PORT_POP3 );
  _user = config->readEntry( CONFIG_ENTRY_ACCOUNT_USER, DEFAULT_ACCOUNT_USER );
  _passwordStorage = config->readNumEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, DEFAULT_ACCOUNT_PASSWORD_STORAGE );

  if( _passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
    _password = decrypt( config->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, DEFAULT_ACCOUNT_PASSWORD ) );
  else if( _passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    _password = KWalletAccess::getPassword( getAccountName() );
  else
    _password = QString::null;

  _active = config->readBoolEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, DEFAULT_ACCOUNT_ACTIVE );
  _transferSecurity = config->readNumEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, DEFAULT_ACCOUNT_SECTRANSFER );*/
}

void AccountSetupItem::setTransferSecurity( int type )
{
  if( type == CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE || type == CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL || type == CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS )
    _transferSecurity = type;
  else
    _transferSecurity = DEFAULT_ACCOUNT_SECTRANSFER;
}

int AccountSetupItem::getTransferSecurity( ) const
{
  return _transferSecurity;
}




