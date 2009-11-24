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

Mail::Mail( long number, const QString& unid, bool isNew, QObject* parent )
	: QObject( parent )
{
	this->unid = unid;
  setNumber( number );
  setNew( isNew );
	
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
  cout << "New Flag: " << isNew() << endl;
  cout << "From: " << from.toStdString() << endl;
  cout << "To: " << to.toStdString() << endl;
  cout << "Content Type: " << contentType.toStdString() << endl;
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

long Mail::getNumber() const
{
	return number;
}

void Mail::setNumber( long number )
{
	this->number = number;
}

void Mail::init()
{
	size = 0;
}

void Mail::setNew( bool isnew )
{
  _new = isnew;
}

bool Mail::isNew( ) const
{
  return _new;
}

void Mail::setHeader(const QStringList & header)
{
  this->header = header;

  //get subject
  QString subject = scanHeader( "Subject" );
  subject = subject.simplified();
  setSubject( subject );

  //extract sender and store it
  QString from = scanHeader( "From" );
  from = from.simplified();
  setFrom( from );

  //extract addressee and store it
  QString to = scanHeader( "To" );
  to = to.simplified();
  setTo( to );

  //extract date and store it
  QString date = scanHeader( "Date" );
  setDate( date );

  //extract content type
  QString content = scanHeader( "Content-Type" );
  content = content.simplified ();

  //remove the stuff after the content type; see RFC 2045
  int posSemicolon = content.indexOf( ';' );
  if( posSemicolon != -1 )
  {
    content.remove( posSemicolon, content.length() - posSemicolon );
  }

  //store content type
  setContent( content );

}

QString Mail::scanHeader(const QString & item) const
{
  //behind the keyword there is a : and a blank
  QString _item( item );
  _item.append( ": " );

  //get the header
  QStringList header = getHeader();

  //searching for the keyword
  for ( QStringList::Iterator it = header.begin(); it != header.end(); ++it ) {
    QString line( *it );
    if( line.startsWith( _item ) )
    {
      //remove the keyword from the line and remove the remained chars
      int lengthKeyword = _item.length();
      return line.remove( 0, lengthKeyword );
    }
  }

  //we have nothing found
  return "";
}

void Mail::setSubject( const QString & subject )
{
  this->subject = subject;
}

void Mail::setFrom( const QString & from )
{
  this->from = from;
}

void Mail::setTo( const QString & to )
{
  this->to = to;
}

void Mail::setDate( const QString& date )
{
/*  DwDateTime dwDate;      //this class represents an RFC-822 date-time;
                          //see mimelib/datetime.h

  //convert and store the date-time
  dwDate.FromString( date );
  dwDate.Parse();
  sendDate.setTime_t( dwDate.AsUnixTime() );*/
}

void Mail::setContent( const QString& contentType )
{
  this->contentType = contentType;
}

QStringList Mail::getHeader() const
{
  return header;
}


