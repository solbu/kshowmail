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

#ifndef CONFIGDISPLAY_H
#define CONFIGDISPLAY_H

//Qt headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qtooltip.h>

//KDE headers
#include <kcmodule.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <KConfigGroup>

//KShowmail headers
#include <constants.h>


/**
 * @brief Part of the setup dialog to configure the display.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigDisplay : public KCModule
{
Q_OBJECT

  public:
    /**
     * Generic Constructor
     * @param parent parent widget
     * @param args arguments
     */
    explicit ConfigDisplay( QWidget *parent = 0, const QVariantList & args = QVariantList() );

    /**
     * Destructor
     */
    ~ConfigDisplay();

    /**
     * Overloaded method of KCModule.
     * Sets the user interface elements to reflect the current settings stored in the
     * config file.
     */
    virtual void load();

    /**
     * Overloaded method of KCModule.
     * Sets the user interface elements to default values.
     * This method is called when the user clicks the "Default" button.
     */
    virtual void defaults();

    /**
     * Overloaded method of KCModule.
     * Stores the config information as shown in the user interface in the
     * config file.
     * Is called when the user clicks "Apply" or "Ok".
     */
    virtual void save();

  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * Check box to switch on/off the "Active" column in the account list.
     */
    QCheckBox* chkAccountlistActive;

    /**
     * Check box to switch on/off the "Account" column in the account list.
     */
    QCheckBox* chkAccountlistAccount;

    /**
     * Check box to switch on/off the "Server" column in the account list.
     */
    QCheckBox* chkAccountlistServer;

    /**
     * Check box to switch on/off the "User" column in the account list.
     */
    QCheckBox* chkAccountlistUser;

    /**
     * Check box to switch on/off the "Messages" column in the account list.
     */
    QCheckBox* chkAccountlistMessages;

    /**
     * Check box to switch on/off the "Size" column in the account list.
     */
    QCheckBox* chkAccountlistSize;

    /**
     * Check box to switch on/off the "Number" column in the message list.
     */
    QCheckBox* chkMessagelistNumber;

    /**
     * Check box to switch on/off the "Account" column in the message list.
     */
    QCheckBox* chkMessagelistAccount;

    /**
     * Check box to switch on/off the "From" column in the message list.
     */
    QCheckBox* chkMessagelistFrom;

    /**
     * Check box to switch on/off the "To" column in the message list.
     */
    QCheckBox* chkMessagelistTo;

    /**
     * Check box to switch on/off the "Subject" column in the message list.
     */
    QCheckBox* chkMessagelistSubject;

    /**
     * Check box to switch on/off the "Date" column in the message list.
     */
    QCheckBox* chkMessagelistDate;

    /**
     * Check box to switch on/off the "Size" column in the message list.
     */
    QCheckBox* chkMessagelistSize;

    /**
     * Check box to switch on/off the "Content" column in the message list.
     */
    QCheckBox* chkMessagelistContent;

    /**
     * Check box to switch on/off the "State" column in the message list.
     */
    QCheckBox* chkMessagelistState;

    /**
     * Check box to switch on/off HTML in the message view.
     */
    QCheckBox* chkAllowHTML;

  public slots:

    /**
     * Connected with the configuration items.
     * Calls the slot changed() of KCModule to notify the dialog about the change.
     */
    void slotChanged();
};

#endif
