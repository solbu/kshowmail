//
// C++ Implementation: senderlistdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "senderlistdialog.h"

SenderListDialog::SenderListDialog(QWidget *parent, ListType list, const char *name)
  : KDialogBase( parent, name, true, QString::null, Ok|Cancel, Ok, true )
{
  //save list type
  this->list = list;

  //get application config object (kshowmailrc)
  config = KApplication::kApplication()->config();

  //set caption
  if( list == White )
    setCaption( "Whitelist" );
  else
    setCaption( "Blacklist" );

  //build GUI
  //---------

  //main widget and layout
  QWidget* wdgMain = new QWidget( this );
  setMainWidget( wdgMain );
  QVBoxLayout* layMain = new QVBoxLayout( wdgMain, 0, spacingHint() );

  //edit frame
  editFrame = new KEditListBox( wdgMain, "editFrame", true, KEditListBox::Add | KEditListBox::Remove );
  editFrame->setTitle( i18n( "List" ) );
  if( list == White )
    QToolTip::add( editFrame->listBox(), i18n( "A mail whose sender is listed here will pass the filter.\nA mail will be accepted, if its From line incloses a list entry.\nE.g. a line of\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de> is accepted by the entries\nUlrich Weigelt\nulrich.weigelt@gmx.de\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de>" ) );
  else
    QToolTip::add( editFrame->listBox(), i18n( "A mail whose sender is listed here will be hold up by the filter.\nA mail will be stopped, if its From line incloses a list entry.\nE.g. a line of\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de> is filtered by the entries\nUlrich Weigelt\nulrich.weigelt@gmx.de\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de>" ) );

  layMain->addWidget( editFrame );

  connect( editFrame->addButton(), SIGNAL( clicked() ), this, SLOT( slotSort() ) );
  connect( editFrame->lineEdit(), SIGNAL( returnPressed( const QString & ) ), this, SLOT( slotSort() ) );

  //this radio buttons we just need in the blacklist
  if( list == Black )
  {
    QGroupBox* gboxAction = new QGroupBox( 0, Qt::Horizontal, i18n( "Action" ), wdgMain, "gboxAction" );
    QHBoxLayout* layAction = new QHBoxLayout( gboxAction->layout(), 10 );
    layMain->addWidget( gboxAction );

    grpAction = new QButtonGroup( NULL, "grpAction" );
    QRadioButton* btnDelete = new QRadioButton( i18n( "Delete"), gboxAction, "btnDelete" );
    QRadioButton* btnMark = new QRadioButton( i18n( "Mark" ), gboxAction, "btnMark" );

    grpAction->insert( btnDelete, ID_BUTTON_FILTER_SENDERLIST_DELETE );
    grpAction->insert( btnMark, ID_BUTTON_FILTER_SENDERLIST_MARK );

    QToolTip::add( btnDelete, i18n( "The mails will be deleted." ) );
    QToolTip::add( btnMark, i18n( "The mails will be marked." ) );

    layAction->addWidget( btnDelete );
    layAction->addWidget( btnMark );

    //set default
    switch( DEFAULT_FILTER_BLACKLIST_ACTION )
    {
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE  : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_DELETE ); break;
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK    : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_MARK ); break;
      default                                           : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_DELETE ); break;
    }
  }
  //set size
  setGeometry( 50, 50, (int)( this->width() / 2 ), this->height() );

  //get values
  fillDialog();
}


SenderListDialog::~SenderListDialog()
{
}

void SenderListDialog::slotSort( )
{
  editFrame->listBox()->sort();
}

void SenderListDialog::slotOk( )
{
  //set config group
  config->setGroup( CONFIG_GROUP_FILTER );

  //save the list of senders
  if( list == Black )
    config->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST, editFrame->items() );
  else
    config->writeEntry( CONFIG_ENTRY_FILTER_WHITELIST, editFrame->items() );

  //save blacklist action
  if( list == Black )
  {
   int action = grpAction->selectedId();
   if( action != ID_BUTTON_FILTER_SENDERLIST_DELETE && action != ID_BUTTON_FILTER_SENDERLIST_MARK )
     action = DEFAULT_FILTER_BLACKLIST_ACTION;

    switch( action )
    {
      case ID_BUTTON_FILTER_SENDERLIST_DELETE   : config->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE ); break;
      case ID_BUTTON_FILTER_SENDERLIST_MARK     : config->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK ); break;
      default                                   : config->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE ); break;
    }
  }

  config->sync();

  //call slot of super class to close the dialog
  KDialogBase::slotOk();
}

void SenderListDialog::fillDialog( )
{
  //set config group
  config->setGroup( CONFIG_GROUP_FILTER );

  //get list of senders
  if( list == Black )
    editFrame->setItems( config->readListEntry( CONFIG_ENTRY_FILTER_BLACKLIST ) );
  else
    editFrame->setItems( config->readListEntry( CONFIG_ENTRY_FILTER_WHITELIST ) );

  //get blacklist action
  if( list == Black )
  {
    switch( config->readNumEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, DEFAULT_FILTER_BLACKLIST_ACTION ) )
    {
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE    : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_DELETE ); break;
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK      : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_MARK ); break;
      default                                             : grpAction->setButton( ID_BUTTON_FILTER_SENDERLIST_DELETE ); break;
    }
  }
}


#include "senderlistdialog.moc"
