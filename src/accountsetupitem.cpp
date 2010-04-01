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

#include "accountsetupitem.h"

AccountSetupItem::AccountSetupItem( QTreeWidget* parent )
 : QTreeWidgetItem( parent )
{
  init();

}

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
  accountConfig->writeEntry( CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN, getUnsecureLoginAllowed() );

}

void AccountSetupItem::load( )
{
  KConfigGroup* accountConfig = new KConfigGroup( config, getAccountName() );

  _server = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_SERVER, DEFAULT_ACCOUNT_SERVER );
  _protocol = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PROTOCOL, DEFAULT_ACCOUNT_PROTOCOL );
  _port = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PORT, DEFAULT_ACCOUNT_PORT_POP3 );
  _user = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_USER, DEFAULT_ACCOUNT_USER );
  _passwordStorage = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE, DEFAULT_ACCOUNT_PASSWORD_STORAGE );

  if( _passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE )
    _password = decrypt( accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_PASSWORD, DEFAULT_ACCOUNT_PASSWORD ) );
  else if( _passwordStorage == CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET )
    _password = KWalletAccess::getPassword( getAccountName() );
  else
    _password.clear();

  _active = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_ACTIVE, DEFAULT_ACCOUNT_ACTIVE );
  _transferSecurity = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_SECTRANSFER, DEFAULT_ACCOUNT_SECTRANSFER );
  _allowUnsecureLogin = accountConfig->readEntry( CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN, DEFAULT_ACCOUNT_ALLOW_UNSECURE_LOGIN );
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

void AccountSetupItem::print()
{
  kDebug() << "Account Name: " << getAccountName() << endl;
  kDebug() << "Server: " << getServer() << endl;
  kDebug() << "Port: " << getPort() << endl;
  kDebug() << "Protocol: " << getProtocol() << endl;
  kDebug() << "User: " << getUser() << endl;
  kDebug() << "Password: " << getPassword() << endl;

  switch( getPasswordStorageType() )
  {
    case CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE :
      kDebug() << "Password Storage: Don't Save" << endl; break;

    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE :
      kDebug() << "Password Storage: File" << endl; break;

    case CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET :
      kDebug() << "Password Storage: KWallet" << endl; break;

    default :
      kDebug() << "unknown" << endl; break;
  }

  kDebug() << "Active: " << getActive() << endl;

  switch( getTransferSecurity() )
  {
    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE :
      kDebug() << "Transfer Security: None" << endl; break;

    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL :
      kDebug() << "Transfer Security: SSL" << endl; break;

    case CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS :
      kDebug() << "Transfer Security: TLS" << endl; break;

    default :
      kDebug() << "Transfer Security: unknwon" << endl; break;

  }
}

void AccountSetupItem::setUnsecureLoginAllowed(bool allowed)
{
  _allowUnsecureLogin = allowed;
}

bool AccountSetupItem::getUnsecureLoginAllowed() const
{
  return _allowUnsecureLogin;
}



