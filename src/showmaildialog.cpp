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

#include "showmaildialog.h"

ShowMailDialog::ShowMailDialog( QWidget * parent, QString caption, bool allowHTML, bool isHTML, QString sender, QString date, QString size, QString subject, QStringList body ) :
    KDialog( parent )
{

  //set buttons
  //we need OK, CANCEL and the USER1 button
  setButtons( Ok | Cancel | User1 );

  //the User1 button we use to reply a mail
  setButtonText( User1, i18nc( "@action:button to reply a mail", "Reply") );
  setButtonIcon( User1, KIcon( "mail-reply-sender" ) );
  setButtonToolTip( User1, i18nc( "@info:tooltip", "Reply to sender" ) );

  //save some things about the mail to reply it
  m_subject = subject;
  m_sender = sender;
  m_body = body;

  //create main widget
  QWidget* mainWidget = new QWidget( this );
  setMainWidget( mainWidget );
  setCaption( caption );

  //this layout separates meta data area (date, subject, and so on) from the mail body area
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
  QLabel* lblSender = new QLabel( i18nc( "@label:textbox sender of the mail", "Sender:" ), mainWidget );
  layLabels->addWidget( lblSender );

  QLabel* lblDate = new QLabel( i18nc( "@label:textbox send date", "Date:" ), mainWidget );
  layLabels->addWidget( lblDate );

  QLabel* lblSize = new QLabel( i18nc( "@label:textbox mail size", "Size:" ), mainWidget );
  layLabels->addWidget( lblSize );

  QLabel* lblSubject = new QLabel( i18nc( "@label:textbox mail subject", "Subject:" ), mainWidget );
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
  else if( isHTML )
    txtBody->setHtml( body.join( "\n" ) );
  else
    txtBody->setText( body.join( "\n" ) );
  txtBody->setMinimumSize( WIDTH_VIEW_MAILBODY, HEIGHT_VIEW_MAILBODY );

  layMain->addWidget( txtBody );

}

ShowMailDialog::~ShowMailDialog()
{
}

void ShowMailDialog::slotButtonClicked(int button)
{
  //Reply button
  //starts the standard mailer to compose a reply of this mail
  if( button == User1 ) {

    QStringList newBody;
    foreach( const QString& line,  m_body ) {
      newBody.append( "> " + line );
    }

    KToolInvocation::invokeMailer( m_sender,
                                   "",
                                   "",
                                   "Re: " + m_subject,
                                   newBody.join( "\n" )
    );


  }

    KDialog::slotButtonClicked(button);
}

#include "showmaildialog.moc"
