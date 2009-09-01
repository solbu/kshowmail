//
// C++ Implementation: configfilter
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configfilter.h"

typedef KGenericFactory<ConfigFilter, QWidget> ConfigFilterFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfigfilter, ConfigFilterFactory(
    "kcm_kshowmailconfigfilter" ) );

ConfigFilter::ConfigFilter( QWidget * parent, const char * name, const QStringList & args )
  : KCModule( ConfigFilterFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configfilter" );

  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //check box to activate the filter
  chkActivateFilter = new QCheckBox( i18n( "Activate Filter" ), this, "chkActivateFilter" );
  layMain->addWidget( chkActivateFilter );
  QToolTip::add( chkActivateFilter, i18n( "Check to activate the header filter." ) );
  connect( chkActivateFilter, SIGNAL( toggled( bool ) ), this, SLOT( slotFilterActiveToggled( bool ) ) );
  connect( chkActivateFilter, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );

  //GUI of sender lists
  //-------------------
  gboxSenderLists = new QGroupBox( 0, Qt::Horizontal, i18n( "First Check: Sender Lists" ), this, "gboxSenderLists" );
  QHBoxLayout* laySender = new QHBoxLayout( gboxSenderLists->layout(), 10 );
  layMain->addWidget( gboxSenderLists );

  btnOpenWhitelist = new KPushButton( KGuiItem( QString( i18n( "Whitelist") ), QString( "mail_forward" ) ), gboxSenderLists, "btnOpenWhitelist" );
  QToolTip::add( btnOpenWhitelist, i18n( "Click here to edit the list of senders whose mails shall pass the filter." ) );
  laySender->addWidget( btnOpenWhitelist );
  laySender->setStretchFactor( btnOpenWhitelist, 3 );
  btnOpenWhitelist->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  connect( btnOpenWhitelist, SIGNAL( clicked() ), this, SLOT( slotOpenWhitelist() ) );

  btnOpenBlacklist = new KPushButton( KGuiItem( QString( i18n( "Blacklist" ) ), QString( "mail_delete" ) ), gboxSenderLists, "btnOpenBlacklist" );
  QToolTip::add( btnOpenBlacklist, i18n( "Click here to edit the list of senders whose mails shall be deleted or marked." ) );
  laySender->addWidget( btnOpenBlacklist );
  laySender->setStretchFactor( btnOpenBlacklist, 3 );
  btnOpenBlacklist->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  connect( btnOpenBlacklist, SIGNAL( clicked() ), this, SLOT( slotOpenBlacklist() ) );

  laySender->insertStretch( 0, 2 );
  laySender->insertStretch( 2, 2 );
  laySender->insertStretch( 4, 2 );

  //GUI of filters
  //--------------
  gboxFilters = new QGroupBox( 0, Qt::Horizontal, i18n( "Second Check: Filters" ), this, "gboxFilters" );
  QHBoxLayout* layFilters = new QHBoxLayout( gboxFilters->layout(), 10 );
  layMain->addWidget( gboxFilters );

  listFilters = new KListView( gboxFilters, "listFilters" );
  listFilters->addColumn( i18n( "No." ) );
  listFilters->addColumn( i18n( "Name" ) );
  listFilters->addColumn( i18n( "Action" ) );
  layFilters->addWidget( listFilters );
  listFilters->setColumnWidthMode( 0, QListView::Maximum );
  listFilters->setColumnWidthMode( 1, QListView::Maximum );
  listFilters->setColumnWidthMode( 2, QListView::Maximum );
  listFilters->setResizeMode( QListView::LastColumn );
  listFilters->setColumnAlignment( 0, Qt::AlignHCenter );
  listFilters->setSorting( 0 );   //the list is sorted by filter number
  listFilters->setAllColumnsShowFocus( true );

  QVBoxLayout* layFiltersButtons = new QVBoxLayout( layFilters, 10 );
  btnAdd = new KPushButton( KStdGuiItem::add(), gboxFilters, "btnAdd" );
  btnEdit = new KPushButton( KStdGuiItem::configure(), gboxFilters, "btnEdit" );
  btnRemove = new KPushButton( KStdGuiItem::remove(), gboxFilters, "btnRemove" );
  btnMoveTop = new KPushButton( KGuiItem( "", "top", i18n( "Moves the selected filter at the top" ) ), gboxFilters, "btnMoveTop" );
  btnMoveUp = new KPushButton( KGuiItem( "", "up", i18n( "Moves the selected filter up" ) ), gboxFilters, "btnMoveUp" );
  btnMoveDown = new KPushButton( KGuiItem( "", "down", i18n( "Moves the selected filter down" ) ), gboxFilters, "btnMoveDown" );
  btnMoveBottom = new KPushButton( KGuiItem( "", "bottom", i18n( "Moves the selected filter at the bottm" ) ), gboxFilters, "btnMoveBottom" );
  btnAdd->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnEdit->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnRemove->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnMoveTop->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnMoveUp->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnMoveDown->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  btnMoveBottom->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
  layFiltersButtons->addWidget( btnAdd );
  layFiltersButtons->addWidget( btnEdit );
  layFiltersButtons->addWidget( btnRemove );
  layFiltersButtons->addItem( new QSpacerItem( 1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
  layFiltersButtons->addWidget( btnMoveTop );
  layFiltersButtons->addWidget( btnMoveUp );
  layFiltersButtons->addWidget( btnMoveDown );
  layFiltersButtons->addWidget( btnMoveBottom );
  connect( btnAdd, SIGNAL( clicked() ), this, SLOT( slotAdd() ) );
  connect( btnEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  connect( btnRemove, SIGNAL( clicked() ), this, SLOT( slotRemove() ) );
  connect( btnMoveTop, SIGNAL( clicked() ), this, SLOT( slotMoveTop() ) );
  connect( btnMoveBottom, SIGNAL( clicked() ), this, SLOT( slotMoveBottom() ) );
  connect( btnMoveUp, SIGNAL( clicked() ), this, SLOT( slotMoveUp() ) );
  connect( btnMoveDown, SIGNAL( clicked() ), this, SLOT( slotMoveDown() ) );


  //GUI of Others
  //-------------
  gboxOthers = new QGroupBox( 0, Qt::Horizontal, i18n( "Third Check: Action for all others" ), this, "gboxOthers" );
  QHBoxLayout* layOthers = new QHBoxLayout( gboxOthers->layout(), 10 );
  layMain->addWidget( gboxOthers );

  //create combobox to select action
  cmbActionOthers = new KComboBox( gboxOthers, "cmbActionOthers" );
  layOthers->addWidget( cmbActionOthers );
  QToolTip::add( cmbActionOthers, i18n( "Choose the action for all mails which are not filtered by the steps before." ) );
  connect( cmbActionOthers, SIGNAL( activated( int ) ), this, SLOT( slotOtherActionChanged( int ) ) );
  connect( cmbActionOthers, SIGNAL( activated( int ) ), this, SLOT( slotChanged() ) );

  //insert items
  cmbActionOthers->insertItem( i18n( "Show" ), ID_COMBO_FILTER_OTHERS_ACTION_PASS );
  cmbActionOthers->insertItem( i18n( "Delete" ), ID_COMBO_FILTER_OTHERS_ACTION_DELETE );
  cmbActionOthers->insertItem( i18n( "Mark" ), ID_COMBO_FILTER_OTHERS_ACTION_MARK );
  cmbActionOthers->insertItem( i18n( "Move" ), ID_COMBO_FILTER_OTHERS_ACTION_MOVE );
  cmbActionOthers->insertItem( i18n( "Spamcheck" ), ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK );
  cmbActionOthers->insertItem( i18n( "Ignore" ), ID_COMBO_FILTER_OTHERS_ACTION_IGNORE );

  //create edit line to defined the mailbox for move
  txtMailbox = new KLineEdit( gboxOthers );
  layOthers->addWidget( txtMailbox );
  connect( txtMailbox, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotChanged() ) );

  //create wizard button to configure mailbox
  btnMailboxWizard= new KPushButton( KGuiItem( QString::null, "wizard" ), gboxOthers );
  layOthers->addWidget( btnMailboxWizard );
  QToolTip::add( btnMailboxWizard, i18n( "Choose the mailbox" ) );
  connect( btnMailboxWizard, SIGNAL( clicked() ), this, SLOT( slotOpenMailBoxWizard() ) );

  //set defaults
  switch( DEFAULT_FILTER_OTHERS_ACTION )
  {
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
  }
  chkActivateFilter->setChecked( DEFAULT_FILTER_ACTIVE );

  //enable or disable widgets
  slotOtherActionChanged( cmbActionOthers->currentItem() );
  slotFilterActiveToggled( DEFAULT_FILTER_ACTIVE );

  //get application config object (kshowmailrc)
  config = KApplication::kApplication()->config();

  //init counter for filter item numbering. This variable contains the last assigned number of a filter setup item.
  //Because it is initialized with zero, the first filter number is 1.
  lastFilterNumber = 0;

  //load configured values
  load();
}


ConfigFilter::~ConfigFilter()
{
}

void ConfigFilter::load( )
{
  config->setGroup( CONFIG_GROUP_FILTER );

  //load filter active state
  chkActivateFilter->setChecked( config->readBoolEntry( CONFIG_ENTRY_FILTER_ACTIVE, DEFAULT_FILTER_ACTIVE ) );
  slotFilterActiveToggled( chkActivateFilter->isOn() );

  //load other action
  switch( config->readNumEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ) )
  {
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
  }

  //get mailbox name
  if( config->readNumEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ) == CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE )
    txtMailbox->setText( config->readEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX, DEFAULT_FILTER_ACTION_MOVE_MAILBOX ) );
  else
    txtMailbox->clear();

  //enable or disable widgets for other action
  slotOtherActionChanged( cmbActionOthers->currentItem() );

  //get number of filters
  uint numFilters = config->readNumEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, 0 );

  //create filter setup items and load theirs settings
  for( uint ctr = 1; ctr <= numFilters; ctr++ )
  {
    //create filter item
    FilterSetupItem* item = new FilterSetupItem( listFilters, ctr );

    //load settings
    item->load();
  }

  //set filter index counter
  lastFilterNumber = numFilters;
}

