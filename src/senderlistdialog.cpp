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
#include <qlistview.h>

SenderListDialog::SenderListDialog(QWidget *parent, ListType list )
  : KDialog( parent )
{
  //save list type
  this->list = list;

  //get application config object (kshowmailrc)
  config = KGlobal::config();

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
  QVBoxLayout* layMain = new QVBoxLayout( wdgMain );

  //edit frame
  editFrame = new KEditListBox( wdgMain, "editFrame", true, KEditListBox::Add | KEditListBox::Remove );
  editFrame->setTitle( i18n( "List" ) );
  if( list == White )
    editFrame->listView()->setToolTip( i18n( "A mail whose sender is listed here will pass the filter.\nA mail will be accepted, if its From line incloses a list entry.\nE.g. a line of\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de> is accepted by the entries\nUlrich Weigelt\nulrich.weigelt@gmx.de\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de>" ) );
  else
    editFrame->listView()->setToolTip( i18n( "A mail whose sender is listed here will be hold up by the filter.\nA mail will be stopped, if its From line incloses a list entry.\nE.g. a line of\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de> is filtered by the entries\nUlrich Weigelt\nulrich.weigelt@gmx.de\n\"Ulrich Weigelt\" <ulrich.weigelt@gmx.de>" ) );

  layMain->addWidget( editFrame );

  connect( editFrame->addButton(), SIGNAL( clicked() ), this, SLOT( slotSort() ) );
  connect( editFrame->lineEdit(), SIGNAL( returnPressed( const QString & ) ), this, SLOT( slotSort() ) );

  //this radio buttons we just need in the blacklist
  if( list == Black )
  {
    QGroupBox* gboxAction = new QGroupBox( i18n( "Action" ), wdgMain );
    QHBoxLayout* layAction = new QHBoxLayout();
    gboxAction->setLayout( layAction );
    layMain->addWidget( gboxAction );

    grpAction = new QButtonGroup( NULL );
    btnDelete = new QRadioButton( i18n( "Delete"), gboxAction );
    btnMark = new QRadioButton( i18n( "Mark" ), gboxAction );

    grpAction->addButton( btnDelete, ID_BUTTON_FILTER_SENDERLIST_DELETE );
    grpAction->addButton( btnMark, ID_BUTTON_FILTER_SENDERLIST_MARK );

    btnDelete->setToolTip( i18n( "The mails will be deleted." ) );
    btnMark->setToolTip( i18n( "The mails will be marked." ) );

    layAction->addWidget( btnDelete );
    layAction->addWidget( btnMark );

    //set default
    switch( DEFAULT_FILTER_BLACKLIST_ACTION )
    {
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE  : btnDelete->setChecked( true ); break;
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK    : btnMark->setChecked( true ); break;
      default                                           : btnDelete->setChecked( true ); break;
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
  editFrame->listView()->model()->sort( 0 );
}

void SenderListDialog::slotButtonClicked( int button )
{
  if( button == KDialog::Ok )
  {
    //set config group
    KConfigGroup* configList = new KConfigGroup( config, CONFIG_GROUP_FILTER );

    //save the list of senders
    if( list == Black )
      configList->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST, editFrame->items() );
    else
      configList->writeEntry( CONFIG_ENTRY_FILTER_WHITELIST, editFrame->items() );

    //save blacklist action
    if( list == Black )
    {
    int action = grpAction->checkedId();
    if( action != ID_BUTTON_FILTER_SENDERLIST_DELETE && action != ID_BUTTON_FILTER_SENDERLIST_MARK )
      action = DEFAULT_FILTER_BLACKLIST_ACTION;

      switch( action )
      {
        case ID_BUTTON_FILTER_SENDERLIST_DELETE   : configList->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE ); break;
        case ID_BUTTON_FILTER_SENDERLIST_MARK     : configList->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK ); break;
        default                                   : configList->writeEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE ); break;
      }
    }

    config->sync();

    delete configList;
}

  //call slot of super class to close the dialog
  KDialog::slotButtonClicked( button );

}

void SenderListDialog::fillDialog( )
{
  //set config group
  KConfigGroup* configList = new KConfigGroup( config, CONFIG_GROUP_FILTER );

  //get list of senders
  if( list == Black )
    editFrame->setItems( configList->readEntry( CONFIG_ENTRY_FILTER_BLACKLIST, QStringList() ) );
  else
    editFrame->setItems( configList->readEntry( CONFIG_ENTRY_FILTER_WHITELIST, QStringList() ) );

  //get blacklist action
  if( list == Black )
  {
    switch( configList->readEntry( CONFIG_ENTRY_FILTER_BLACKLIST_ACTION, DEFAULT_FILTER_BLACKLIST_ACTION ) )
    {
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE  : btnDelete->setChecked( true ); break;
      case CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK    : btnMark->setChecked( true ); break;
      default                                           : btnDelete->setChecked( true ); break;
    }
  }

  delete configList;
}


#include "senderlistdialog.moc"
