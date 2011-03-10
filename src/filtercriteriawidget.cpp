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

#include "filtercriteriawidget.h"
#include <kmimetype.h>

FilterCriteriaWidget::FilterCriteriaWidget( QWidget *parent )
 : QWidget( parent )
{

  //Set object name
  setObjectName( FILTER_CRITERIA_WIDGET_NAME );

  //this is the main layout
  QVBoxLayout* layMain = new QVBoxLayout();
  this->setLayout( layMain );

  //and now the layouts of the first and second line
  QHBoxLayout* layLine1 = new QHBoxLayout();
  QHBoxLayout* layLine2 = new QHBoxLayout();
  layMain->addLayout( layLine1 );
  layMain->addLayout( layLine2 );

  //Source Select
  cmbSource = new KComboBox( this );
  cmbSource->insertItem( ID_COMBO_SOURCE_FROM, i18nc( "@item:inlistbox the sender of the mail", "From" ) );
  cmbSource->insertItem( ID_COMBO_SOURCE_TO, i18nc( "@item:inlistbox The addressee of the mail", "To" ) );
  cmbSource->insertItem( ID_COMBO_SOURCE_SIZE, i18nc( "@item:inlistbox the mail subject", "Size (Bytes)" ) );
  cmbSource->insertItem( ID_COMBO_SOURCE_SUBJECT, i18nc( "@item:inlistbox the mail subject", "Subject" ) );
  cmbSource->insertItem( ID_COMBO_SOURCE_HEADER, i18nc( "@item:inlistbox the mail header", "Header" ) );
  cmbSource->insertItem( ID_COMBO_SOURCE_ACCOUNT, i18nc( "@item:inlistbox the account which contains this mail", "Account" ) );
  layLine1->addWidget( cmbSource );
  connect( cmbSource, SIGNAL( activated( int ) ), this, SLOT( slotSetWidgets() ) );

  switch( DEFAULT_FILTER_CRITERIA_SOURCE )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_FROM ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_TO ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE     : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_SIZE ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_SUBJECT ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_HEADER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_ACCOUNT ); break;
    default                                           : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_FROM ); break;
  }

  //Comboboxes to select the condition between source and value
  cmbConditionText = new KComboBox( this );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_CONTAINS, i18n( "contains" ) );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_NOT_CONTAINS, i18n( "does not contain" ) );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_EQUALS, i18n( "equals" ) );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_NOT_EQUALS, i18n( "does not equal" ) );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_REGEXPR, i18n( "matches regular expression" ) );
  cmbConditionText->insertItem( ID_COMBO_COND_TEXT_NOT_REGEXPR, i18n( "does not match reg. expr." ) );
  layLine1->addWidget( cmbConditionText );
  connect( cmbConditionText, SIGNAL( activated( int ) ), this, SLOT( slotSetWidgets() ) );

  switch( DEFAULT_FILTER_CRITERIA_COND_TEXT )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS ); break;
  }

  //Comboboxes to select the condition between source and value (text list)
  cmbConditionTextList = new KComboBox( this );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_CONTAINS, i18n( "some line contains" ) );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_NOT_CONTAINS, i18n( "no line contains" ) );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_EQUALS, i18n( "some line equals" ) );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_NOT_EQUALS, i18n( "no line equals" ) );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_REGEXPR, i18n( "some line matches reg. expr." ) );
  cmbConditionTextList->insertItem( ID_COMBO_COND_TEXT_NOT_REGEXPR, i18n( "no line matches reg. expr." ) );
  layLine1->addWidget( cmbConditionTextList );
  connect( cmbConditionTextList, SIGNAL( activated( int ) ), this, SLOT( slotSetWidgets() ) );

  switch( DEFAULT_FILTER_CRITERIA_COND_TEXT )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS ); break;
  }

  cmbConditionNum = new KComboBox( this );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_EQUAL, i18n( "is equal to" ) );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_NOT_EQUAL, i18n( "is not equal to" ) );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_GREATER, i18n( "is greater than" ) );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_GREATER_EQUAL, i18n( "is greater than or equal to" ) );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_LESS, i18n( "is less than" ) );
  cmbConditionNum->insertItem( ID_COMBO_COND_NUM_LESS_EQUAL, i18n( "is less than or equal to" ) );
  layLine1->addWidget( cmbConditionNum );

  switch( DEFAULT_FILTER_CRITERIA_COND_NUM )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL          : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL      : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_NOT_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER        : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_GREATER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL  : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_GREATER_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS           : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_LESS ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL     : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_LESS_EQUAL ); break;
    default                                                   : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_EQUAL ); break;
  }

  //edit line and spin box which hold the comparation values
  txtCompValueText = new KLineEdit( this );
  txtCompValueText->setMinimumWidth( WIDTH_FILTER_TEXT_VALUE_LINE );
  layLine1->addWidget( txtCompValueText );

  spbCompValueNum = new KIntNumInput( DEFAULT_FILTER_CRITERIA_SIZE, this, 10 );
  spbCompValueNum->setMinimum( 0 );
  spbCompValueNum->setSuffix( " Bytes" );
  layLine1->addWidget( spbCompValueNum );

  //Button to open the regular expression editor
  btnOpenRegExpEditor = new KPushButton( KGuiItem( "", "tools-wizard", "Check Regular Expression" ), this );
  layLine1->addWidget( btnOpenRegExpEditor );
  connect( btnOpenRegExpEditor, SIGNAL( clicked() ), this, SLOT( slotOpenRegExpCheck() ) );

  //checkbox to select case sensitve
  chkCaseSensitive = new QCheckBox( i18n( "Case sensitive" ), this );
  chkCaseSensitive->setChecked( DEFAULT_FILTER_CRITERIA_CASE_SENSITIVE ); //set default
  layLine2->addWidget( chkCaseSensitive );

  //separator to the next criteria widget
  layMain->addWidget( new KSeparator( this ) );

  //set widgets dependent on criteria source
  slotSetWidgets();
}