void ConfigFilter::defaults( )
{
  switch( DEFAULT_FILTER_OTHERS_ACTION )
  {
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
  }

  if( DEFAULT_FILTER_OTHERS_ACTION == CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE )
    txtMailbox->setText( QString( DEFAULT_FILTER_ACTION_MOVE_MAILBOX ) );
  else
    txtMailbox->clear();

  //enable or disable widgets for other action
  slotOtherActionChanged( cmbActionOthers->currentItem() );

  slotChanged();
}

void ConfigFilter::save( )
{
  config->setGroup( CONFIG_GROUP_FILTER );

  //save filter active state
  config->writeEntry( CONFIG_ENTRY_FILTER_ACTIVE, chkActivateFilter->isOn() );

  //save other action
  switch( cmbActionOthers->currentItem() )
  {
    case ID_COMBO_FILTER_OTHERS_ACTION_PASS      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS ); break;
    case ID_COMBO_FILTER_OTHERS_ACTION_DELETE    : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE ); break;
    case ID_COMBO_FILTER_OTHERS_ACTION_MARK      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK ); break;
    case ID_COMBO_FILTER_OTHERS_ACTION_MOVE      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE ); break;
    case ID_COMBO_FILTER_OTHERS_ACTION_IGNORE    : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE ); break;
    case ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
    default                                      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ); break;
  }

  //save mailbox name
  if( cmbActionOthers->currentItem() == ID_COMBO_FILTER_OTHERS_ACTION_MOVE )
  {
    config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX, txtMailbox->text() );
  }
  else
  {
    config->deleteEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX );
  }

  //get old number of filters
  uint oldNumFilter = config->readNumEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, 0 );

  //save current number of filters
  config->writeEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, lastFilterNumber );

  //delete surplus filter configurations
  if( oldNumFilter > lastFilterNumber )
  {
    for( uint ctr = lastFilterNumber + 1; ctr <= oldNumFilter; ctr++ )
    {
      config->deleteGroup( QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( ctr ) );
    }
  }

  //save the filters
  QListViewItemIterator it( listFilters );

  while( it.current() )
  {
    ( (FilterSetupItem*)it.current() )->save();

    //get next item
    ++it;
  }


  config->sync();
}

