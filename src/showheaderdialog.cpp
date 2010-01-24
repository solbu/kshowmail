//
// C++ Implementation: showheaderdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "showheaderdialog.h"

ShowHeaderDialog::ShowHeaderDialog( QWidget * parent, QString & caption, QString & subject, QStringList header ) :
    KDialog( parent )
{
  //create main widget
  QWidget* mainWidget = new QWidget( this );
  setMainWidget( mainWidget );
  setCaption( caption );

  //this layout seperates meta data area (subject) from the mail header area
  QVBoxLayout* layMain = new QVBoxLayout( mainWidget );
  mainWidget->setLayout( layMain );

  //this layout arranges the labels and lines for the meta datas
  QHBoxLayout* layMetaDatas = new QHBoxLayout();
  layMain->addLayout( layMetaDatas );

  //create label for subject
  QLabel* lblSubject = new QLabel( i18n( "Subject:" ), mainWidget );
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
