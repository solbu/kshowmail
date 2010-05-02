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

#ifndef ACCOUNTSETUPITEM_H
#define ACCOUNTSETUPITEM_H

//Qt headers
#include <QTreeWidgetItem>

//KDE headers
#include <KListWidget>
#include <KStandardDirs>
#include <kconfig.h>
#include <kurl.h>
#include <KConfigGroup>
#include <KDebug>

//KShowmail headers
#include "constants.h"
#include "encryption.h"
#include "kwalletaccess.h"

using namespace Encryption;

/**
 * @brief This class is used in ConfigAccounts to show the available accounts in the list view and to store an account setup.
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class AccountSetupItem : public QTreeWidgetItem
{
  public:

    /**
     * @brief Constructor
     * @param parent the list view
     */
    AccountSetupItem( QTreeWidget* parent );

    /**
     * @brief Constructor
     * @param parent the list view
     * @param name account name
     */
    AccountSetupItem( QTreeWidget* parent, const QString& name );

    /**
     * @brief Destructor
     */
    ~AccountSetupItem();

    /**
     * @brief Sets account name
     * @param name account name
     */
    void setAccountName( const QString& name );

    /**
     * @brief Returns the account name.
     * @return account name
     */
    QString getAccountName() const;

    /**
     * @brief Sets the server.
     * @param server server
     */
    void setServer( const QString& server );

    /**
     * @brief Returns the server.
     * @return server
     */
    QString getServer() const;

    /**
     * @brief Sets the protocol.
     * @param protocol protocol
     */
    void setProtocol( const QString& protocol );

    /**
     * @brief Returns the protocol.
     * @return protocol
     */
    QString getProtocol() const;

    /**
     * @brief Sets the port number.
     * @param port port number
     */
    void setPort( int port );

    /**
     * @brief Returns the port number.
     * @return port number (0..65535)
     */
    int getPort() const;

    /**
     * @brief Sets the user name.
     * @param user user name
     */
    void setUser( const QString& user );

    /**
     * @brief Returns the user name.
     * @return user name
     */
    QString getUser() const;

    /**
     * @brief Sets the password.
     * @param password password
     */
    void setPassword( const QString& password );

    /**
     * @brief Returns the password.
     * @return password
     */
    QString getPassword() const;

    /**
     * @brief Sets the password storage type.
     *
     * See constants.h for valid values
     * @param type password storage type (CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE or CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET)
     */
    void setPasswordStorageType( int type );

    /**
     * @brief Returns the password storage type.
     *
     * See constants.h for valid values (CONFIG_VALUE_ACCOUNT_PASSWORD_...)
     * @return storage type
     */
    int getPasswordStorageType() const;

    /**
     * @brief Sets whether the account is active.
     * @param active TRUE - account is active; FALSE - account is not active
     */
    void setActive( bool active );

    /**
     * @brief Returns whether the account is active.
     * @return TRUE - account is active
     * @return FALSE - account is not active
     */
    bool getActive() const;

    /**
     * @brief Sets the transfer security.
     * 
     * See constants.h for valid values.
     * @param type transfer security type (CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE, CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL, CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS)
     */
    void setTransferSecurity( int type );

    /**
     * @brief Returns the transfer security.
     *
     * seee constants.h for valid values (CONFIG_VALUE_ACCOUNT_SECTRANSFER_...)
     * @return transfer security
     */
    int getTransferSecurity() const;

    /**
     * @brief Saves this account into applications config.
     *
     * All values are placed in a group called like the account name.
     */
    void save() const;

    /**
     * @brief Loads the options of this account from the application config.
     */
    void load();

    /**
     * @brief Prints the account settings to stdout
     */
    void print();

    /**
     * @brief Sets whether an unsecure login is allowed
     * @param allowed TRUE - unsecure login is allowd
     */
    void setUnsecureLoginAllowed( bool allowed );

    /**
     * @brief Return whether an unsecure login is allowed.
     * @return TRUE - unsecure login is allowed
     */
    bool getUnsecureLoginAllowed() const;

    

  private:

    /**
     * @brief Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * @brief Account name
     */
    QString _account;

    /**
     * @brief Server
     */
    QString _server;

    /**
     * @brief Protocol
     */
    QString _protocol;

    /**
     * @brief Port
     */
    int _port;

    /**
     * @brief User Name
     */
    QString _user;

    /**
     * @brief Password
     */
    QString _password;

    /**
     * @brief TRUE - account is active
     */
    bool _active;

    /**
     * @brief password storage type
     *
     * Values are defined in constants.h (CONFIG_VALUE_ACCOUNT_PASSWORD_...)
     */
    int _passwordStorage;

    /**
     * @brief transfer security
     *
     * Values are defined in constants.h (CONFIG_VALUE_ACCOUNT_SECTRANSFER_...)
     */
    int _transferSecurity;

    /**
     * @brief TRUE - unsafe login is allowed if a secure login failed
     */
    bool _allowUnsecureLogin;

    
  protected:

    /**
     * @brief Initializes the object.
     */
    void init();

};

#endif
