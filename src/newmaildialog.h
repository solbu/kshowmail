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

#ifndef NEWMAILDIALOG_H
#define NEWMAILDIALOG_H

//Qt headers
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

//KDE headers
#include <kdialog.h>
#include <KLocale>
#include <KStandardDirs>
#include <KDebug>


class NewMailDialog : public KDialog
{

  Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent widget
     */
    NewMailDialog( QWidget* parent );
};

#endif // NEWMAILDIALOG_H
