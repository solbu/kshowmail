//
// C++ Implementation: filtercriteriawidget
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filtercriteriawidget.h"

FilterCriteriaWidget::FilterCriteriaWidget( QWidget *parent, const char *name )
 : QWidget( parent, name )
{
  //this is the main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, KDialog::spacingHint(), "layMain" );

  //and now the layouts of the first and second line
  QHBoxLayout* layLine1 = new QHBoxLayout( layMain, KDialog::spacingHint(), "layLine1" );
  QHBoxLayout* layLine2 = new QHBoxLayout( layMain, KDialog::spacingHint(), "layLine2" );

  //Source Select
  cmbSource = new KComboBox( this, "cmbSource" );
  cmbSource->insertItem( i18n( "From" ), ID_COMBO_SOURCE_FROM );
  cmbSource->insertItem( i18n( "To" ), ID_COMBO_SOURCE_TO );
  cmbSource->insertItem( i18n( "Size (Bytes)" ), ID_COMBO_SOURCE_SIZE );
  cmbSource->insertItem( i18n( "Subject" ), ID_COMBO_SOURCE_SUBJECT );
  cmbSource->insertItem( i18n( "Header" ), ID_COMBO_SOURCE_HEADER );
  cmbSource->insertItem( i18n( "Account" ), ID_COMBO_SOURCE_ACCOUNT );
  layLine1->addWidget( cmbSource );
  connect( cmbSource, SIGNAL( activated( int ) ), this, SLOT( slotSetWidgets() ) );

  switch( DEFAULT_FILTER_CRITERIA_SOURCE )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     : cmbSource->setCurrentItem( ID_COMBO_SOURCE_FROM ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       : cmbSource->setCurrentItem( ID_COMBO_SOURCE_TO ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE     : cmbSource->setCurrentItem( ID_COMBO_SOURCE_SIZE ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  : cmbSource->setCurrentItem( ID_COMBO_SOURCE_SUBJECT ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   : cmbSource->setCurrentItem( ID_COMBO_SOURCE_HEADER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  : cmbSource->setCurrentItem( ID_COMBO_SOURCE_ACCOUNT ); break;
    default                                           : cmbSource->setCurrentItem( ID_COMBO_SOURCE_FROM ); break;
  }

  //Comboboxes to select the condition between source and value
  cmbConditionText = new KComboBox( this, "cmbConditionText" );
  cmbConditionText->insertItem( i18n( "contains" ), ID_COMBO_COND_TEXT_CONTAINS );
  cmbConditionText->insertItem( i18n( "does not contain" ), ID_COMBO_COND_TEXT_NOT_CONTAINS );
  cmbConditionText->insertItem( i18n( "equals" ), ID_COMBO_COND_TEXT_EQUALS );
  cmbConditionText->insertItem( i18n( "does not equal" ), ID_COMBO_COND_TEXT_NOT_EQUALS );
  cmbConditionText->insertItem( i18n( "matches regular expression" ), ID_COMBO_COND_TEXT_REGEXPR );
  cmbConditionText->insertItem( i18n( "does not match reg. expr." ), ID_COMBO_COND_TEXT_NOT_REGEXPR );
  layLine1->addWidget( cmbConditionText );
  connect( cmbConditionText, SIGNAL( activated( int ) ), this, SLOT( slotSetWidgets() ) );

  switch( DEFAULT_FILTER_CRITERIA_COND_TEXT )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_CONTAINS ); break;
  }

  cmbConditionNum = new KComboBox( this, "cmbConditionNum" );
  cmbConditionNum->insertItem( i18n( "is equal to" ), ID_COMBO_COND_NUM_EQUAL );
  cmbConditionNum->insertItem( i18n( "is not equal to" ), ID_COMBO_COND_NUM_NOT_EQUAL );
  cmbConditionNum->insertItem( i18n( "is greater than" ), ID_COMBO_COND_NUM_GREATER );
  cmbConditionNum->insertItem( i18n( "is greater than or equal to" ), ID_COMBO_COND_NUM_GREATER_EQUAL );
  cmbConditionNum->insertItem( i18n( "is less than" ), ID_COMBO_COND_NUM_LESS );
  cmbConditionNum->insertItem( i18n( "is less than or equal to" ), ID_COMBO_COND_NUM_LESS_EQUAL );
  layLine1->addWidget( cmbConditionNum );

  switch( DEFAULT_FILTER_CRITERIA_COND_NUM )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL          : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL      : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_NOT_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER        : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_GREATER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL  : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_GREATER_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS           : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_LESS ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL     : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_LESS_EQUAL ); break;
    default                                                   : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_EQUAL ); break;
  }

  //edit line and spin box which hold the comparation values
  txtCompValueText = new KLineEdit( this, "txtCompValueText" );
  txtCompValueText->setMinimumWidth( WIDTH_FILTER_TEXT_VALUE_LINE );
  layLine1->addWidget( txtCompValueText );

  spbCompValueNum = new KIntNumInput( DEFAULT_FILTER_CRITERIA_SIZE, this, 10, "spbCompValueNum" );
  spbCompValueNum->setMinValue( 0 );
  spbCompValueNum->setSuffix( " Bytes" );
  layLine1->addWidget( spbCompValueNum );

  //Button to open the regular expression editor
  btnOpenRegExpEditor = new KPushButton( KGuiItem( "", "edit", "Edit Regualar Expression" ), this, "btnOpenRegExpEditor" );
  layLine1->addWidget( btnOpenRegExpEditor );
  connect( btnOpenRegExpEditor, SIGNAL( clicked() ), this, SLOT( slotOpenRegExpEditor() ) );

  //check whether the KRegExpEditor is available
  kRegExpEditorAvailable = !KTrader::self()->query( "KRegExpEditor/KRegExpEditor" ).isEmpty();

  //checkbox to select case sensitve
  chkCaseSensitive = new QCheckBox( i18n( "Case sensitive" ), this, "chkCaseSensitive" );
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
  switch( cmbSource->currentItem() )
  {
    case ID_COMBO_SOURCE_FROM     : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
    case ID_COMBO_SOURCE_TO       : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
    case ID_COMBO_SOURCE_SIZE     : cmbConditionNum->setHidden( false );
                                    spbCompValueNum->setHidden( false );
                                    cmbConditionText->setHidden( true );
                                    txtCompValueText->setHidden( true );
                                    chkCaseSensitive->setHidden( true );
                                    break;
    case ID_COMBO_SOURCE_SUBJECT  : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
    case ID_COMBO_SOURCE_HEADER   : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
    case ID_COMBO_SOURCE_ACCOUNT  : cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
    default:                        cmbConditionNum->setHidden( true );
                                    spbCompValueNum->setHidden( true );
                                    cmbConditionText->setHidden( false );
                                    txtCompValueText->setHidden( false );
                                    chkCaseSensitive->setHidden( false );
                                    break;
  }

  //show or hide widgets of regular expressions
  if( !cmbConditionText->isHidden() && ( cmbConditionText->currentItem() == ID_COMBO_COND_TEXT_REGEXPR || cmbConditionText->currentItem() == ID_COMBO_COND_TEXT_NOT_REGEXPR ) )
  {
    if( kRegExpEditorAvailable )
      btnOpenRegExpEditor->setHidden( false );

    //we don't need case sensitve for regular expressions
    chkCaseSensitive->setHidden( true );
  }
  else
  {
    btnOpenRegExpEditor->setHidden( true );
  }
}

