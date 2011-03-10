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


#include "regexcheckdialog.h"

RegexCheckDialog::RegexCheckDialog( QWidget* parent ): KDialog( parent ) {

  //set the buttons
  setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Try );

  //this is the main widget
  QWidget* mainWidget = new QWidget();

  //layout of the main widget
  QGridLayout* layMain = new QGridLayout( mainWidget );

  //line to enter the regex
  QLabel* lblRegEx = new QLabel( i18nc( "@label:textbox the edit line to enter a regular expression", "Regular Expression:" ), mainWidget );
  layMain->addWidget( lblRegEx, 0, 0 );
  txtRegEx = new KLineEdit( mainWidget );
  layMain->addWidget( txtRegEx, 0, 1 );

  //line to enter a test string
  QLabel* lblTestString = new QLabel( i18nc( "@label:textbox the edit line to enter a phrase to test the regular expression", "Test Phrase:" ), mainWidget );
  layMain->addWidget( lblTestString, 1, 0 );
  txtTestString = new KLineEdit( mainWidget );
  layMain->addWidget( txtTestString, 1, 1 );

  //in this we show the result of the regex check
  QLabel* lblResult = new QLabel( i18nc( "@info:status a line which shows the result of the regex test", "Result:" ), mainWidget );
  layMain->addWidget( lblResult, 2, 0 );
  txtResult = new QLabel( mainWidget );
  txtResult->setAlignment( Qt::AlignCenter );
  QFont resultFont = txtResult->font();
  resultFont.setBold( true );
  txtResult->setFont( resultFont );
  layMain->addWidget( txtResult, 2, 1 );

  //set the main widget
  setMainWidget( mainWidget );

}

void RegexCheckDialog::slotButtonClicked(int button)
{
  //we just do something if the Try Button is clicked.
  //For all other buttons we call the super methode.
  if( button == KDialog::Try ) {

    //the try button does the check
    checkRegex();

  } else {
    KDialog::slotButtonClicked(button);
  }
}

void RegexCheckDialog::checkRegex()
{
  //get the regex and create an object of it
  QString strRegex = txtRegEx->text();
  if( strRegex.isEmpty() ) {
    writeRed( i18nc( "@info:status no regex entered", "No Regex" ) );
    return;
  }
  QRegExp regex( strRegex );

  //check Regex
  if( !regex.isValid() ) {
    KMessageBox::detailedError( this, i18nc( "@info Error message about an invalid regular expression", "The regular expression is invalid."), regex.errorString(), i18nc( "@title:window Title from error message box about a invalid regular expression", "Invalid regular expression") );
    writeRed( i18nc( "@info:status Short error message about an invalid regular expression", "invalid regex") );
    return;
  }

  //get the test string
  QString testPhrase = txtTestString->text();
  if( testPhrase.isEmpty() ) {
    writeRed( i18nc( "@info:status no test phrase entered", "No Test Phrase" ) );
    return;
  }

  //check it
  if( regex.exactMatch( testPhrase ) ) {
    writeGreen( i18nc( "@info:status The test phrase is matched exactly by the regular expression", "Matched" ) );
  } else {
    writeRed( i18nc( "@info:status The text phrase is not matched by the regular expression", "Failed" ) );
  }


}

void RegexCheckDialog::writeBlack( QString text )
{
  //change color
  QPalette pal = txtResult->palette();
  pal.setColor( QPalette::Foreground, Qt::black );
  txtResult->setPalette( pal );

  //set text
  txtResult->setText( text );


}

void RegexCheckDialog::writeGreen( QString text )
{
  //change color
  QPalette pal = txtResult->palette();
  pal.setColor( QPalette::Foreground, Qt::green );
  txtResult->setPalette( pal );

  //set text
  txtResult->setText( text );

}

void RegexCheckDialog::writeRed( QString text )
{
  //change color
  QPalette pal = txtResult->palette();
  pal.setColor( QPalette::Foreground, Qt::red );
  txtResult->setPalette( pal );

  //set text
  txtResult->setText( text );

}

QString RegexCheckDialog::getRegex()
{
  return txtRegEx->text();
}

void RegexCheckDialog::setRegex(QString regex)
{
  txtRegEx->setText( regex );
}


