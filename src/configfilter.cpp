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

K_PLUGIN_FACTORY( ConfigFilterFactory, registerPlugin<ConfigFilter>(); )
K_EXPORT_PLUGIN( ConfigFilterFactory( "kcm_kshowmailconfigfilter" ) )

ConfigFilter::ConfigFilter( QWidget * parent, const QVariantList & args )
  : KCModule( ConfigFilterFactory::componentData(), parent, args )
{
  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this );

  //check box to activate the filter
  chkActivateFilter = new QCheckBox( i18n( "Activate Filter" ), this );
  layMain->addWidget( chkActivateFilter );
  chkActivateFilter->setToolTip( i18n( "Check to activate the header filter." ) );
  connect( chkActivateFilter, SIGNAL( toggled( bool ) ), this, SLOT( slotFilterActiveToggled( bool ) ) );
  connect( chkActivateFilter, SIGNAL( toggled( bool ) ), this, SLOT( slotChanged() ) );

  //GUI of sender lists
  //-------------------
  gboxSenderLists = new QGroupBox( i18n( "First Check: Sender Lists" ), this );
  QHBoxLayout* laySender = new QHBoxLayout();
  gboxSenderLists->setLayout( laySender );
  layMain->addWidget( gboxSenderLists );

  KIcon picWhiteList = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/button_whitelist.png" ) );
  btnOpenWhitelist = new KPushButton( picWhiteList, i18n( "Whitelist"), gboxSenderLists );
  btnOpenWhitelist->setToolTip( i18n( "Click here to edit the list of senders whose mails shall pass the filter." ) );
  laySender->addWidget( btnOpenWhitelist );
  laySender->setStretchFactor( btnOpenWhitelist, 3 );
  btnOpenWhitelist->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  connect( btnOpenWhitelist, SIGNAL( clicked() ), this, SLOT( slotOpenWhitelist() ) );

  KIcon picBlackList = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/button_blacklist.png" ) );
  btnOpenBlacklist = new KPushButton( picBlackList, i18n( "Blacklist" ), gboxSenderLists );
  btnOpenBlacklist->setToolTip( i18n( "Click here to edit the list of senders whose mails shall be deleted or marked." ) );
  laySender->addWidget( btnOpenBlacklist );
  laySender->setStretchFactor( btnOpenBlacklist, 3 );
  btnOpenBlacklist->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  connect( btnOpenBlacklist, SIGNAL( clicked() ), this, SLOT( slotOpenBlacklist() ) );

  laySender->insertStretch( 0, 2 );
  laySender->insertStretch( 2, 2 );
  laySender->insertStretch( 4, 2 );

  //GUI of filters
  //--------------
  gboxFilters = new QGroupBox( i18n( "Second Check: Filters" ), this );
  QHBoxLayout* layFilters = new QHBoxLayout();
  gboxFilters->setLayout( layFilters );
  layMain->addWidget( gboxFilters );

  listFilters = new QTreeWidget( gboxFilters );
  listFilters->setColumnCount( 3 );
  QStringList labels;
  labels << i18n( "No.") << i18n( "Name" ) << i18n( "Action" );
  listFilters->setHeaderLabels( labels ),
  layFilters->addWidget( listFilters );
  listFilters->setIndentation( 0 );   //the list is sorted by filter number
  listFilters->setAllColumnsShowFocus( true );

  QVBoxLayout* layFiltersButtons = new QVBoxLayout();
  layFilters->addLayout( layFiltersButtons );
  btnAdd = new KPushButton( KStandardGuiItem::add(), gboxFilters );
  btnEdit = new KPushButton( KStandardGuiItem::configure(), gboxFilters );
  btnRemove = new KPushButton( KStandardGuiItem::remove(), gboxFilters );
  btnMoveTop = new KPushButton( KGuiItem( "", "go-top", i18n( "Moves the selected filter at the top" ) ), gboxFilters );
  btnMoveUp = new KPushButton( KGuiItem( "", "go-up", i18n( "Moves the selectKListViewed filter up" ) ), gboxFilters );
  btnMoveDown = new KPushButton( KGuiItem( "", "go-down", i18n( "Moves the selected filter down" ) ), gboxFilters );
  btnMoveBottom = new KPushButton( KGuiItem( "", "go-bottom", i18n( "Moves the selected filter at the bottm" ) ), gboxFilters );
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


