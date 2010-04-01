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

#include "kwalletaccess.h"


bool KWalletAccess::savePassword( const QString & account, const QString & password )
{
  //check for enabled KWallet
  if( !KWallet::Wallet::isEnabled() )
  {
    KMessageBox::error( NULL, i18n( "KWallet is not available." ) );
    return false;
  }

  //get wallet name for network data
  QString name = KWallet::Wallet::NetworkWallet();
  if( name.isEmpty() )
  {
    KMessageBox::error( NULL, i18n( "Could not get wallet name for network datas from KWallet." ) );
    return false;
  }

  //open wallet
  //we want to create a connection object just at first call of this function
  static KWallet::Wallet* wallet;

  if( wallet == NULL )
  {
    wallet = KWallet::Wallet::openWallet( name, 0 );
  }
  else if( !wallet->isOpen() )
  {
    delete wallet;
    wallet = KWallet::Wallet::openWallet( name, 0 );
  }

  if( wallet == NULL )
  {
    KMessageBox::error( NULL, i18n( "Could not open KWallet." ) );
    return false;
  }

  //create kshowmail folder if it does not exist
  if( !wallet->hasFolder( "KShowmail" ) )
  {
    bool createFolderSuccess = wallet->createFolder( "KShowmail" );

    if( !createFolderSuccess )
    {
      KMessageBox::error( NULL, i18n( "Could not create folder for KShowmail in KWallet." ) );
      return false;
    }
  }

  //set folder
  bool setFolderSuccess = wallet->setFolder( "KShowmail" );
  if( !setFolderSuccess )
  {
    KMessageBox::error( NULL, i18n( "Could not open folder for KShowmail in KWallet." ) );
    return false;
  }

  //write password
  int writePasswordSuccess = wallet->writePassword( account, password );
  if( writePasswordSuccess != 0 )
  {
    KMessageBox::error( NULL, i18n( "Could not save password in KWallet." ) );
    return false;
  }


  return true;
}

QString KWalletAccess::getPassword( const QString & account )
{
  //check for enabled KWallet
  if( !KWallet::Wallet::isEnabled() )
  {
    KMessageBox::error( NULL, i18n( "KWallet is not available." ) );
    return QString();
  }

  //get wallet name for network data
  QString name = KWallet::Wallet::NetworkWallet();
  if( name.isEmpty() )
  {
    KMessageBox::error( NULL, i18n( "Could not get wallet name for network datas from KWallet." ) );
    return QString();
  }

  //open wallet
  //we want to create a connection object just at first call of this function
  static KWallet::Wallet* wallet;

  if( wallet == NULL )
  {
    wallet = KWallet::Wallet::openWallet( name, 0 );
  }
  else if( !wallet->isOpen() )
  {
    delete wallet;
    wallet = KWallet::Wallet::openWallet( name, 0 );
  }

  if( wallet == NULL )
  {
    KMessageBox::error( NULL, i18n( "Could not open KWallet." ) );
    return QString();
  }

  //set folder
  bool setFolderSuccess = wallet->setFolder( "KShowmail" );
  if( !setFolderSuccess )
  {
    KMessageBox::error( NULL, i18n( "Could not open folder for KShowmail in KWallet." ) );
    return QString();
  }

  //read password
  QString password;

  int readPasswordSuccess = wallet->readPassword( account, password );
  if( readPasswordSuccess != 0 )
  {
    KMessageBox::error( NULL, i18nc( "@info error message", "Could not get password of account <resource>%1</resource> from KWallet.", account) );
    return QString();
  }

  return password;
}
