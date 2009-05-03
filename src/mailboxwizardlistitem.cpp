//
// C++ Implementation: mailboxwizardlistitem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mailboxwizardlistitem.h"

MailBoxWizardListItem::~MailBoxWizardListItem()
{
}

MailBoxWizardListItem::MailBoxWizardListItem( QListView* parent, QString label, QString path )
 : KListViewItem( parent, label )
{
  this->path = path;
}

QString MailBoxWizardListItem::getPath( )
{
  return path;
}

