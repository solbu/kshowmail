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

#include "filtersetupdialog.h"

FilterSetupDialog::FilterSetupDialog( QWidget* parent, FilterSetupItem* item )
  : KDialog( parent )
{
  //save pointer to account and view
  filter = item;

  //set caption
  if( item != NULL )
  {
    if( item->getName() == DEFAULT_FILTER_NAME )
      setCaption( i18nc( "@title:window", "New filter" ) );
    else
      setCaption( i18nc( "@title:window", "Edit filter" ) );
  }

  //create main widget and layout
  QWidget* pgMain = new QWidget( this );
  setMainWidget( pgMain );
  QVBoxLayout* layMain = new QVBoxLayout( pgMain );

  //create filter name widgets
  QHBoxLayout* layName = new QHBoxLayout();
  layMain->addLayout( layName );
  QLabel* lblName = new QLabel( i18nc( "@label:textbox name of the filter", "Name:" ), pgMain );
  layName->addWidget( lblName );
  txtName = new KLineEdit( pgMain );
  layName->addWidget( txtName );

  //create group boxes and layouts for rules and action
  QGroupBox* gboxCriteria = new QGroupBox( i18nc( "@title:group", "Filter Criterias" ), pgMain );
  QVBoxLayout* layCriteria = new QVBoxLayout();
  gboxCriteria->setLayout( layCriteria );
  layMain->addWidget( gboxCriteria );

  QGroupBox* gboxAction = new QGroupBox( i18nc( "@title:group", "Filter Action" ), pgMain );
  QHBoxLayout* layAction = new QHBoxLayout();
  gboxAction->setLayout( layAction );
  layMain->addWidget( gboxAction );

  //radio buttons to select the kind of linkage between filter criterias
  QHBoxLayout* layLinkage = new QHBoxLayout();
  layCriteria->addLayout( layLinkage );
  grpLinkage = new QButtonGroup( NULL );

  QRadioButton* btnMatchAll = new QRadioButton( i18nc( "@option:radio all criterias must match", "Match all of the following" ), gboxCriteria );
  QRadioButton* btnMatchAny = new QRadioButton( i18nc( "@option:radio just any criterias must match", "Match any of the following" ), gboxCriteria );
  grpLinkage->addButton( btnMatchAll, ID_BUTTON_LINKAGE_MATCH_ALL );
  grpLinkage->addButton( btnMatchAny, ID_BUTTON_LINKAGE_MATCH_ANY );
  layLinkage->addWidget( btnMatchAll );
  layLinkage->addWidget( btnMatchAny );

  switch( DEFAULT_FILTER_CRITERIA_LINKAGE )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL : btnMatchAll->setChecked( true ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY : btnMatchAny->setChecked( true ); break;
    default                                             : btnMatchAll->setChecked( true ); break;
  }

  //this widget holds one or more criterias (class FilterCriteriaWidget)
  wdgCriteriasHolding = new QWidget( gboxCriteria );
  layCriteriasHolding = new QVBoxLayout( wdgCriteriasHolding );
  layCriteria->addWidget( wdgCriteriasHolding );

  //buttons to add or remove criterias
  QHBoxLayout* layButtons = new QHBoxLayout();
  layCriteria->addLayout( layButtons );
  btnAddCriteria = new KPushButton( KGuiItem( i18nc( "@action:button show more criterias", "More" ), "button_more", i18nc( "@info:tooltip", "Add a further criteria." ) ), gboxCriteria );
  btnRemoveCriteria = new KPushButton( KGuiItem( i18nc( "@action:button how fewer criterias", "Fewer" ), "button_fewer", i18nc( "@info:tooltip", "Remove the last criteria." ) ), gboxCriteria );
  btnAddCriteria->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  btnRemoveCriteria->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
  layButtons->addWidget( btnAddCriteria );
  layButtons->addWidget( btnRemoveCriteria );
  layButtons->addStretch();
  connect( btnAddCriteria, SIGNAL( clicked() ), this, SLOT( slotAddCriteriaWidget() ) );
  connect( btnRemoveCriteria, SIGNAL( clicked() ), this, SLOT( slotRemoveCriteriaWidget() ) );

  //create combobox to select action
  cmbAction = new KComboBox( gboxAction );
  layAction->addWidget( cmbAction );
  cmbAction->setToolTip( i18nc( "@info:tooltip", "Choose the action for all mails which are filtered by this filter." ) );
  connect( cmbAction, SIGNAL( activated( int ) ), this, SLOT( slotActionChanged( int ) ) );

  //insert items
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_PASS, i18nc( "@item:inlistbox show the filtered mail", "Show" ) );
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_DELETE, i18nc( "@item:inlistbox delete the filtered mail", "Delete" ) );
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_MARK, i18nc( "@item:inlistbox mark the filtered mail", "Mark" ) );
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_MOVE, i18nc( "@item:inlistbox write the filtered mail into a mailbox", "Move" ) );
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_SPAMCHECK, i18nc( "@item:inlistbox check the filtered mail for spam", "Spamcheck" ) );
  cmbAction->insertItem( ID_COMBO_FILTER_ACTION_IGNORE, i18nc( "@item:inlistbox ignore the filtered mail", "Ignore" ) );

  //create edit line to defined the mailbox for move
  txtMailbox = new KLineEdit( gboxAction );
  layAction->addWidget( txtMailbox );

  //create wizard button to configure mailbox
  btnMailboxWizard= new KPushButton( KGuiItem( QString::null, "tools-wizard" ), gboxAction );
  layAction->addWidget( btnMailboxWizard );
  btnMailboxWizard->setToolTip( i18nc( "@info:tooltip", "Choose the mailbox" ) );
  connect( btnMailboxWizard, SIGNAL( clicked() ), this, SLOT( slotOpenMailBoxWizard() ) );

  //set default action
  switch( DEFAULT_FILTER_ACTION )
  {
    case CONFIG_VALUE_FILTER_ACTION_PASS       : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_ACTION_DELETE     : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_ACTION_MARK       : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_ACTION_MOVE       : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_ACTION_SPAMCHECK  : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_SPAMCHECK ); break;
    case CONFIG_VALUE_FILTER_ACTION_IGNORE     : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_IGNORE ); break;
  }

  //enable or disable action widgets
  slotActionChanged( cmbAction->currentIndex() );

  //this adds a first empty criteria widget at the dialog if the given setup item pointer is null
  //(this means we want to create a new filter)
  if( item == NULL )
  {
    slotAddCriteriaWidget();
  }

  //write values of the given filter into the dialog items
  if( filter != NULL )
    fillDialog();
}


