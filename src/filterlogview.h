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

#ifndef FILTERLOGVIEW_H
#define FILTERLOGVIEW_H

//config group
#define CONFIG_GROUP "FilterLogViewSettings"


//Qt headers
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <QTreeView>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kseparator.h>



//KShowmail headers
#include "filterlog.h"
#include "filterlogviewmovedmodel.h"
#include "filterlogviewdeletedmodel.h"

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
    explicit FilterLogView( QWidget* parent = NULL, FilterLog* log = NULL );

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
    QTreeView* viewDeleted;

    /**
     * Model for the view of deleted mails
     */
    FilterLogViewDeletedModel* modelDeleted;
    
    /**
     * List view of moved mails
     */
    QTreeView* viewMoved;

    /**
     * Model for the view of moved mails
     */
    FilterLogViewMovedModel* modelMoved;

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
