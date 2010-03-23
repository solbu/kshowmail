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

#ifndef MAILBOXWIZARDLISTITEM_H
#define MAILBOXWIZARDLISTITEM_H

//Qt-Headers
#include <QTreeWidget>

/**
 * This is a list item in the mailbox list of the mailbox wizard
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class MailBoxWizardListItem : public QTreeWidgetItem
{

  public:

    /**
     * Constructor
     * Constructs a new top-level entry in the mailbox list.
     * @param parent pointer to mailbox list.
     * @param label column content
     * @param path the absolute path to this mailbox
     */
    MailBoxWizardListItem( QTreeWidget* parent, QString label, QString path );

    /**
     * Destructor
     */
    ~MailBoxWizardListItem();

    /**
     * Returns the path to this mailbox.
     * @return path to this mailbox
     */
    QString getPath();

  private:

    /**
     * The path to this mailbox
     */
    QString path;



};

#endif
