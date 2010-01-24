//
// C++ Interface: showmaildialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHOWMAILDIALOG_H
#define SHOWMAILDIALOG_H

//QT headers
#include <qlayout.h>
#include <qlabel.h>
#include <QStringList>

//KDE headers
#include <klocale.h>
#include <KDialog>
#include <klineedit.h>
#include <kdebug.h>
#include <ktextedit.h>
#include <kurl.h>
#include <kapplication.h>

//KShowmail headers
#include "constants.h"

/**
 * This dialog is used in ConfigElem to show a downloaded mail.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
*/
class ShowMailDialog : public KDialog
{

Q_OBJECT

public:

  /**
   * generic constructor
   * @param parent parent of the dialog
   * @param caption the dialog caption
   * @param allowHTML TRUE - allow the dialog to use the HTML in the mail body; FALSE - don't allow
   * @param sender mail sender
   * @param date date of send
   * @param size mail size
   * @param subject mail subject
   * @param body mail body
   */
  ShowMailDialog( QWidget* parent, QString caption, bool allowHTML, QString sender, QString date, QString size, QString subject, QStringList body );

  /**
   * Destructor
   */
  ~ShowMailDialog();

};

#endif
