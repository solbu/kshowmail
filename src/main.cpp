#include "kshowmail.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] = I18N_NOOP("A powerful pop3 mail checker");

static const char version[] = "4.0";

int main(int argc, char **argv)
{
    //some things about KShowmail
    KAboutData about( "kshowmail", 0, ki18n( "KShowmail" ), version, ki18n( description ),
                      KAboutData::License_GPL, ki18n("(C) 2000 - 2005 Eggert Ehmke\n(C) 2006 - 2009 Ulrich Weigelt"), KLocalizedString(),
                      "http://kshowmail.sourceforge.net", "ulrich.weigelt@gmx.de");

    about.addAuthor( ki18n( "Eggert Ehmke" ), ki18n( "Initiator and first developer" ), "eggert.ehmke@berlin.de", "http://kshowmail.sourceforge.net" );
    about.addAuthor( ki18n( "Ulrich Weigelt" ), ki18n( "current developer" ), "ulrich.weigelt@gmx.de" );
    about.addAuthor( ki18n( "Allistar Melville" ), KLocalizedString(), "allistar@silvermoon.co.nz" );
    about.addAuthor( ki18n( "Oleg Ivanov" ), KLocalizedString(), "saruman@unigsm.com" );
    
    about.setTranslator( ki18nc( "NAME OF TRANSLATORS", "Your names" ), ki18nc( "EMAIL OF TRANSLATORS", "Your emails" ) );


    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    kshowmail *widget = new kshowmail;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(kshowmail);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //kshowmail *widget = new kshowmail;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //kshowmail *widget = new kshowmail;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
