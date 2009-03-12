/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

  m_pApp->show();

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  args->clear();

  return 0;
}