FilterSetupDialog::~FilterSetupDialog()
{
}

void FilterSetupDialog::fillDialog( )
{
  //check for valid pointer to the setup item
  if( filter == NULL )
  {
    kdError() << "FilterSetupDialog::fillDialog: The given pointer to the filter setup item is NULL." << endl;
    return;
  }

  //set filter name
 txtName->setText( filter->getName() );

  //set linkage
  QRadioButton* btnToCheck;
  switch( filter->getCriteriaLinkage() )
  {
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL : btnToCheck = static_cast<QRadioButton*>( grpLinkage->button( ID_BUTTON_LINKAGE_MATCH_ALL ) ); break;
    case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY : btnToCheck = static_cast<QRadioButton*>( grpLinkage->button( ID_BUTTON_LINKAGE_MATCH_ANY ) ); break;
    default                                             : kdError() << "FilterSetupDialog::fillDialog: The Filter Setup Item of filter " << filter->getName() << " has returned an invalid linkage value. Set default value." << endl;
                                                          switch( DEFAULT_FILTER_CRITERIA_LINKAGE )
                                                          {
                                                            case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL : btnToCheck = static_cast<QRadioButton*>( grpLinkage->button( ID_BUTTON_LINKAGE_MATCH_ALL ) ); break;
                                                            case CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY : btnToCheck = static_cast<QRadioButton*>( grpLinkage->button( ID_BUTTON_LINKAGE_MATCH_ANY ) ); break;
                                                            default: kdError() << "FilterSetupDialog::fillDialog: The default value DEFAULT_FILTER_CRITERIA_LINKAGE in constants.h is also invalid. Set linkage to ALL." << endl;
                                                                     btnToCheck = static_cast<QRadioButton*>( grpLinkage->button( ID_BUTTON_LINKAGE_MATCH_ALL ) );
                                                                     break;
                                                          }
                                                          break;
  }
  btnToCheck->setChecked( true );


  //set criterias
  FilterCriteriaList_Type criteriaList = filter->getCriteriaList();

  if( !criteriaList.empty() )
  {
    FilterCriteriaList_Type::iterator it;
    for( it = criteriaList.begin(); it != criteriaList.end(); ++it )
    {
      //get source and condition
      int source = (*it).source;
      int condition = (*it).condition;

      //depend on the source we add a numeric or text criteria
      switch( source )
      {
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM     :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO       :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT  :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER   :
        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT  : addCriteriaWidget( source, condition, (*it).txtValue, (*it).cs ); break;

        case CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE     : addCriteriaWidget( source, condition, (*it).numValue ); break;
        default                                           : kdError() << "FilterSetupDialog::fillDialog: The Filter Setup Item of filter " << filter->getName() << " has returned an invalid criteria. Add an empty criteria." << endl;
                                                            break;
      }
    }
  }
  else
  {
    //just add a empty criteria widget
    slotAddCriteriaWidget();
  }


  //set action
  switch( filter->getAction() )
  {
    case CONFIG_VALUE_FILTER_ACTION_PASS      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_ACTION_DELETE    : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_ACTION_MARK      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_ACTION_MOVE      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_ACTION_IGNORE    : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_IGNORE ); break;
    case CONFIG_VALUE_FILTER_ACTION_SPAMCHECK : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_SPAMCHECK ); break;
    default                                   : kdError() << "FilterSetupDialog:: fillDialog: The Filter Setup Item of filter " << filter->getName() << " has returned an invalid action value. Set default value." << endl;
                                                switch( DEFAULT_FILTER_ACTION )
                                                {
                                                  case CONFIG_VALUE_FILTER_ACTION_PASS      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_PASS ); break;
                                                  case CONFIG_VALUE_FILTER_ACTION_DELETE    : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_DELETE ); break;
                                                  case CONFIG_VALUE_FILTER_ACTION_MARK      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MARK ); break;
                                                  case CONFIG_VALUE_FILTER_ACTION_MOVE      : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_MOVE ); break;
                                                  case CONFIG_VALUE_FILTER_ACTION_IGNORE    : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_IGNORE ); break;
                                                  case CONFIG_VALUE_FILTER_ACTION_SPAMCHECK : cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_SPAMCHECK ); break;
                                                  default: kdError() << "FilterSetupDialog::fillDialog: The default value DEFAULT_FILTER_ACTION in constants.h is also invalid. Set action to PASS." << endl;
                                                           cmbAction->setCurrentIndex( ID_COMBO_FILTER_ACTION_PASS );
                                                           break;
                                                }
  }

  //set mailbox name if action is MOVE
  if( filter->getAction() == CONFIG_VALUE_FILTER_ACTION_MOVE )
    txtMailbox->setText( filter->getMailBox() );

  //enable or disable action widgets
  slotActionChanged( cmbAction->currentIndex() );

}

