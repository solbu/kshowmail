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

#include "showheaderdialog.h"

ShowHeaderDialog::ShowHeaderDialog( QWidget * parent, QString & caption, QString & subject, QStringList header ) :
    KDialog( parent )
{
  //create main widget
  QWidget* mainWidget = new QWidget( this );
  setMainWidget( mainWidget );
  setCaption( caption );

  //this layout separates meta data area (subject) from the mail header area
  QVBoxLayout* layMain = new QVBoxLayout( mainWidget );
  mainWidget->setLayout( layMain );

  //this layout arranges the labels and lines for the meta data
  QHBoxLayout* layMetaDatas = new QHBoxLayout();
  layMain->addLayout( layMetaDatas );

  //create label for subject
  QLabel* lblSubject = new QLabel( i18nc( "@label:textbox mail subject", "Subject:" ), mainWidget );
  layMetaDatas->addWidget( lblSubject );

  //create line edit for subject
  KLineEdit* liSubject = new KLineEdit( subject, mainWidget );
  liSubject->setReadOnly( true );
  layMetaDatas->addWidget( liSubject );

  //create text edit for the header
  KTextEdit* txtHeader = new KTextEdit( mainWidget );

  txtHeader->setText( header.join( "\n" ) );
  txtHeader->setMinimumSize( WIDTH_VIEW_MAILHEADER, HEIGHT_VIEW_MAILHEADER );

  layMain->addWidget( txtHeader );
}



ShowHeaderDialog::~ShowHeaderDialog()
{
}


#include "showheaderdialog.moc"
