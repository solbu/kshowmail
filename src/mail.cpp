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
  cout << "Send Date: " << sendDate.toString( KDateTime::LocalDate ).toStdString() << endl;
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
  markAtViewRefresh = false;
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
  setSubject( subject);

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
  this->subject = decodeRfc2047( subject );
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
  KMime::Headers::Date mimeDate;
  mimeDate.from7BitString( QByteArray( date.toAscii() ) );
  sendDate = mimeDate.dateTime();
}

void Mail::setContent( const QString& contentType )
{
  this->contentType = contentType;
}

QStringList Mail::getHeader() const
{
  return header;
}

QString Mail::decodeRfc2047( const QString& text ) const
{
  //header informations can be encoded with RFC2047B or RFC2047Q
  //to get the right codec we have to look into the encoding string
  KMime::Codec* codec;

  //Look for RFC2047B
  QString input;
  QRegExp regex( "=\\?.*\\?[bB]\\?.*\\?=" );
  if( regex.indexIn( text ) != -1 )
  {
    //to get the codec for RFC2047B you must pass a "b"
    codec = KMime::Codec::codecForName( "b" );
    
  }
  else
  {
    regex= QRegExp( "=\\?.*\\?[qQ]\\?.*\\?=" );
    if( regex.indexIn( text ) != -1 )
    {
      //to get the codec for RFC2047Q you must pass a "q"
      codec = KMime::Codec::codecForName( "q" );
    }
    else
    {
      //it is not a rfc2047 encoded string
      return text;
    }
  }
  
  
  if( codec == NULL )
  {
    kdDebug() << "No codec available." << endl;
    return text;
  }

  
  KMime::Decoder *decoder = codec->makeDecoder();

  //look for the encoded string parts
  //we are using the regex of the codec choice
  //the loop ends when the whole string is scaned
  bool breakIt = false;
  QString decodedText;
  QString encodedText( text );

  while( !breakIt && !encodedText.isEmpty() )
  {
    //look for a encoded string part
    regex.setMinimal( true );
    int index = regex.indexIn( encodedText );
    if( index == -1 )
    {
      //nothing found
      breakIt = true;

      //append the remained text to the decoded string
      decodedText.append( encodedText );
    }
    else
    {
      //there is a encoded part
      //append the part to the found index to the decoded text and
      //remove it from the encoded text
      decodedText.append( encodedText.left( index ) );
      encodedText = encodedText.remove( 0, index );

      //get the encoded part and remove it from the source string
      QString encodedPart( encodedText.left( regex.matchedLength() ) );
      encodedText = encodedText.remove( 0, regex.matchedLength() );

      //get the indexes of the question marks
      int firstQuestionMark = encodedPart.indexOf( "?" );
      int secondQuestionMark = encodedPart.indexOf( "?", firstQuestionMark + 1);
      int thirthQuestionMark = encodedPart.indexOf( "?", secondQuestionMark + 1 );
      int fourthQuestionMark = encodedPart.indexOf( "?", thirthQuestionMark + 1 );

      //extract the charset string between the first and second question mark
      QString charset = encodedPart.mid( firstQuestionMark + 1, secondQuestionMark - firstQuestionMark - 1 );
      
      //extract the text between the thirth and fourth question mark      
      encodedPart = encodedPart.mid( thirthQuestionMark + 1, fourthQuestionMark - thirthQuestionMark - 1 );

      

      //decode it

      //the KMime decoders work on Byte-Arrays
      //copy the input string into a byte array
      QByteArray decInput;
      decInput.append( encodedPart );
      QByteArray::ConstIterator iterIn = decInput.begin();

      //this is the byte array for the decoder output
      QByteArray decOutput( 256, '\0');
      QByteArray::Iterator iterOut = decOutput.begin();

      //in this we build the string to return
      QString out;

      //Lets go!
      while( !decoder->decode( iterIn, decInput.end(), iterOut, decOutput.end() ) )
      {
        if( iterOut == decOutput.end() )
        {
          out.append( decOutput );
          iterOut = decOutput.begin();
          kdDebug() << out << endl;
        }
      }
      while( !decoder->finish( iterOut, decOutput.end() ) )
      {
        if( iterOut == decOutput.end() )
        {
          out.append( decOutput );
          iterOut = decOutput.begin();
          kdDebug() << "in Finish:" << out << endl;
        }
      }
      decOutput.resize( iterOut - decOutput.begin() );
      out.append( decOutput );

      //transform it to unicode if necessary
      if( !charset.toLower().startsWith( "utf") )
      {
        QTextCodec* codec = QTextCodec::codecForName( charset.toAscii() );
        if( codec != NULL )
        {
          out = codec->toUnicode( out.toLocal8Bit() );
        }
      }

      //append the decoded part to the target string
      decodedText.append( out );
      
    }
  }


  delete decoder;

  return decodedText;
}

Account* Mail::getAccount() const
{
  return (dynamic_cast<MailList*>( parent() ))->getAccount();
}

QString Mail::getFrom() const
{
  return from;
}

QString Mail::getTo() const
{
  return to;
}

QString Mail::getSizeSuffix() const
{
  QString strSize;

  if( size >= 1024 * 1024 )
  {
    //prefix is mega
    strSize = QString( "%L1M" ).arg( ( (double)size / ( 1024 * 1024 ) ), 0, 'f', 1 );
  }
  else if( size >= 1024 )
  {
    //prefix is kilo
    strSize = QString( "%L1K" ).arg( ( (double)size / 1024 ), 0, 'f', 1 );
  }
  else
    //no prefix
    strSize = QString( "%L1" ).arg( size );

  return strSize;
}

KDateTime Mail::getDateTime() const
{
  return sendDate;
}

QString Mail::getContent() const
{
  return contentType;
}

FilterAction_Type Mail::applyHeaderFilter( HeaderFilter* filter, QString account, QString& mailbox, FilterLog* log )
{
  FilterAction_Type action = filter->check( getFrom(), getTo(), getSize(), getSubject(), getHeader(), account, mailbox );

  //if the action is MARK, the related view entry shall be marked at the next view refresh
  if( action == FActMark ) markAtViewRefresh = true;

  //if the action is DELETE, we add a entry to the log
  if( log == NULL )
    kdError() << "Mail::applyHeaderFilter: Pointer to the filter log is NULL. Can't write to log." << endl;;
  if( action == FActDelete && log != NULL )
    log->addDeletedMail( getDateTime(), getFrom(), account, getSubject() );
  if( action == FActMove && log != NULL )
    log->addMovedMail( getDateTime(), getFrom(), account, getSubject(), mailbox );


  return action;
}