FilterCriteriaWidget::~FilterCriteriaWidget()
{
}

void FilterCriteriaWidget::slotSetWidgets( )
{
  switch( cmbSource->currentIndex() )
  {
    case ID_COMBO_SOURCE_FROM     : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( true );
                                    break;
    case ID_COMBO_SOURCE_TO       : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( true );
                                    break;
    case ID_COMBO_SOURCE_SIZE     : cmbConditionNum->setHidden( false );
                                    spbCompValueNum->setHidden( false );
                                    cmbConditionText->setHidden( true );
                                    txtCompValueText->setHidden( true );
                                    chkCaseSensitive->setHidden( true );
                                    cmbConditionTextList->setHidden( true );
                                    break;
    case ID_COMBO_SOURCE_SUBJECT  : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( true );
                                    break;
    case ID_COMBO_SOURCE_HEADER   : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( true );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( false );
                                    break;
    case ID_COMBO_SOURCE_ACCOUNT  : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( true );
                                    break;
    default:                        cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    cmbConditionTextList->setHidden( true );
                                    break;
  }

  //show or hide widgets of regular expressions
  if( ( !cmbConditionText->isHidden() && ( cmbConditionText->currentIndex() == ID_COMBO_COND_TEXT_REGEXPR || cmbConditionText->currentIndex() == ID_COMBO_COND_TEXT_NOT_REGEXPR ) ) ||
      ( !cmbConditionTextList->isHidden() && ( cmbConditionTextList->currentIndex() == ID_COMBO_COND_TEXT_REGEXPR || cmbConditionTextList->currentIndex() == ID_COMBO_COND_TEXT_NOT_REGEXPR ) ) )
  {
    btnOpenRegExpEditor->setHidden( false );

    //we don't need case sensitve for regular expressions
    chkCaseSensitive->setHidden( true );
  }
  else
  {
    btnOpenRegExpEditor->setHidden( true );
  }
}

void FilterCriteriaWidget::slotOpenRegExpCheck( )
{
  //create check dialog
  QPointer<RegexCheckDialog> dlg = new RegexCheckDialog( this );

  //put the previously entered regex in it
  dlg->setRegex( txtCompValueText->text() );

  //open dialog
  int result = dlg->exec();

  //if the user has clicked on OK, the regex will be transferred into the value edit line
  if( result == RegexCheckDialog::Accepted ) {
    txtCompValueText->setText( dlg->getRegex() );
  }

  delete dlg;
}

void FilterCriteriaWidget::setNumCriteria( int source, int condition, uint value )
{
  //set source
  switch( source )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_SIZE ); break;
    default                                       : kdError() << "FilterCriteriaWidget::setNumCriteria: invalid source parameter." << endl;
                                                    return;
  }

  //show or hide widgets
  slotSetWidgets();

  //set condition
  switch( condition )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL          : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL      : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_NOT_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER        : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_GREATER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL  : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_GREATER_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS           : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_LESS ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL     : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_LESS_EQUAL ); break;
    default                                                   : cmbConditionNum->setCurrentIndex( ID_COMBO_COND_NUM_EQUAL ); break;
  }

  //set comparison value
  spbCompValueNum->setValue( value );

  //show or hide widgets
  slotSetWidgets();
}

