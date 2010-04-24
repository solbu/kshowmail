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

#include "systemtrayicon.h"


SystemTrayIcon::SystemTrayIcon( QWidget* parent, QIcon trayIcon, KAction* actionRefresh ) : KSystemTrayIcon( trayIcon, parent )
{
  //save the icon
  _icon = trayIcon;
  
  //set tool tip
  setToolTip( i18n("KShowmail: a POP3 email checker") );

  //create flashing timer
  flashingTimer = new QTimer( this );
  flashingTimer->setSingleShot( false );
  connect( flashingTimer, SIGNAL( timeout() ), this, SLOT( slotFlash() ) );

  //add action
  contextMenu()->addAction( actionRefresh );
}


void SystemTrayIcon::drawNumber (int n, const QColor& color)
{
  //stop the flashing timer
  flashingTimer->stop();
  
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
  //stop the flashing timer
  flashingTimer->stop();

}

void SystemTrayIcon::showLooking() {

  //stop timer
  flashingTimer->stop();
  
  //draw first question mark
  drawLooking();

  //initiate flag
  flasherFlag = true;

  //start timer
  flashingTimer->start( 1000 );
  
}

void SystemTrayIcon::drawLooking() {

  //reset the icon
  setIcon( _icon );

  //get icon picture
  QPixmap pix = icon().pixmap( 22, 22 );

  //paint question mark into the pixmap
  QPainter p( &pix );
  QFont font = KGlobalSettings::toolBarFont();
  font.setPixelSize( 14 );
  font.setBold( true );
  p.setFont( font );
  p.setPen( Qt::black );
  p.drawText( 0, 0, 22, 22, Qt::AlignCenter, "?" );

  //create a new icon with the pixmap
  QIcon iconWithMark = QIcon();
  iconWithMark.addPixmap( pix );

  //set new icon
  setIcon( iconWithMark );
}

void SystemTrayIcon::slotFlash() {

  if( flasherFlag ) {

    //clear the icon
    setIcon( _icon );
    
    flasherFlag = false;
    
  } else {

    //draw the question mark
    drawLooking();
    
    flasherFlag = true;
  }

}
