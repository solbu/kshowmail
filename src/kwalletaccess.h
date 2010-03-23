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