void ConfigFilter::slotChanged( )
{
  KCModule::changed();
}

void ConfigFilter::slotOpenWhitelist( )
{
  //open list dialog
  SenderListDialog* dlg = new SenderListDialog( this, SenderListDialog::White, "dlgWhiteList" );
  dlg->exec();

  //delete dialog
  delete dlg;
}

void ConfigFilter::slotOpenBlacklist( )
{
  //open list dialog
  SenderListDialog* dlg = new SenderListDialog( this, SenderListDialog::Black, "dlgBlackList" );
  dlg->exec();

  //delete dialog
  delete dlg;
}

void ConfigFilter::slotAdd( )
{
  //create a new filter setup item
  FilterSetupItem* item = new FilterSetupItem( listFilters , lastFilterNumber + 1 );

  //open setup dialog
  FilterSetupDialog* dlg = new FilterSetupDialog( this, item );
  int res = dlg->exec();

  if( res == KDialogBase::Accepted )
  {
    //inform application setup dialog about changes
    slotChanged();

    //increase variable for the last assigned filter number
    lastFilterNumber++;
  }
  else
    //delete new item because the user don't want it
    delete item;

  //delete dialog
  delete dlg;
}

void ConfigFilter::slotEdit( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  //return if no item selected
  if( item == NULL ) return;

  //open setup dialog
  FilterSetupDialog* dlg = new FilterSetupDialog( this, item );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialogBase::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;

}

void ConfigFilter::slotRemove( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  if( item != NULL )
  {
    //get number of the item
    uint num = item->getNumber();

    //delete item
    delete item;

    //decrease the number of all filter items after the removed one
    decreaseNumbers( num + 1 );

    //decrease last assigned number
    lastFilterNumber--;

    //inform application setup dialog about changes
    slotChanged();
  }
}

