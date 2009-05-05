//
// C++ Interface: encryption
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

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
