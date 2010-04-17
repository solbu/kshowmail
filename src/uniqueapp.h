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

#ifndef UNIQUEAPP_H
#define UNIQUEAPP_H

//KDE headers
#include <kuniqueapplication.h>
#include <kcmdlineargs.h>

//KShowmail headers
#include "kshowmail.h"
#include "constants.h"

/**
 * @short Creates a unique instance of kshowmail
 * @author Eggert Ehmke
 * @author Ulrich Weigelt
 */
class UniqueApp : public KUniqueApplication
{
  public:
    
  /**
   * Constructor
   */
  UniqueApp();

  /**
   * Destructor
   */
  ~UniqueApp();

  /**
   * overloaded method of KUniqueApplication. Creates a new instance of kshowmail.
   */
  virtual int newInstance();


private:

  /**
   * the kshowmail instance
   */
  KShowmail* m_pApp;
};

#endif // UNIQUEAPP_H