void ConfigFilter::slotOtherActionChanged( int index )
{
  switch( index )
  {
    case ID_COMBO_FILTER_OTHERS_ACTION_MOVE : txtMailbox->setEnabled( true );
                                              if( txtMailbox->text() == "" ) txtMailbox->setText( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
                                              btnMailboxWizard->setHidden( false );
                                              break;
    default                                 : txtMailbox->setEnabled( false );
                                              btnMailboxWizard->setHidden( true );
                                              break;
  }
}

void ConfigFilter::decreaseNumbers( uint number )
{
  //get an iterator of the list view
  QListViewItemIterator it( listFilters );

  //iterate over the filter list
  while( it.current() )
  {
    //get number of current filter item
    uint curNumber = ( (FilterSetupItem*)it.current() )->getNumber();

    //decrease current number, if the given number is lesser or equals
    if( curNumber >= number && curNumber > 0 )
      ( (FilterSetupItem*)it.current() )->setNumber( curNumber - 1 );

    //get next item
    ++it;
  }
}

void ConfigFilter::slotMoveTop( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  if( item != NULL )
  {
    //get number of the item
    uint num = item->getNumber();

    if( num > 1 )
    {
      //set number of the selected to zero, this will move it at the top
      item->setNumber( 0 );

      //now we move all items after the old position one step up
      decreaseNumbers( num + 1 );

      //at last we add one to all numbers
      QListViewItemIterator it( listFilters );

      while( it.current() )
      {
        uint curNumber = ( (FilterSetupItem*)it.current() )->getNumber();
        ( (FilterSetupItem*)it.current() )->setNumber( curNumber + 1 );

        ++it;
      }

      //refresh list view
      listFilters->sort();

      //inform application setup dialog about changes
      slotChanged();

    }
  }


}

void ConfigFilter::slotMoveBottom( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  if( item != NULL )
  {
    //get number of the item
    uint num = item->getNumber();

    if( num < lastFilterNumber )
    {
      //move the selected item at the bottom
      item->setNumber( lastFilterNumber + 1 );

      //move all items after the old position one step up (decrease number)
      decreaseNumbers( num + 1 );

      //refresh list view
      listFilters->sort();

      //inform application setup dialog about changes
      slotChanged();
    }

  }
}

void ConfigFilter::slotMoveUp( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  if( item != NULL )
  {
    //get number of the item
    uint num = item->getNumber();

    if( num > 1 )
    {
      //get item before the selected one
      FilterSetupItem* itemBefore = getFilterItem( num - 1 );

      if( itemBefore != NULL )
      {
        //swap numbers
        item->setNumber( num - 1 );
        itemBefore->setNumber( num );

        //inform application setup dialog about changes
        slotChanged();

      }
    }
    //refresh list view
    listFilters->sort();
  }
}

void ConfigFilter::slotMoveDown( )
{
  //get selected item
  FilterSetupItem* item = (FilterSetupItem*)listFilters->selectedItem();

  if( item != NULL )
  {
    //get number of the item
    uint num = item->getNumber();

    if( num < lastFilterNumber )
    {
      //get item behind the selected one
      FilterSetupItem* itemBehind = getFilterItem( num + 1 );

      if( itemBehind != NULL )
      {
        //swap numbers
        item->setNumber( num + 1 );
        itemBehind->setNumber( num );

        //inform application setup dialog about changes
        slotChanged();

      }
    }
    //refresh list view
    listFilters->sort();
  }
}

FilterSetupItem* ConfigFilter::getFilterItem( uint num )
{
  FilterSetupItem* foundItem = NULL;
  bool found = false;

  //get Iterator
  QListViewItemIterator it( listFilters );

  //iterate over th elist to look for the item
  while( it.current() && !found )
  {
    //get current number
    uint curNumber = ( (FilterSetupItem*)it.current() )->getNumber();

    //have we found it?
    if( curNumber == num )
    {
      foundItem = (FilterSetupItem*)it.current();

      found = true;
    }

    ++it;
  }

  return foundItem;
}

void ConfigFilter::slotFilterActiveToggled( bool filterOn )
{
  gboxSenderLists->setEnabled( filterOn );
  gboxFilters->setEnabled( filterOn );
  gboxOthers->setEnabled( filterOn );
}

void ConfigFilter::slotOpenMailBoxWizard( )
{
  MailBoxWizard wizard( this, "malboxwizard" );
  wizard.setCaption( i18n( "Mailbox Select" ) );
  int res = wizard.exec();

  if( res == QDialog::Accepted )
    txtMailbox->setText( wizard.getPath() );

}



#include "configfilter.moc"