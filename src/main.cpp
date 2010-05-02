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

//KDE headers
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>
#include <kuniqueapplication.h>
#include <kdebug.h>

//KShowmail headers
#include "kshowmail.h"
#include "uniqueapp.h"
#include "buildConfig.h"

static const char description[] = I18N_NOOP("A pop3 mail checker");

//the version number is defined in cmake via buildConfig.h.in
static const char version[] = KSHOWMAIL_VERSION;

/**
 * @mainpage
 * This is the API documentation of %KShowmail.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */

int main(int argc, char **argv)
{
  //some things about KShowmail
  KAboutData about( "kshowmail", 0, ki18n( "KShowmail" ), version, ki18n( description ),
                    KAboutData::License_GPL, ki18n("(C) 2000 - 2005 Eggert Ehmke\n(C) 2006 - 2009 Ulrich Weigelt"), KLocalizedString(),
                    "http://kshowmail.sourceforge.net", "ulrich.weigelt@gmx.de");

  //add authors
  about.addAuthor( ki18n( "Eggert Ehmke" ), ki18n( "Initiator and first developer" ), "eggert.ehmke@berlin.de", "http://kshowmail.sourceforge.net" );
  about.addAuthor( ki18n( "Ulrich Weigelt" ), ki18n( "current developer" ), "ulrich.weigelt@gmx.de" );
  about.addAuthor( ki18n( "Allistar Melville" ), KLocalizedString(), "allistar@silvermoon.co.nz" );
  about.addAuthor( ki18n( "Oleg Ivanov" ), KLocalizedString(), "saruman@unigsm.com" );

  //add Translators
  about.setTranslator( ki18nc( "NAME OF TRANSLATORS", "Your names" ), ki18nc( "EMAIL OF TRANSLATORS", "Your emails" ) );

  //Initialize command line arguments
  KCmdLineArgs::init( argc, argv, &about );

  //add special command line options for unique KDE applications
  KUniqueApplication::addCmdLineOptions();

  //test whether the new application to start is unique
  if( KUniqueApplication::start() )
  {
    //yes, wa can start a unique kshowmail instance

    //create a new unique application
    //UniqueApp is a inherited class of KUniqueApplication
    //see uniqueapp.h
    UniqueApp* app = new UniqueApp();

    //start the application
    return app->exec();
  }
  else
  {
    //kshowmail is already running
    kWarning() << I18N_NOOP( "KShowmail is already running!" ) << endl;
  }

  return EXIT_SUCCESS;
}
