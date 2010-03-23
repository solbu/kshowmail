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

#include "./accountviewdelegate.h"

AccountViewDelegate::AccountViewDelegate( QObject* parent ) 
	: QItemDelegate( parent )
{
  //load pictures for a active or inactive account
	//used by the combo box to switch the account state
  picActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountActive.png" ) );
  picNotActive = KIcon( KStandardDirs::locate( "data", "kshowmail/pics/accountNotActive.png" ) );

}

QWidget* AccountViewDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index ) const
{
	//just the first column is editable
	if( !index.isValid() ) return NULL;
	if( index.column() != 0 ) return NULL;

	//the editor is a combo box to change between active and inactive state of the account
	KComboBox* cmbEditor = new KComboBox( parent );
	cmbEditor->addItem( KIcon( picActive ), i18n( "active" ) );
	cmbEditor->addItem( KIcon( picNotActive ), i18n( "inactive" ) );
	
	return cmbEditor;
	
}

void AccountViewDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
	//do nothing if the index is not the first column
	if( !index.isValid() ) return;
	if( index.column() != 0 ) return;
	
	//get the active state of the account
	bool state = index.model()->data( index, Qt::EditRole ).toBool();

	//set showed item of the combo box
	KComboBox* box = static_cast<KComboBox*>( editor );
	if( state == true )
	{
		box->setCurrentIndex( 0 );
	}
	else
	{
		box->setCurrentIndex( 1 );
	}
}

void AccountViewDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
	//just the first column can be edited
	if( index.column() != 0 ) return;
	
	//get the value from the editor and give it to account model
	KComboBox* box = static_cast<KComboBox*>( editor );
	switch( box->currentIndex() )
	{
		case 0 : model->setData( index, QVariant( true ) ); break;
		case 1 : model->setData( index, QVariant( false ) ); break;
		default : return;
	}
}

void AccountViewDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry( option.rect.x(), option.rect.y(), option.rect.width(), editor->minimumSizeHint().height() );
}