void FilterCriteriaWidget::slotOpenRegExpEditor( )
{
  QDialog* editorDialog = KParts::ComponentFactory::createInstanceFromQuery<QDialog>( "KRegExpEditor/KRegExpEditor" );

  if( editorDialog != NULL )
  {
    //kdeutils was installed, so the dialog was found fetch the editor interface
    KRegExpEditorInterface* editor = static_cast<KRegExpEditorInterface*>( editorDialog->qt_cast( "KRegExpEditorInterface" ) );
    if( editor == NULL )
    {
      KMessageBox::error( this, i18n( "The Regular Expression Editor could not be initilized." ) );
      return;
    }

    //use the editor.
    editor->setRegExp( txtCompValueText->text() );

    //exec the dialog
    if( editorDialog->exec() == QDialog::Accepted )
      txtCompValueText->setText( editor->regExp() );

    //delete dialog
    delete editorDialog;
  }
  else
    KMessageBox::error( this, i18n( "The Regular Expression Editor is not available." ) );

}

void FilterCriteriaWidget::setNumCriteria( int source, int condition, uint value )
{
  //set source
  switch( source )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE : cmbSource->setCurrentItem( ID_COMBO_SOURCE_SIZE ); break;
    default                                       : kdError() << "FilterCriteriaWidget::setNumCriteria: invalid source parameter." << endl;
                                                    return;
  }

  //show or hide widgets
  slotSetWidgets();

  //set condition
  switch( condition )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL          : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL      : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_NOT_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER        : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_GREATER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL  : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_GREATER_EQUAL ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS           : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_LESS ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL     : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_LESS_EQUAL ); break;
    default                                                   : cmbConditionNum->setCurrentItem( ID_COMBO_COND_NUM_EQUAL ); break;
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
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     : cmbSource->setCurrentItem( ID_COMBO_SOURCE_FROM ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       : cmbSource->setCurrentItem( ID_COMBO_SOURCE_TO ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  : cmbSource->setCurrentItem( ID_COMBO_SOURCE_SUBJECT ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   : cmbSource->setCurrentItem( ID_COMBO_SOURCE_HEADER ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  : cmbSource->setCurrentItem( ID_COMBO_SOURCE_ACCOUNT ); break;
    default                                           : kdError() << "FilterCriteriaWidget::setTextCriteria: invalid source parameter." << endl;
                                                        return;
  }

  //set condition
  switch( condition )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS      : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS  : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_CONTAINS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS        : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS    : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_EQUALS );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR       : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_REGEXPR );break;
    case CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR   : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_NOT_REGEXPR );break;
    default                                                   : cmbConditionText->setCurrentItem( ID_COMBO_COND_TEXT_CONTAINS ); break;
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

  switch( cmbSource->currentItem() )
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
      crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER ||
      crit.source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT )
  {
    switch( cmbConditionText->currentItem() )
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
    switch( cmbConditionNum->currentItem() )
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