//   //GUI of Others
//   //-------------
//   gboxOthers = new QGroupBox( 0, Qt::Horizontal, i18n( "Third Check: Action for all others" ), this, "gboxOthers" );
//   QHBoxLayout* layOthers = new QHBoxLayout( gboxOthers->layout(), 10 );
//   layMain->addWidget( gboxOthers );
// 
//   //create combobox to select action
//   cmbActionOthers = new KComboBox( gboxOthers, "cmbActionOthers" );
//   layOthers->addWidget( cmbActionOthers );
//   QToolTip::add( cmbActionOthers, i18n( "Choose the action for all mails which are not filtered by the steps before." ) );
//   connect( cmbActionOthers, SIGNAL( activated( int ) ), this, SLOT( slotOtherActionChanged( int ) ) );
//   connect( cmbActionOthers, SIGNAL( activated( int ) ), this, SLOT( slotChanged() ) );
// 
//   //insert items
//   cmbActionOthers->insertItem( i18n( "Show" ), ID_COMBO_FILTER_OTHERS_ACTION_PASS );
//   cmbActionOthers->insertItem( i18n( "Delete" ), ID_COMBO_FILTER_OTHERS_ACTION_DELETE );
//   cmbActionOthers->insertItem( i18n( "Mark" ), ID_COMBO_FILTER_OTHERS_ACTION_MARK );
//   cmbActionOthers->insertItem( i18n( "Move" ), ID_COMBO_FILTER_OTHERS_ACTION_MOVE );
//   cmbActionOthers->insertItem( i18n( "Spamcheck" ), ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK );
//   cmbActionOthers->insertItem( i18n( "Ignore" ), ID_COMBO_FILTER_OTHERS_ACTION_IGNORE );
// 
//   //create edit line to defined the mailbox for move
//   txtMailbox = new KLineEdit( gboxOthers );
//   layOthers->addWidget( txtMailbox );
//   connect( txtMailbox, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotChanged() ) );
// 
//   //create wizard button to configure mailbox
//   btnMailboxWizard= new KPushButton( KGuiItem( QString::null, "wizard" ), gboxOthers );
//   layOthers->addWidget( btnMailboxWizard );
//   QToolTip::add( btnMailboxWizard, i18n( "Choose the mailbox" ) );
//   connect( btnMailboxWizard, SIGNAL( clicked() ), this, SLOT( slotOpenMailBoxWizard() ) );
// 
//   //set defaults
//   switch( DEFAULT_FILTER_OTHERS_ACTION )
//   {
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
//   }
//   chkActivateFilter->setChecked( DEFAULT_FILTER_ACTIVE );
// 
//   //enable or disable widgets
//   slotOtherActionChanged( cmbActionOthers->currentItem() );
//   slotFilterActiveToggled( DEFAULT_FILTER_ACTIVE );
// 
  //get application config object (kshowmailrc)
  config = KGlobal::config();
//
//   //init counter for filter item numbering. This variable contains the last assigned number of a filter setup item.
//   //Because it is initialized with zero, the first filter number is 1.
//   lastFilterNumber = 0;

  //load configured values
  load();
}


ConfigFilter::~ConfigFilter()
{
}

