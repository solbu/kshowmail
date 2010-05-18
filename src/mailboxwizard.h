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
#include <QTreeWidget>

//KDE headers
#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <kfiledialog.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <kdebug.h>

//Kshowmail headers
#include "constants.h"
#include "mailboxwizardlistitem.h"

/**
 * @brief Wizard to configure the mailbox to which a filteres mail shall be moved.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class MailBoxWizard : public QWizard
{

  Q_OBJECT

  public:

    /**
     * @brief Constructor
     * @param parent parent widget
     */
    MailBoxWizard( QWidget* parent = 0 );

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
    QTreeWidget* lstMailboxes;

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
     * @param pageID ID of the showed page
     */
    void slotPageChanged( int pageID );


};

#endif
