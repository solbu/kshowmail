//
// C++ Implementation: kwalletaccess
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
  if( name == "" || name == QString::null )
  {
    KMessageBox::error( NULL, i18n( "Could not get wallet name for network datas from KWallet." ) );
    return false;
  }

  //open wallet
  //we want to create a connection object just at first call of this function
  static KWallet::Wallet* wallet;

  if( wallet == NULL )
  {
    wallet = KWallet::Wallet::openWallet( name );
  }
  else if( !wallet->isOpen() )
  {
    delete wallet;
    wallet = KWallet::Wallet::openWallet( name );
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
    return QString::null;
  }

  //get wallet name for network data
  QString name = KWallet::Wallet::NetworkWallet();
  if( name == "" || name == QString::null )
  {
    KMessageBox::error( NULL, i18n( "Could not get wallet name for network datas from KWallet." ) );
    return QString::null;
  }

  //open wallet
  //we want to create a connection object just at first call of this function
  static KWallet::Wallet* wallet;

  if( wallet == NULL )
  {
    wallet = KWallet::Wallet::openWallet( name );
  }
  else if( !wallet->isOpen() )
  {
    delete wallet;
    wallet = KWallet::Wallet::openWallet( name );
  }

  if( wallet == NULL )
  {
    KMessageBox::error( NULL, i18n( "Could not open KWallet." ) );
    return QString::null;
  }

  //set folder
  bool setFolderSuccess = wallet->setFolder( "KShowmail" );
  if( !setFolderSuccess )
  {
    KMessageBox::error( NULL, i18n( "Could not open folder for KShowmail in KWallet." ) );
    return QString::null;
  }

  //read password
  QString password;

  int readPasswordSuccess = wallet->readPassword( account, password );
  if( readPasswordSuccess != 0 )
  {
    KMessageBox::error( NULL, i18n( "Could not get password of account %1 from KWallet." ).arg( account) );
    return QString::null;
  }

  return password;
}
