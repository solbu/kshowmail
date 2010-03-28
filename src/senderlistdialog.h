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

#ifndef SENDERLISTDIALOG_H
#define SENDERLISTDIALOG_H

//Qt headers
#include <qlayout.h>
#include <qtooltip.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>

//KDE headers
#include <KDialog>
#include <kapplication.h>
#include <keditlistbox.h>
#include <klocale.h>
#include <klineedit.h>
#include <kconfig.h>

//KShowmail headers
#include "constants.h"

//Button IDs
#define ID_BUTTON_FILTER_SENDERLIST_DELETE 1
#define ID_BUTTON_FILTER_SENDERLIST_MARK   2

/**
 * @brief This is a dialog to edit the blacklist and whitelist. It is used by the class ConfigFilter.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class SenderListDialog : public KDialog
{

  Q_OBJECT

  public:

    /**
     * Type of list.
     */
    enum ListType { Black, White };

    /**
     * Constructor
     * @param parent parent widget
     * @param list list which shall be opened
     */
    explicit SenderListDialog( QWidget *parent = 0, ListType list = Black );

    /**
     * Destructor
     */
    ~SenderListDialog();

  private:

    /**
     * list which shall be opened
     */
    ListType list;

    /**
     * Application Config Object
     */
    KSharedConfigPtr config;

    /**
     * Edit Frame
     */
    KEditListBox* editFrame;

    /**
     * Combines the action radio buttons for the blacklist.
     */
    QButtonGroup* grpAction;

    /**
     * Radio button to delete filtered mails
     */
    QRadioButton* btnDelete;

    /**
     * Radio button to mark filtered mails
     */
    QRadioButton* btnMark;

    /**
     * Reads the entries for the dialog from the config file fill them in it.
     */
    void fillDialog();

  private slots:

    /**
     * Connected with add button of the edit frame.
     * Sorts the content of the list box.
     */
    void slotSort();

    /**
     * Overwritten method of KDialogBase.
     * Activated when the button <code>button</code> is clicked.
     * If OK was clicked, it will stores the entered values.
     * After then it invokes <code>slotButtonClicked</code>of KDialogBase.
     * @param button clicked button; is the type KDialog::ButtonCode
     */
    void slotButtonClicked( int button );


};

#endif
