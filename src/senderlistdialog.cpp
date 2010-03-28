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
  editFrame->setTitle( i18nc( "@title:window dialog to edit the black- or whitelist", "List" ) );
  if( list == White )
    editFrame->listView()->setToolTip( i18nc( "@info/rich", "A mail whose sender is listed here will pass the filter.<nl/>A mail will be accepted, if its From line incloses a list entry.<nl/>E.g. a line of<nl/>&quot;Ulrich Weigelt&quot; &lt;ulrich.weigelt@gmx.de&gt; is accepted by the entries<nl/>Ulrich Weigelt<nl/>ulrich.weigelt@gmx.de<nl/>&quot;Ulrich Weigelt&quot; &lt;ulrich.weigelt@gmx.de&gt;" ) );
  else
    editFrame->listView()->setToolTip( i18nc( "@info:tooltip", "A mail whose sender is listed here will be hold up by the filter.<nl/>A mail will be stopped, if its From line incloses a list entry.<nl/>E.g. a line of<nl/>&quot;Ulrich Weigelt&quot; &lt;ulrich.weigelt@gmx.de&gt; is filtered by the entries<nl/>Ulrich Weigelt<nl/>ulrich.weigelt@gmx.de<nl/>&quot;Ulrich Weigelt&quot; &lt;ulrich.weigelt@gmx.de&gt;" ) );

  layMain->addWidget( editFrame );

  connect( editFrame->addButton(), SIGNAL( clicked() ), this, SLOT( slotSort() ) );
  connect( editFrame->lineEdit(), SIGNAL( returnPressed( const QString & ) ), this, SLOT( slotSort() ) );

  //this radio buttons we just need in the blacklist
  if( list == Black )
  {
    QGroupBox* gboxAction = new QGroupBox( i18nc( "@title:group action to execute if a blacklisted mail comes in", "Action" ), wdgMain );
    QHBoxLayout* layAction = new QHBoxLayout();
    gboxAction->setLayout( layAction );
    layMain->addWidget( gboxAction );

    grpAction = new QButtonGroup( NULL );
    btnDelete = new QRadioButton( i18nc( "@option:radio delete a blacklisted mail", "Delete"), gboxAction );
    btnMark = new QRadioButton( i18nc( "@option:radio mark a blacklisted mail", "Mark" ), gboxAction );

    grpAction->addButton( btnDelete, ID_BUTTON_FILTER_SENDERLIST_DELETE );
    grpAction->addButton( btnMark, ID_BUTTON_FILTER_SENDERLIST_MARK );

    btnDelete->setToolTip( i18nc( "@info:tooltip", "The mails will be deleted." ) );
    btnMark->setToolTip( i18nc( "@info:tooltip", "The mails will be marked." ) );

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
