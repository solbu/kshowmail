//
// C++ Interface: mailboxwizard
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAILBOXWIZARD_H
#define MAILBOXWIZARD_H

//C-headers
#include <stdlib.h>

//Qt headers
#include <qwizard.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qdir.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <qmap.h>

//KDE headers
#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <kfiledialog.h>
#include <klistview.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <kdebug.h>

//Kshowmail headers
#include "../constants.h"
#include "mailboxwizardlistitem.h"

/**
 * Wizard to configure the mailbox to which a filteres mail shall be moved.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class MailBoxWizard : public QWizard
{

  Q_OBJECT

  public:

    /**
     * Constructor
     */
    MailBoxWizard( QWidget* parent = 0, const char* name = 0 );

    /**
     * Destructor
     */
    ~MailBoxWizard();

    /**
     * Returns the absolute path of the selected mailbox.
     * @return the path
     */
    QString getPath();

  private:

    /**
     * This contains the path to the mail directory
     */
    KLineEdit* txtMailDir;

    /**
     * This button opens a file dialog to slecht the mail directory
     */
    KPushButton* btnMailDir;

    /**
     * This view shows the available mailboxes in the selected directory
     */
    KListView* lstMailboxes;

    /**
     * Title of page 1.
     * Set by constructor
     */
    QString title1;

    /**
     * Title of page 2.
     * Set by Constructor.
     */
    QString title2;

    /**
     * Returns whether the given directory is a maildir directory
     * @param path the directory for test
     * @return TRUE - directory is a maildir directory
     */
    bool isMailDir( const QDir& path );

    /**
     * Adds a new mailbox list view item to the mailbox list or to a given parent item.
     * @param boxname name of the mailbox
     * @param path path to the directory of this mailbox
     */
    void addMailBoxListItem( QString boxname, QDir path );



  private slots:

    /**
     * Connected with btnMailDir.
     * Opens a file dialog to choose the mail directory.
     */
    void slotOpenDirDialog();

    /**
     * Connected with the signal selected of this wizard.
     * Searchs for mailboxes inside the path typed in page1.
     * @param pageTitle title of the selected page
     */
    void slotPageChanged( const QString& pageTitle );


};

#endif
