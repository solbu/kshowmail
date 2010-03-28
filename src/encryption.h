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

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

//Qt headers
#include <qstring.h>

//KDE headers
#include <KUrl>

/**
 * @brief Provides functions to crypt and decrypt a password.
 */
namespace Encryption
{
  /**
   * Transforms the given hex numeric character to integer.
   * For example: a given '2' will transformed to 2; 'B' to 12.
   * @param c character to transform [0..9, 'A'..'F']; no lower case letters
   * @return integer value of the given character
   */
  int hexbyt( const char c );

  /**
   * Encrypts the the password in the given url and returns it.
   * The encryption algorithm uses host and user name to crypt the password.
   * @param url the url with password, host and user name
   * @return encrypted password
   */
  const QString crypt( const KUrl& url );

  /**
   * Decrypts the given encrypted password.
   * @param pass encrypted password
   * @return decrypted password
     */
  const QString decrypt( const QString& pass );
}

#endif
