//
// C++ Implementation: showmaildialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "showmaildialog.h"

ShowMailDialog::ShowMailDialog( QWidget * parent, QString caption, bool allowHTML, QString sender, QString date, QString size, QString subject, QStringList body ) :
    KDialog( parent )
{
  //create main widget
  QWidget* mainWidget = new QWidget( this );
  setMainWidget( mainWidget );
  setCaption( caption );

  //this layout seperates meta data area (date, subject, and so on) from the mail body area
  QVBoxLayout* layMain = new QVBoxLayout();
  mainWidget->setLayout( layMain );

  //this layouts arranges the labels and lines for the meta data
  QHBoxLayout* layMetaDatas = new QHBoxLayout();
  layMain->addLayout( layMetaDatas );
  QVBoxLayout* layLabels = new QVBoxLayout();
  layMetaDatas->addLayout( layLabels );
  QVBoxLayout* layLines = new QVBoxLayout();
  layMetaDatas->addLayout( layLines );

  //create labels for meta data
  QLabel* lblSender = new QLabel( i18n( "Sender:" ), mainWidget );
  layLabels->addWidget( lblSender );

  QLabel* lblDate = new QLabel( i18n( "Date:" ), mainWidget );
  layLabels->addWidget( lblDate );

  QLabel* lblSize = new QLabel( i18n( "Size:" ), mainWidget );
  layLabels->addWidget( lblSize );

  QLabel* lblSubject = new QLabel( i18n( "Subject:" ), mainWidget );
  layLabels->addWidget( lblSubject );

  //create edit lines to show the meta data
  KLineEdit* liSender = new KLineEdit( sender, mainWidget );
  liSender->setReadOnly( true );
  layLines->addWidget( liSender );

  KLineEdit* liDate = new KLineEdit( date, mainWidget );
  liDate->setReadOnly( true );
  layLines->addWidget( liDate );

  KLineEdit* liSize = new KLineEdit( size, mainWidget );
  liSize->setReadOnly( true );
  layLines->addWidget( liSize );

  KLineEdit* liSubject = new KLineEdit( subject, mainWidget );
  liSubject->setReadOnly( true );
  layLines->addWidget( liSubject );

  //create text browser for the mail body
  KTextEdit* txtBody = new KTextEdit( mainWidget );
  txtBody->setReadOnly( true );

  if( !allowHTML )    //set HTML view or not
    txtBody->setPlainText( body.join( "\n" ) );
  else
    txtBody->setText( body.join( "\n" ) );
  txtBody->setMinimumSize( WIDTH_VIEW_MAILBODY, HEIGHT_VIEW_MAILBODY );

  layMain->addWidget( txtBody );

}

ShowMailDialog::~ShowMailDialog()
{
}

#include "showmaildialog.moc"
