//
// C++ Interface: mailboxwizardlistitem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
