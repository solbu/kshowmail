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

#ifndef CONFIGACTIONS_H
#define CONFIGACTIONS_H

#include <string.h>

//QT headers
#include <qlayout.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>


//KDE headers
#include <kcmodule.h>
#include <kgenericfactory.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kfiledialog.h>
#include <kprocess.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

//kshowmail headers
#include "constants.h"

/**
 * @brief Part of the setup dialog to configure the actions.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ConfigActions : public KCModule
{
  Q_OBJECT

  public:
    /**
     * Generic Constructor
     * @param parent parent widget
     * @param args arguments
     */
    explicit ConfigActions( QWidget *parent = 0, const QVariantList & args = QVariantList() );

    /**
    * Destructor
    */
    ~ConfigActions();

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
     * Check box to select whether an alert message will be shown if a new mail is arrived.
     */
    QCheckBox* chkNewMailsAlertWindow;

    /**
     * Check box to select whether the main window will be shown if a new mail is arrived.
     */
    QCheckBox* chkNewMailsMainWindow;

    /**
     * Check box to select whether the system beep will be played if a new mail is arrived.
     */
    QCheckBox* chkNewMailsBeep;

    /**
     * Check box to select whether a sound will be played if a new mail is arrived.
     */
    QCheckBox* chkNewMailsSound;

    /**
     * Check box to select whether a command will be executed if a new mail is arrived.
     */
    QCheckBox* chkNewMailsCommand;

    /**
     * Button to test the sound which will be played if a new mail is arrived.
     */
    KPushButton* btnNewMailsPlaySound;

    /**
     * Button to test the command which will be executed if a new mail is arrived.
     */
    KPushButton* btnNewMailsExecCommand;

    /**
     * Edit line which contains the path to the sound file which will be played if a new mail is arrived.
     */
    KLineEdit* txtNewMailsSound;

    /**
     * Edit line which contains the path to the command which will be executed if a new mail is arrived.
     */
    KLineEdit* txtNewMailsCommand;

    /**
     * Button to open a file dialog to choose the sound file which will be played if a new is arrived.
     */
    KPushButton* btnNewMailsChooseSound;

    /**
     * Button to open a file dialog to choose the command which will be executed if a new mail is arrived.
     */
    KPushButton* btnNewMailsChooseCommand;

    /**
     * Check box to select whether the main window will be minimized if no new mail is arrived.
     */
    QCheckBox* chkNoNewMailsMinimize;

    /**
     * Check box to select whether the application will be terminated if no new mail is arrived.
     */
    QCheckBox* chkNoNewMailsTerminate;

  private slots:

    /**
     * Connected with button btnNewMailsChooseSound.
     * Opens a file dialog to choose the sound file which will be played.
     * Writes the chosen path into the edit line txtNewMailsSound.
     */
    void slotChooseSound();

    /**
     * Connected with button btnNewMailsChooseCommand.
     * Opens a file dialog to choose the command which shall be executed.
     * Writes the chosen path into the edit line txtNewMailsCommand.
     */
    void slotChooseCommand();

    /**
     * Connected with button btnNewMailsPlaySound.
     * Plays the sound.
     */
    void slotPlaySound();

    /**
     * Connected with button btnNewMailsExecCommand.
     * Executes the coommand.
     */
    void slotExecuteCommand();

    /**
     * Connected with check box chkNewMailsSound.
     * Enable or disable the sound configuration items.
     * @param on TRUE - box is checked; FALSE - box is not checked
     */
    void slotSoundToggled( bool on );

    /**
     * Connected with check box chkNewMailsCommand.
     * Enable or disable the command configuration items.
     * @param on TRUE - box is checked; FALSE - box is not checked
     */
    void slotCommandToggled( bool on );

    /**
     * Connected with the configuration items.
     * Calls the slot changed() of KCModule to notify the dialog about the change.
     */
    void slotChanged();
};

#endif
