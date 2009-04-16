/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "./mail.h"

Mail::Mail( const QString& unid, QObject* parent )
	: QObject( parent )
{
	this->unid = unid;
	
	init();
}

Mail::~Mail()
{
}

void Mail::setSize( long size )
{
	this->size = size;
}

void Mail::print() const
{
	cout << "Subject: " << getSubject().toStdString() << endl;
	cout << "UNID:    " << getUNID().toStdString() << endl;
	cout << "Number:  " << getNumber() << endl;
	cout << "Size:    " << getSize() << " Bytes" << endl;
}

QString Mail::getSubject() const
{
	return subject;
}

QString Mail::getUNID() const
{
	return unid;
}

long Mail::getSize() const
{
	return size;
}

int Mail::getNumber() const
{
	return number;
}

void Mail::setNumber( int number )
{
	this->number = number;
}

void Mail::init()
{
	number = 0;
	size = 0;
}