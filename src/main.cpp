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


/**
 * @mainpage
 * This is the API documentation of %KShowmail.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */

int main(int argc, char **argv)
{
  //some things about KShowmail
  //the version number is defined in cmake via buildConfig.h.in
  KAboutData about( QByteArray( "kshowmail" ), NULL, ki18nc( "@title application name", "KShowmail" ), QByteArray( KSHOWMAIL_VERSION ), 
                    ki18nc( "@title", "A pop3 mail checker" ),
                    KAboutData::License_GPL, ki18nc( "@info:credit copyright statement", "(C) 2006 - 2009 Ulrich Weigelt" ), KLocalizedString(),
                    QByteArray( "http://kshowmail.sourceforge.net" ), QByteArray( "ulrich.weigelt@gmx.de" ) );

  //add authors
  about.addAuthor( ki18nc( "@info:credit", "Ulrich Weigelt" ), ki18nc( "@info:credit maintainer", "maintainer" ), "ulrich.weigelt@gmx.de" );
  about.addAuthor( ki18nc( "@info:credit", "Eggert Ehmke" ), ki18nc( "@info:credit", "Initiator and first developer" ), "eggert.ehmke@berlin.de", "http://kshowmail.sourceforge.net" );
  about.addAuthor( ki18nc( "@info:credit", "Allistar Melville" ), KLocalizedString(), "allistar@silvermoon.co.nz" );
  about.addAuthor( ki18nc( "@info:credit", "Oleg Ivanov" ), KLocalizedString(), "saruman@unigsm.com" );

  //add Translators
  about.setTranslator( ki18nc( "@info:credit NAME OF TRANSLATORS", "Your names" ), ki18nc( "@info:credit EMAIL OF TRANSLATORS", "Your emails" ) );

  //add credits
  about.addCredit( ki18nc( "@info:credit", "Scott Barninger" ), ki18nc( "@info:credit Who build the RPMs and releases they", "Release Manager") );
  about.addCredit( ki18nc( "@info:credit", "Luca Pedrielli" ), ki18nc( "@info:credit", "a lot of testing and italian translation") );
  about.addCredit( ki18nc( "@info:credit", "Elsa Andrés" ), ki18nc( "@info:credit", "spanish translation") );
  about.addCredit( ki18nc( "@info:credit", "Heimen Stoffels" ), ki18nc( "@info:credit", "dutch translation") );
  about.addCredit( ki18nc( "@info:credit", "Otmar Mak" ), ki18nc( "@info:credit", "handbook and testing") );
  about.addCredit( ki18nc( "@info:credit", "Tor B. Løken" ), ki18nc( "@info:credit", "norwegian translation") );

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
    kWarning() << I18N_NOOP2( "@info shown if Kshowmail was started but it is already running", "KShowmail is already running!" ) << endl;
  }

  return EXIT_SUCCESS;
}
