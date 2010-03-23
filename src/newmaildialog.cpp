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

#include "newmaildialog.h"
#include <kdebug.h>

NewMailDialog::NewMailDialog( QWidget* parent ): KDialog( parent )
{
  setCaption( "KShowmail" );

  //the second button is to show the main window
  setButtonText( KDialog::Cancel, i18n( "Show main mindow" ) );
  setButtonToolTip( KDialog::Cancel, i18n( "Shows the main window" ) );
  setButtonWhatsThis( KDialog::Cancel, i18n( "Shows the main window" ) );
  setButtonIcon( KDialog::Cancel, KIcon( "kshowmail" ) );

  //Font of the label
  QFont font( "Sans Serif", 12 );
  font.setBold( true );

  //the picture
  QPixmap pic( KStandardDirs::locate( "data", "kshowmail/pics/newMailPic.png" ) );

  //create the main widget
  QWidget* mainWidget = new QWidget( this );
  QHBoxLayout* layMain = new QHBoxLayout;

  QLabel* lblPic = new QLabel();
  lblPic->setPixmap( pic );
  lblPic->setAlignment( Qt::AlignCenter );
  layMain->addWidget( lblPic );

  QLabel* lblText = new QLabel( i18n( "New mail has arrived!" ) );
  lblText->setAlignment( Qt::AlignCenter );
  lblText->setFont( font );
  layMain->addWidget( lblText );

  mainWidget->setLayout( layMain );
  
  setMainWidget( mainWidget );
}
