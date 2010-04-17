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

#include "./uniqueapp.h"

UniqueApp::UniqueApp()
{
  m_pApp = NULL;
}

UniqueApp::~UniqueApp()
{}

int UniqueApp::newInstance ()
{
  //start our application
  //---------------------

  //at the first invoke of newInstance() create a new instance of kshowmail
  if( m_pApp == NULL )
  {
    m_pApp = new KShowmail();
  }

  //get the config
  KConfigGroup* configGeneral = new KConfigGroup( KGlobal::config(), CONFIG_GROUP_GENERAL );

  //hide or show the window in order of the config
  if( configGeneral->readEntry( CONFIG_ENTRY_START_MINIMIZED, DEFAULT_START_MINIMIZED ) ) {

      m_pApp->hide();

  } else {

    m_pApp->show();
  }
  
  return 0;
}
