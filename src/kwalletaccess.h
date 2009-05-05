//
// C++ Interface: kwalletaccess
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KWALLETACCESS_H
#define KWALLETACCESS_H

//Qt headers
#include <qstring.h>

//KDE headers
#include <kwallet.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <klocale.h>

/**
 * @brief Provides routines to save and read passwords using KWallet.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
namespace KWalletAccess
{

  /**
   * Saves the given password into KWallet.
   * @param account account name
   * @param password the password
   * @return TRUE - password successfully saved
   * @return FALSE - password could not be saved
   */
  bool savePassword( const QString& account, const QString& password );

  /**
   * Returns the password of the given account.
   * @param account account name
   * @return the password or QString::null, if the sought password is not stored in KWallet.
   */
  QString getPassword( const QString& account );
}

#endif