FilterCriteriaWidget* FilterSetupDialog::slotAddCriteriaWidget( )
{
  //create a new widget
  FilterCriteriaWidget* crit = new FilterCriteriaWidget( wdgCriteriasHolding );
  crit->setHidden( false );   //the new widgets are hidden by default

  //insert it into the layout
  layCriteriasHolding->addWidget( crit );

  //hide remove button if necessary
  const QObjectList critList = wdgCriteriasHolding->children();

  if( critList.count() <= 2 )
    btnRemoveCriteria->setEnabled( false );
  else
    btnRemoveCriteria->setEnabled( true );



  return crit;
}

void FilterSetupDialog::slotRemoveCriteriaWidget( )
{
  //get a list of all criteria widgets
  QObjectList critList = wdgCriteriasHolding->children();

  //return, if the holding widget doesn't contain a criteria widget
  //the count of children has to be greater than 2, because the layout is the first child
  //and one criteria must always be present.
  if( critList.count() <= 2 ) return;

  //get iterator
  QListIterator<QObject*> it( critList );
  while( it.hasNext() )
    kdDebug() << it.next()->objectName() << endl;

  //get last child
  it.toBack();
  QObject* obj = it.previous();

  //remove last child
  if( obj->objectName() == FILTER_CRITERIA_WIDGET_NAME )
  {
    ((QWidget*)obj)->setHidden( true );
    layCriteriasHolding->removeWidget( (QWidget*)obj );
    critList.removeAll( obj );
    delete obj;
  }
  else
    kdError() << "FilterSetupDialog::slotRemoveCriteriaWidget: the last object of the criteria widget holding is not a criteria widget." << endl;

  //hide remove button if necessary
  if( critList.count() <= 2 )
    btnRemoveCriteria->setEnabled( false );
  else
    btnRemoveCriteria->setEnabled( true );

}

void FilterSetupDialog::addCriteriaWidget( int source, int condition, uint value )
{
  //check source and condition
  if( source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE )
  {
    kdError() << "FilterSetupDialog::addCriteriaWidget: invalid source parameter." << endl;
    return;
  }

  if( condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL )
  {
    kdError() << "FilterSetupDialog::addCriteriaWidget: invalid condition parameter." << endl;
    return;
  }

  //create widget
  FilterCriteriaWidget* crit = slotAddCriteriaWidget();

  //set values
  crit->setNumCriteria( source, condition, value );
}