void FilterCriteriaWidget::setTextCriteria( int source, int condition, QString value, bool cs )
{
  //set source
  switch( source )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_FROM ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_TO ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_SUBJECT ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_ACCOUNT ); break;
    default                                           : kdError() << "FilterCriteriaWidget::setTextCriteria: invalid source parameter." << endl;
                                                        return;
  }

  //set condition
  switch( condition )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionText->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS ); break;
  }

  //set value
  txtCompValueText->setText( value );

  //set case sensitve
  chkCaseSensitive->setChecked( cs );

  //show or hide widgets
  slotSetWidgets();

}

void FilterCriteriaWidget::setTextListCriteria( int source, int condition, QString value, bool cs )
{
  //set source
  switch( source )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   : cmbSource->setCurrentIndex( ID_COMBO_SOURCE_HEADER ); break;
    default                                           : kdError() << "FilterCriteriaWidget::setTextListCriteria: invalid source parameter." << endl;
                                                        return;
  }

  //set condition
  switch( condition )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionTextList->setCurrentIndex( ID_COMBO_COND_TEXT_CONTAINS ); break;
  }

  //set value
  txtCompValueText->setText( value );

  //set case sensitve
  chkCaseSensitive->setChecked( cs );

  //show or hide widgets
  slotSetWidgets();

}

FilterCriteria_Type FilterCriteriaWidget::getValues()
{
  FilterCriteria_Type crit;    //return value

  switch( cmbSource->currentIndex() )
  {
    case ID_COMBO_SOURCE_FROM     : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM; break;
    case ID_COMBO_SOURCE_TO       : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO; break;
    case ID_COMBO_SOURCE_SIZE     : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE; break;
    case ID_COMBO_SOURCE_SUBJECT  : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT; break;
    case ID_COMBO_SOURCE_HEADER   : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER; break;
    case ID_COMBO_SOURCE_ACCOUNT  : crit.source = CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT; break;
    default                       : crit.source = 99; break;
  }

  if( crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM ||
      crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO ||
      crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT ||
      crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT )
  {
    switch( cmbConditionText->currentIndex() )
    {
      case ID_COMBO_COND_TEXT_CONTAINS      : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS; break;
      case ID_COMBO_COND_TEXT_NOT_CONTAINS  : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS; break;
      case ID_COMBO_COND_TEXT_EQUALS        : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS; break;
      case ID_COMBO_COND_TEXT_NOT_EQUALS    : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS; break;
      case ID_COMBO_COND_TEXT_REGEXPR       : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR; break;
      case ID_COMBO_COND_TEXT_NOT_REGEXPR   : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR; break;
      default                               : crit.condition = 99; break;
    }
  } else if( crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER )
  {
    switch( cmbConditionTextList->currentIndex() )
    {
      case ID_COMBO_COND_TEXT_CONTAINS      : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS; break;
      case ID_COMBO_COND_TEXT_NOT_CONTAINS  : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS; break;
      case ID_COMBO_COND_TEXT_EQUALS        : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS; break;
      case ID_COMBO_COND_TEXT_NOT_EQUALS    : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS; break;
      case ID_COMBO_COND_TEXT_REGEXPR       : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR; break;
      case ID_COMBO_COND_TEXT_NOT_REGEXPR   : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR; break;
      default                               : crit.condition = 99; break;
    }
  } else if ( crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE )
  {
    switch( cmbConditionNum->currentIndex() )
    {
      case ID_COMBO_COND_NUM_EQUAL          : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL; break;
      case ID_COMBO_COND_NUM_NOT_EQUAL      : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL; break;
      case ID_COMBO_COND_NUM_GREATER        : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER; break;
      case ID_COMBO_COND_NUM_GREATER_EQUAL  : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL; break;
      case ID_COMBO_COND_NUM_LESS           : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS; break;
      case ID_COMBO_COND_NUM_LESS_EQUAL     : crit.condition = CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL; break;
      default                               : crit.condition = 99;
    }
  } else
    crit.condition = 99;

    crit.txtValue = txtCompValueText->text();
    crit.numValue = spbCompValueNum->value();
    crit.cs       = chkCaseSensitive->isChecked();

    return crit;
}


#include "filtercriteriawidget.moc"