void ConfigFilter::load( )
{
  KConfigGroup* configFilter = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //load filter active state
  chkActivateFilter->setChecked( configFilter->readEntry( CONFIG_ENTRY_FILTER_ACTIVE, DEFAULT_FILTER_ACTIVE ) );
  slotFilterActiveToggled( chkActivateFilter->isChecked() );

  //load other action
/*  switch( configFilter->readEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ) )
  {
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
    case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
  }

  //get mailbox name
  if( configFilter->readEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ) == CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE )
    txtMailbox->setText( configFilter->readEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX, DEFAULT_FILTER_ACTION_MOVE_MAILBOX ) );
  else
    txtMailbox->clear();

  //enable or disable widgets for other action
  slotOtherActionChanged( cmbActionOthers->currentItem() ); */

  //get number of filters
  uint numFilters = configFilter->readEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, 0 );

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

  delete configFilter;
}

void ConfigFilter::defaults( )
{
  chkActivateFilter->setChecked( DEFAULT_FILTER_ACTIVE );
  slotFilterActiveToggled( chkActivateFilter->isChecked() );

//   switch( DEFAULT_FILTER_OTHERS_ACTION )
//   {
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_PASS ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_DELETE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MARK ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE       : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_MOVE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE     : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_IGNORE ); break;
//     case CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK  : cmbActionOthers->setCurrentItem( ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
//   }
// 
//   if( DEFAULT_FILTER_OTHERS_ACTION == CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE )
//     txtMailbox->setText( QString( DEFAULT_FILTER_ACTION_MOVE_MAILBOX ) );
//   else
//     txtMailbox->clear();
// 
//   //enable or disable widgets for other action
//   slotOtherActionChanged( cmbActionOthers->currentItem() );

  slotChanged();
}

void ConfigFilter::save( )
{
  KConfigGroup* configFilter = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //save filter active state
  configFilter->writeEntry( CONFIG_ENTRY_FILTER_ACTIVE, chkActivateFilter->isChecked() );

//   //save other action
//   switch( cmbActionOthers->currentItem() )
//   {
//     case ID_COMBO_FILTER_OTHERS_ACTION_PASS      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS ); break;
//     case ID_COMBO_FILTER_OTHERS_ACTION_DELETE    : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE ); break;
//     case ID_COMBO_FILTER_OTHERS_ACTION_MARK      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK ); break;
//     case ID_COMBO_FILTER_OTHERS_ACTION_MOVE      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE ); break;
//     case ID_COMBO_FILTER_OTHERS_ACTION_IGNORE    : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE ); break;
//     case ID_COMBO_FILTER_OTHERS_ACTION_SPAMCHECK : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK ); break;
//     default                                      : config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_ACTION, DEFAULT_FILTER_OTHERS_ACTION ); break;
//   }
// 
//   //save mailbox name
//   if( cmbActionOthers->currentItem() == ID_COMBO_FILTER_OTHERS_ACTION_MOVE )
//   {
//     config->writeEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX, txtMailbox->text() );
//   }
//   else
//   {
//     config->deleteEntry( CONFIG_ENTRY_FILTER_OTHERS_MAILBOX );
//   }
// 
//   //get old number of filters
//   uint oldNumFilter = config->readNumEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, 0 );
// 
//   //save current number of filters
//   config->writeEntry( CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS, lastFilterNumber );
// 
//   //delete surplus filter configurations
//   if( oldNumFilter > lastFilterNumber )
//   {
//     for( uint ctr = lastFilterNumber + 1; ctr <= oldNumFilter; ctr++ )
//     {
//       config->deleteGroup( QString( "%1%2" ).arg( CONFIG_GROUP_FILTER ).arg( ctr ) );
//     }
//   }
// 
//   //save the filters
//   QListViewItemIterator it( listFilters );
// 
//   while( it.current() )
//   {
//     ( (FilterSetupItem*)it.current() )->save();
// 
//     //get next item
//     ++it;
//   }
// 

  config->sync();
}

void ConfigFilter::slotChanged( )
{
  KCModule::changed();
}

void ConfigFilter::slotOpenWhitelist( )
{
  //open list dialog
  SenderListDialog* dlg = new SenderListDialog( this, SenderListDialog::White );
  dlg->exec();

  //delete dialog
  delete dlg;
}

