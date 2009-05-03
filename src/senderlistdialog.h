//
// C++ Interface: senderlistdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SENDERLISTDIALOG_H
#define SENDERLISTDIALOG_H

//Qt headers
#include <qlayout.h>
#include <qtooltip.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>

//KDE headers
#include <kdialogbase.h>
#include <kapplication.h>
#include <keditlistbox.h>
#include <klocale.h>
#include <klineedit.h>
#include <kconfig.h>

//KShowmail headers
#include "../constants.h"

//Button IDs
#define ID_BUTTON_FILTER_SENDERLIST_DELETE 1
#define ID_BUTTON_FILTER_SENDERLIST_MARK   2

/**
 * @brief This is a dialog to edit the blacklist and whitelist. It is used by the class ConfigFilter.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class SenderListDialog : public KDialogBase
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
     * @param name widget name
     */
    SenderListDialog( QWidget *parent = 0, ListType list = Black, const char *name = 0 );

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
    KConfig* config;

    /**
     * Edit Frame
     */
    KEditListBox* editFrame;

    /**
     * Combines the action radio buttons for the blacklist.
     */
    QButtonGroup* grpAction;

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
     * Called if OK was clicked.
     * Stores the entered values.
     * After then it invokes slotOk() of KDialogBase.
     */
    void slotOk();


};

#endif
