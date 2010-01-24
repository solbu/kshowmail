//
// C++ Interface: filterlogview
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTERLOGVIEW_H
#define FILTERLOGVIEW_H

//config group
#define CONFIG_GROUP "FilterLogViewSettings"


//Qt headers
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <QTreeWidget>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kseparator.h>



//KShowmail headers
#include "filterlog.h"
#include "filterlogviewdeleteditem.h"
#include "filterlogviewmoveditem.h"

/**
 * @brief This dialog box shows the filter log.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class FilterLogView : public KDialog
{

Q_OBJECT

  public:

    /**
     * General constructor
     * @param parent pointer to the parent widget
     * @param log pointer to the filter log
     */
    FilterLogView( QWidget* parent = NULL, FilterLog* log = NULL );

    /**
     * Destructor
     */
    ~FilterLogView();

  private:

    /**
     * Pointer to the filter log.
     */
    FilterLog* log;

    /**
     * List view of deleted mails
     */
    QTreeWidget* lstViewDeleted;

    /**
     * List view of moved mails
     */
    QTreeWidget* lstViewMoved;

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;


  private slots:

    /**
     * Clears the list of deleted mails.
     */
    void slotClearDeletedMails();

    /**
     * Clears th elist of moved mails.
     */
    void slotClearMovedMails();

    /**
     * Overloaded from KDialog.<p>
     * Activated when the button <code>button</code> is clicked.
     * @param button clocked button
     */
    virtual void slotButtonClicked( int button );

};

#endif
