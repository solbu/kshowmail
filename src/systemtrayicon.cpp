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

#include "systemtrayicon.h"


SystemTrayIcon::SystemTrayIcon( QWidget* parent, QIcon trayIcon ) : KSystemTrayIcon( trayIcon, parent )
{
  //save the icon
  _icon = trayIcon;
  
  //set tool tip
  setToolTip( i18n("KShowmail: a powerful pop3 email checker") );
}


void SystemTrayIcon::drawNumber (int n, const QColor& color)
{
  //reset the icon
  setIcon( _icon );

  //get icon picture
  QPixmap pix = icon().pixmap( 22, 22 );

  //convert number to text
  QString num( QString::number( n ) );

  //paint number into the pixmap
  QPainter p( &pix );
  QFont font = KGlobalSettings::toolBarFont();
  font.setPixelSize( 14 );
  font.setBold( true );
  p.setFont( font );
  p.setPen( color );
  p.drawText( 0, 0, 22, 22, Qt::AlignCenter, num );

  //create a new icon with the pixmap
  QIcon iconWithNumber = QIcon();
  iconWithNumber.addPixmap( pix );

  //set new icon
  setIcon( iconWithNumber );
}

void SystemTrayIcon::clear ()
{
  setIcon( _icon );
}
