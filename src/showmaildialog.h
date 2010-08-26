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
#include <KIcon>
#include <KToolInvocation>

//KShowmail headers
#include "constants.h"

/**
 * @brief This dialog is used in ConfigElem to show a downloaded mail.
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
   * @param isHTML <code>true</code> - the text is really HTML
   * @param sender mail sender
   * @param date date of send
   * @param size mail size
   * @param subject mail subject
   * @param body mail body
   */
  ShowMailDialog( QWidget* parent, QString caption, bool allowHTML, bool isHTML, QString sender, QString date, QString size, QString subject, QStringList body );

  /**
   * Destructor
   */
  ~ShowMailDialog();

private:

  /**
   * @brief Activated when a button is clicked.
   *
   * Overloaded methode of KDialog.
   * Hallo
   */
  void slotButtonClicked( int button );

  /**
   * mail subject
   * @see ShowMailDialog
   * @see slotButtonClicked
   */
  QString m_subject;

  /**
   * mail body
   * @see ShowMailDialog
   * @see slotButtonClicked
   */
  QStringList m_body;

  /**
   * mail sender
   * @see ShowMailDialog
   * @see slotButtonClicked
   */
  QString m_sender;


};

#endif
