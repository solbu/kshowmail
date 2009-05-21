//
// C++ Interface: accountsetupitem
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
     * Constructor
     * @param parent the list view
     */
    AccountSetupItem( QTreeWidget* parent );

    /**
     * Constructor
     * @param parent the list view
     * @param name account name
     */
    AccountSetupItem( QTreeWidget* parent, const QString& name );

    /**
     * Destructor
     */
    ~AccountSetupItem();

    /**
     * Sets account name
     * @param name account name
     */
    void setAccountName( const QString& name );

    /**
     * Returns the account name.
     * @return account name
     */
    QString getAccountName() const;

    /**
     * Sets the server.
     * @param server server
     */
    void setServer( const QString& server );

    /**
     * Returns the server.
     * @return server
     */
    QString getServer() const;

    /**
     * Sets the protocol.
     * @param protocol protocol
     */
    void setProtocol( const QString& protocol );

    /**
     * Returns the protocol.
     * @return protocol
     */
    QString getProtocol() const;

    /**
     * Sets the port number.
     * @param port port number
     */
    void setPort( int port );

    /**
     * Returns the port number.
     * @return port number (0..65535)
     */
    int getPort() const;

    /**
     * Sets the user name.
     * @param user user name
     */
    void setUser( const QString& user );

    /**
     * Returns the user name.
     * @return user name
     */
    QString getUser() const;

    /**
     * Sets the password.
     * @param password password
     */
    void setPassword( const QString& password );

    /**
     * Returns the password.
     * @return password
     */
    QString getPassword() const;

    /**
     * Sets the password storage type.
     * See constants.h for valid values
     * @param type password storage type (CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE, CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE or CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET)
     */
    void setPasswordStorageType( int type );

    /**
     * Returns the password storage type.
     * See constants.h for valid values (CONFIG_VALUE_ACCOUNT_PASSWORD_...)
     * @return storage type
     */
    int getPasswordStorageType() const;

    /**
     * Sets whether the account is active.
     * @param active TRUE - account is active; FALSE - account is not active
     */
    void setActive( bool active );

    /**
     * Returns whether the account is active.
     * @return TRUE - account is active
     * @return FALSE - account is not active
     */
    bool getActive() const;

    /**
     * Sets the transfer security.
     * See constants.h for valid values.
     * @param type transfer security type (CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE, CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL, CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS)
     */
    void setTransferSecurity( int type );

    /**
     * Returns the transfer security.
     * seee constants.h for valid values (CONFIG_VALUE_ACCOUNT_SECTRANSFER_...)
     * @return transfer security
     */
    int getTransferSecurity() const;

    /**
     * Saves this account into applications config.
     * All values are placed in a group called like the account name.
     */
    void save() const;

    /**
     * Loads the options of this account from the application config.
     */
    void load();

    /**
     * Prints the account settings to stdout
     */
    void print();


  private:

    /**
     * Connector to the configuration file
     */
    KSharedConfigPtr config;

    /**
     * Account name
     */
    QString _account;

    /**
     * Server
     */
    QString _server;

    /**
     * Protocol
     */
    QString _protocol;

    /**
     * Port
     */
    int _port;

    /**
     * User Name
     */
    QString _user;

    /**
     * Password
     */
    QString _password;

    /**
     * TRUE - account is active
     */
    bool _active;

    /**
     * password storage type
     * Values are defined in constants.h (CONFIG_VALUE_ACCOUNT_PASSWORD_...)
     */
    int _passwordStorage;

    /**
     * transfer security
     * Values are defined in constants.h (CONFIG_VALUE_ACCOUNT_SECTRANSFER_...)
     */
    int _transferSecurity;

  protected:

    /**
     * Initializes the object.
     */
    void init();

};

#endif