void ConfigFilter::slotOpenBlacklist( )
{
  //open list dialog
  SenderListDialog* dlg = new SenderListDialog( this, SenderListDialog::Black );
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

  if( res == KDialog::Accepted )
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
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;
  
  FilterSetupItem* item = (FilterSetupItem*)items.first();

  //return if no item selected
  if( item == NULL ) return;

  //open setup dialog
  FilterSetupDialog* dlg = new FilterSetupDialog( this, item );
  int res = dlg->exec();

  //inform application setup dialog about changes
  if( res == KDialog::Accepted )
    slotChanged();

  //delete dialog
  delete dlg;

}

void ConfigFilter::slotRemove( )
{
  //get selected item
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;

  FilterSetupItem* item = (FilterSetupItem*)items.first();

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
/*  switch( index )
  {
    case ID_COMBO_FILTER_OTHERS_ACTION_MOVE : txtMailbox->setEnabled( true );
                                              if( txtMailbox->text() == "" ) txtMailbox->setText( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
                                              btnMailboxWizard->setHidden( false );
                                              break;
    default                                 : txtMailbox->setEnabled( false );
                                              btnMailboxWizard->setHidden( true );
                                              break;
  }*/
}

void ConfigFilter::decreaseNumbers( uint number )
{
  //get an iterator of the list view
  QTreeWidgetItemIterator it( listFilters );

  //iterate over the filter list
  while( *it )
  {
    //get number of current filter item
    uint curNumber = ( (FilterSetupItem*)*it)->getNumber();

    //decrease current number, if the given number is lesser or equals
    if( curNumber >= number && curNumber > 0 )
      ( (FilterSetupItem*)*it)->setNumber( curNumber - 1 );

    //get next item
    ++it;
  }
}

void ConfigFilter::slotMoveTop( )
{
  //get selected item
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;

  FilterSetupItem* item = (FilterSetupItem*)items.first();

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
      QTreeWidgetItemIterator it( listFilters );

      while( *it )
      {
        uint curNumber = ( (FilterSetupItem*)*it )->getNumber();
        ( (FilterSetupItem*)*it )->setNumber( curNumber + 1 );

        ++it;
      }


      //inform application setup dialog about changes
      slotChanged();

    }
  }


}

void ConfigFilter::slotMoveBottom( )
{
  //get selected item
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;

  FilterSetupItem* item = (FilterSetupItem*)items.first();

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

      //inform application setup dialog about changes
      slotChanged();
    }

  }
}

void ConfigFilter::slotMoveUp( )
{
  //get selected item
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;

  FilterSetupItem* item = (FilterSetupItem*)items.first();

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

  }
}

void ConfigFilter::slotMoveDown( )
{
  //get selected item
  QList<QTreeWidgetItem*> items = listFilters->selectedItems();
  if( items.isEmpty() ) return;

  FilterSetupItem* item = (FilterSetupItem*)items.first();

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
  }
}

FilterSetupItem* ConfigFilter::getFilterItem( uint num )
{
  FilterSetupItem* foundItem = NULL;
  bool found = false;

  //get Iterator
  QTreeWidgetItemIterator it( listFilters );

  //iterate over th elist to look for the item
  while( *it && !found )
  {
    //get current number
    uint curNumber = ( (FilterSetupItem*)*it )->getNumber();

    //have we found it?
    if( curNumber == num )
    {
      foundItem = (FilterSetupItem*)*it;

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
/*  gboxOthers->setEnabled( filterOn );*/
}

void ConfigFilter::slotOpenMailBoxWizard( )
{
//   MailBoxWizard wizard( this, "malboxwizard" );
//   wizard.setCaption( i18n( "Mailbox Select" ) );
//   int res = wizard.exec();
// 
//   if( res == QDialog::Accepted )
//     txtMailbox->setText( wizard.getPath() );

}



#include "configfilter.moc"
