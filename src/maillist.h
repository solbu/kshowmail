/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef MAILLIST_H
#define MAILLIST_H

//Qt headers
#include <QtCore/QObject>

//KDE headers

//KSHowmail headers
#include "mail.h"


class MailList : public QObject
{
  Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent object
     */
    MailList( QObject* parent );

    /**
     * Destructor
     */
    ~MailList();

    /**
     * Creates a mail object and adds it to the list
     * @param unid UNID of the mail
     */
    void addMail( const QString& unid );

    /**
     * Prints all data of this mail list to stdout
     */
    void print() const;


  private:

    /**
     * List of mails
     */
    QList<Mail*> mails;


};

#endif // MAILLIST_H
