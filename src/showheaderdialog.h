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

#ifndef SHOWHEADERDIALOG_H
#define SHOWHEADERDIALOG_H

//QT headers
#include <qlayout.h>
#include <qlabel.h>
#include <QStringList>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <klineedit.h>
#include <ktextedit.h>

//KShowmail headers
#include "constants.h"

/**
 * @brief Used in ShowRecordElem to show the mail header.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ShowHeaderDialog : public KDialog
{
Q_OBJECT

  public:
    /**
     * Generic constructor.
     * @param parent parent of the dialog
     * @param caption the dialog caption
     * @param subject mail subject
     * @param header mail header
     */
    ShowHeaderDialog( QWidget* parent, QString& caption, QString& subject, QStringList header );

    /**
     * Destructor
     */
    ~ShowHeaderDialog();
};

#endif
