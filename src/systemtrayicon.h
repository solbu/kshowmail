/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

//Qt headers
#include <QIcon>
#include <QPainter>
#include <QColor>
#include <QTimer>

//KDE headers
#include <KSystemTrayIcon>
#include <KLocale>
#include <KDebug>
#include <KGlobalSettings>

class SystemTrayIcon : public KSystemTrayIcon
{
  Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent widget
     */
    SystemTrayIcon( QWidget* parent, QIcon trayIcon );

    /**
     * Draws the given number on the icon.
     * @param n number to draw
     * @param color color
     */
    void drawNumber( int n, const QColor& color );

    /**
     * Clears the number from the icon
     */
    void clear ();

    /**
     * Shows a flashing question mark
     */
    void showLooking();

  private:

    /**
     * the icon
     */
    QIcon _icon;

    /**
     * Timer to animate the ?
     */
    QTimer* flashingTimer;

    /**
     * auxiliary flag for the flasher<p>
     * TRUE - question mark is on; FALSE - question mark is off
     */
    bool flasherFlag;

  protected:

    /**
     * Draws a question mark
     */
    void drawLooking();

  protected slots:

    /**
     * Receives the timer event and depending on flasherFlag it calls drawLooking() or clears the icon.
     * @see flasherFlag
     * @see drawLooking
     */
    void slotFlash();
};

#endif // SYSTEMTRAYICON_H
