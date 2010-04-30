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

#ifndef ACCOUNTVIEWITEM_H
#define ACCOUNTVIEWITEM_H

class AccountViewItem;

//Qt Header
#include <QString>
#include <QPointer>

//KShowmail headers
#include "account.h"

/**
 * This class is used by AccountViewModel to hold a list of the accounts.<p>
 * It can't use a list of pointers to a account because this pointers can be invalid for
 * a short time after an account was removed. When the account view refreshes in this time
 * the app crashed.
 */
class AccountViewItem
{

  public:

    /**
     * Contructor
     * @param active <code>true</code> - the account is active; <code>false</code> - the account is not active
     * @param name Name of the account
     * @param server server
     * @param user user
     * @param numberMails number of mails
     * @param totalSize total size of all mails
     * @param account pointer to the associated account
     */
    AccountViewItem( bool active, const QString& name, const QString& server, const QString& user, int numberMails, long totalSize, QPointer<Account> account );

    /**
     * Returns whether the account is active
     * @return <code>true</code> - account is active; <code>false</code> - account isn't active
     */
    bool isActive() const;

    /**
     * Returns the account name
     * @return account name
     */
    QString getName() const;

    /**
     * Returns ther server host name
     * @return server host
     */
    QString getServer() const;

    /**
     * Returns the user
     * @return user
     */
    QString getUser() const;

    /**
     * Returns the number of mails
     * @return number of mails
     */
    int getNumberMails() const;

    /**
     * Returns the total size of mails with appended unit.
     * @return total size of mails
     */
    QString getTotalSizeUnit() const;

    /**
     * Returns the total size of mails.
     * @return total size of mails
     */
    long getTotalSize() const;

    /**
     * Returns the pointer to the account
     * @return pointer to the associated account
     */
    QPointer<Account> getAccount() const;
    
    /**
     * Compares this account with <i>other</i> and returns an integer less than,
     * equal to, or greater than zero if this account is less than, equal to,
     * or greater than <i>other</i>.
     * @param other other account
     * @param property the account property which will be compared
     */
    int compare( const AccountViewItem& other, AccountSort_Type property );


  private:

    /**
     * <code>true</code> - the account is active
     */
    bool active;

    /**
     * account name
     */
    QString name;

    /**
     * server
     */
    QString server;

    /**
     * user
     */
    QString user;

    /**
     * number of mails
     */
    int numberMails;

    /**
     * total size of mails, inclusive unit
     */
    long totalSize;

    /**
     * pointer to the account
     */
    QPointer<Account> account;
};

#endif // ACCOUNTVIEWITEM_H