void FilterSetupDialog::addCriteriaWidget( int source, int condition, QString value, bool cs )
{
  //check source and condition
  if( source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM &&
      source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO &&
      source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT &&
      source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER &&
      source != CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT )
  {
    kdError() << "FilterSetupDialog::addCriteriaWidget: invalid source parameter." << endl;
    return;
  }

  if( condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR &&
      condition != CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR )
  {
    kdError() << "FilterSetupDialog::addCriteriaWidget: invalid condition parameter." << endl;
    return;
  }

  //create widget
  FilterCriteriaWidget* crit = slotAddCriteriaWidget();

  //set values
  if( source == CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER )
  {
    crit->setTextListCriteria( source, condition, value, cs );
  }
  else
  {
    crit->setTextCriteria( source, condition, value, cs );
  }

}

void FilterSetupDialog::slotButtonClicked( int button )
{
  //call the original slot if the clicked button is not Ok
  if( button != KDialog::Ok )
  {
    KDialog::slotButtonClicked( button );
    return;
  }


  //here we write the settings back to the filter setup item

  //filter name
  filter->setName( txtName->text() );

  //linkage
  switch( grpLinkage->checkedId() )
  {
    case ID_BUTTON_LINKAGE_MATCH_ALL    : filter->setCriteriaLinkage( CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL ); break;
    case ID_BUTTON_LINKAGE_MATCH_ANY    : filter->setCriteriaLinkage( CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY ); break;
    default                             : kdDebug() << "FilterSetupDialog::slotOk: Invalid linkage value. Set default value." << endl;
                                          filter->setCriteriaLinkage( DEFAULT_FILTER_CRITERIA_LINKAGE ); break;
  }

  //action
  switch( cmbAction->currentIndex() )
  {
    case ID_COMBO_FILTER_ACTION_PASS      : filter->setAction( CONFIG_VALUE_FILTER_ACTION_PASS ); break;
    case ID_COMBO_FILTER_ACTION_DELETE    : filter->setAction( CONFIG_VALUE_FILTER_ACTION_DELETE ); break;
    case ID_COMBO_FILTER_ACTION_MARK      : filter->setAction( CONFIG_VALUE_FILTER_ACTION_MARK ); break;
    case ID_COMBO_FILTER_ACTION_MOVE      : filter->setAction( CONFIG_VALUE_FILTER_ACTION_MOVE ); break;
    case ID_COMBO_FILTER_ACTION_IGNORE    : filter->setAction( CONFIG_VALUE_FILTER_ACTION_IGNORE ); break;
    case ID_COMBO_FILTER_ACTION_SPAMCHECK : filter->setAction( CONFIG_VALUE_FILTER_ACTION_SPAMCHECK ); break;
    default                               : kdDebug() << "FilterSetupDialog::slotOk: Invalid action value. Set default value." << endl;
                                            filter->setAction( DEFAULT_FILTER_ACTION ); break;
  }

  //mailbox name if action is MOVE
  if( cmbAction->currentIndex() == ID_COMBO_FILTER_ACTION_MOVE )
    filter->setMailBox( txtMailbox->text() );

  //criteria widgets

  //get list of all children of the criterias holding widget
  QObjectList children = wdgCriteriasHolding->children();

  //get iterator of the list
  QListIterator<QObject*> it( children );

  QObject* obj;   //current child object
  FilterCriteriaList_Type critList;   //list of criterias; will be give to the filter setup item

  //iterate over the children list
  while ( it.hasNext() )
  {
    //get child
    obj = it.next();

    if( obj->objectName() == FILTER_CRITERIA_WIDGET_NAME )  //check for criteria object
    {
      //get the values of the criteria
      FilterCriteria_Type crit = ( (FilterCriteriaWidget*)obj )->getValues();

      //append the new criteria at the end  of the criteria list
      critList.push_back( crit );
    }
  }

  //pass criteria list to filter setup item
  filter->setCriteriaList( critList );

  //call slot of super class to close the dialog
  KDialog::slotButtonClicked( KDialog::Ok );
}

void FilterSetupDialog::slotActionChanged( int index )
{
  switch( index )
  {
    case ID_COMBO_FILTER_ACTION_MOVE : txtMailbox->setEnabled( true );
                                       if( txtMailbox->text() == "" ) txtMailbox->setText( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
                                       btnMailboxWizard->setHidden( false );
                                       break;
    default                          : txtMailbox->setEnabled( false );
                                       btnMailboxWizard->setHidden( true );
                                       break;
  }
}

void FilterSetupDialog::slotOpenMailBoxWizard( )
{
  QPointer<MailBoxWizard> wizard = new MailBoxWizard( this );
  wizard->setWindowTitle( i18nc( "@title:window", "Mailbox Select" ) );
  int res = wizard->exec();

  if( res == QDialog::Accepted )
    txtMailbox->setText( wizard->getPath() );

  delete wizard;

}


#include "filtersetupdialog.moc"
