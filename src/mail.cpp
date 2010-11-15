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

#include "./mail.h"

Mail::Mail( long number, const QString& unid, bool isNew, QPointer<Account> account, QObject* parent )
	: QObject( parent )
{
	this->unid = unid;
  this->acc = account;
  accountName = acc->getName();
  setNumber( number );
  setNew( isNew );

	init();

}

Mail::Mail( const Mail& mail ): QObject ( NULL ),
  unid( mail.unid ), subject( mail.subject ), header( mail.header ), size( mail.size ), number( mail.number ), _new( mail._new ),
  from( mail.from ), to( mail.to ), sendDate( mail.sendDate ), contentType( mail.contentType ), markedByFilter( mail.markedByFilter ),
  acc( mail.acc ), accountName( mail.accountName ), filterName( mail.filterName )
{
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
	size = 0;
  markedByFilter = false;
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

QString Mail::scanBodyPart( const QStringList& part, const QString& item ) const {

  //behind the keyword there is a : and a blank
  QString _item( item );
  _item.append( ": " );

  //searching for the keyword
  QListIterator<QString> it( part );
  while( it.hasNext() ) {

    QString line = it.next();
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
  //header information can be encoded with RFC2047B or RFC2047Q
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
        }
      }
      while( !decoder->finish( iterOut, decOutput.end() ) )
      {
        if( iterOut == decOutput.end() )
        {
          out.append( decOutput );
          iterOut = decOutput.begin();
        }
      }
      decOutput.resize( iterOut - decOutput.begin() );
      out.append( decOutput );

      //append the decoded part to the target string
      decodedText.append( out );

    }
  }


  delete decoder;

  return decodedText;
}

QPointer<Account> Mail::getAccount() const
{
  return acc;
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
  FilterAction_Type action = filter->check( getFrom(), getTo(), getSize(), getSubject(), getHeader(), account, mailbox, filterName );

  //if the action is MARK, the related view entry shall be marked at the next view refresh
  if( action == FActMark ) markedByFilter = true;

  //if the action is DELETE, we add an entry to the log
  if( log == NULL )
    kdError() << "Mail::applyHeaderFilter: Pointer to the filter log is NULL. Can't write to log." << endl;;
  if( action == FActDelete && log != NULL )
    log->addDeletedMail( getDateTime(), getFrom(), account, getSubject() , DelFilter, filterName );
  if( action == FActMove && log != NULL )
    log->addMovedMail( getDateTime(), getFrom(), account, getSubject(), mailbox );


  return action;
}

void Mail::save( QDomDocument& doc, QDomElement& parent )
{
  //build item tag of this mail( with mail number)
  QString hdr = QString( ITEM_MESSAGE );
  hdr.append( "%1" );
  hdr = hdr.arg( getNumber() );

  //create a new element and store the mail meta data in it
  QDomElement elem = doc.createElement( hdr );
  elem.setAttribute( QString( ATTRIBUTE_MAIL_NUMBER ), getNumber() );
  elem.setAttribute( QString( ATTRIBUTE_MAIL_SIZE ), (qlonglong)getSize() );
  elem.setAttribute( QString( ATTRIBUTE_MAIL_UID ), getUNID() );

  //create a sub element for the mail header in store the header in it
  QDomElement subelem = doc.createElement( ITEM_MAIL_HEADER );
  subelem.appendChild( doc.createTextNode( getHeader().join( HEADER_SEPARATOR ) ) );

  //add header element to the mail element
  elem.appendChild( subelem );

  //add mail element to the account (parent) element
  parent.appendChild( elem );
}

bool Mail::isMarkedByFilter() const
{
  return markedByFilter;
}

QStringList Mail::decodeMailBody( const QStringList& body, bool preferHTML, bool& isHTML ) const
{

  QString charset;    //charset of the content
  QString encoding;   //content transfer encoding
  QStringList decodedBody;

  //only set it on true, if the returned part is HTML without a doubt
  isHTML = false;

  //get boundary that is separating the parts of a multipart message
  //if the header doesn't contain a boundary attribute, this messsage
  //has just one part
  QString boundary = getBoundary();


  //process body subject to it is a multipart messsage or not
  if( boundary.isEmpty() )
  {
    //the message has just one body part

    //get the position of the first blank line
    int posBlankLine = body.indexOf( "" );

    //truncate body; the found blank line is separating the
    //header from the message
    if( posBlankLine != -1 && !body.isEmpty() && body.size() > posBlankLine + 1 ) {
      decodedBody = body.mid( posBlankLine + 1 );
      decodedBody = trim( decodedBody );
    }


    //get charset of the message; it is behind the
    //content type attribute in the header
    charset = getCharsetFromHeader();

    //get transfer encoding type from the header
    encoding = getTransferEncodingFromHeader();

    //set HTML flag
    isHTML = getContent().contains( "html" ) && preferHTML;

  }
  else
  {
    //the message has multiple parts

    //have we got a plain text or html part?
    bool hasPlaintText = !body.filter( "text/plain", Qt::CaseInsensitive ).isEmpty();
    bool hasHTML = !body.filter( "text/html", Qt::CaseInsensitive ).isEmpty();

    //just decode the body, if a plain text or a HTML part is available
    if( hasPlaintText || hasHTML )
    {

      //get the position of the first blank line
      int posBlankLine = body.indexOf( "" );

      //truncate body; the found blank line is separating the
      //header from the message
      if( posBlankLine != -1 && !body.isEmpty() && body.size() > posBlankLine + 1 ) {
        decodedBody = trim( body.mid( posBlankLine + 1 ) );
      }

      if( decodedBody.isEmpty() ) return body;

      //split the body into its parts
      QList<QStringList> bodyParts;
      QListIterator<QString> itBody( decodedBody );

      //in the body the boundary has "--" in front
      boundary = "--" + boundary;

      //the first line must be the boundary
      //QString line = itBody.next();
      //if( line != boundary ) return body;

      //search for the first boundary
      bool boundFound = false;
      while( !boundFound && itBody.hasNext() ) {
        QString line = itBody.next();
        boundFound = line.startsWith( boundary );
      }

      QStringList part; //contains the current part

      while( itBody.hasNext() ) {

        //get next line
        QString line = itBody.next();

        if( !line.startsWith( boundary ) ) {

          //it is not a boundary
          part.append( line );

        } else {

          //it is a boundary
          //append the part to the part list and create a new (empty) part
          bodyParts.append( trim( part ) );
          part = QStringList();
        }

      }

      if( bodyParts.isEmpty() ) return body;

      //do we want to take the HTML part?
      bool takeHTML = ( hasHTML && preferHTML ) || !hasPlaintText;

      //search for the desired part
      QString contentTypeString;
      if( takeHTML ) {
        contentTypeString = QString( "text/html" );
      } else {
        contentTypeString = QString( "text/plain" );
      }

      QListIterator<QStringList> itParts( bodyParts );
      bool found = false;
      while( itParts.hasNext() && !found ) {

        QStringList part = itParts.next();
        if( !part.filter( contentTypeString, Qt::CaseInsensitive ).isEmpty() ) {

          //found!!
          decodedBody = part;
          found = true;

          //it is really HTML?
          if( takeHTML ) isHTML = true;
        }
      }

      //nothing found?
      if( !found ) return body;

      //extract charset and transfer encoding
      charset = getCharset( decodedBody );
      encoding = getTransferEncoding( decodedBody );


      //we just need the text after the first blank line
      posBlankLine = decodedBody.indexOf( "" );
      if( posBlankLine != -1 && !decodedBody.isEmpty() && decodedBody.size() > posBlankLine + 1 ) {
        decodedBody = trim( decodedBody.mid( posBlankLine + 1 ) );
      }

      if( decodedBody.isEmpty() ) return body;


    } else {
      //No plain text or html found
      return body;
    }
  }

  //Good things come to those who wait. We have extract the message.
  //Now we have to decode the message, if it is encoded.

  //join the body
  QString joinedBody = decodedBody.join( "\n" );


  if( !encoding.isEmpty() ) {

    //get the codec
    KMime::Codec* codec = KMime::Codec::codecForName( encoding.toAscii() );

    if( codec != NULL ) {

      //create decoder
      KMime::Decoder *decoder = codec->makeDecoder();

      //the KMime decoders work on Byte-Arrays
      //copy the input string into a byte array
      QByteArray decInput;
      decInput.append( joinedBody );
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
        }
      }
      while( !decoder->finish( iterOut, decOutput.end() ) )
      {
        if( iterOut == decOutput.end() )
        {
          out.append( decOutput );
          iterOut = decOutput.begin();
        }
      }
      decOutput.resize( iterOut - decOutput.begin() );
      out.append( decOutput );


      joinedBody = out;
    }


  }

  //apply charset
  if( !charset.isEmpty() ) {

    //get codec
    QTextCodec* codec = QTextCodec::codecForName( charset.toAscii() );

    if( codec != NULL )
    {
      joinedBody = codec->toUnicode( joinedBody.toAscii() );
    }

  }

  //split the body to a string list
  decodedBody = joinedBody.split( '\n' );


  return decodedBody;
}

QString Mail::getBoundary( ) const
{
  const QString TAG( "boundary=" );

  QString boundary;

  //check, whether it is a multipart message
  if( contentType.contains( "multipart", Qt::CaseInsensitive ) )
  {
    //it is a multipart message

    //get the line with "boundary="
    QStringList boundaries = header.filter( TAG, Qt::CaseInsensitive );
    if( boundaries.isEmpty() ) return boundary;
    QString boundLine = boundaries.first();

    //get the position of the first occurrence of "boundary="
    int posBoundary = boundLine.indexOf( TAG, 0, Qt::CaseInsensitive );

    //continue, if a boundary attribute was found
    if( posBoundary >= 0 )
    {
      //calculate position of the first quote
      int posFirstQuote = posBoundary + TAG.length();

      //get the position of closing quote
      int posSecondQuote = boundLine.indexOf( '"', posFirstQuote + 1 );

      //get boundary string
      boundary.append( boundLine.mid( posFirstQuote + 1, posSecondQuote - posFirstQuote - 1 ) );
    }
  }

  return boundary;
}

QString Mail::getCharsetFromHeader( ) const
{
  return getCharset( header );
}

QString Mail::getCharset( const QStringList& text ) const {

  QString charset;
  const QString TAG( "charset=" );

  //remove all spaces from the text
  QStringList textWithoutSpaces;
  QStringListIterator it( text );
  while( it.hasNext() ) {
    QString line = it.next();
    textWithoutSpaces.append( line.remove( ' ' ) );
  }

  //get the line with "charset="
  QStringList charsets = textWithoutSpaces.filter( TAG, Qt::CaseInsensitive );
  if( charsets.isEmpty() ) return charset;
  QString charsetline = charsets.first();



  //get the position of the first occurrence of "charset="
  int posCharset = charsetline.indexOf( TAG, 0, Qt::CaseInsensitive );

  //continue, if a charset attribute was found
  if( posCharset != -1 )
  {
    //if the charset quoted?
    int posFirstQuote = charsetline.indexOf( '"', 0, Qt::CaseInsensitive );
    if( posFirstQuote != -1 && posFirstQuote == posCharset + TAG.length() )
    {
      //yes it is quoted

      //get the position of closing quote
      int posLastQuote = charsetline.indexOf( '"', posFirstQuote + 1, Qt::CaseInsensitive );

      if( posLastQuote != - 1 )
      {
        //get the string
        charset = charsetline.mid( posFirstQuote + 1, posLastQuote - posFirstQuote - 1 );
      }
    }
    else
    {
      //if the charset is not quoted
      //get the start position of the charset
      int start = posCharset + TAG.length();

      //get all chars after this start position
      charset = charsetline.mid( start ).trimmed();

      //remove all after a ; or "
      int posEnd = charset.indexOf( ';' );
      if( posEnd != -1 ) {
        charset = charset.left( posEnd );
      }
      posEnd = charset.indexOf( '"' );
      if( posEnd != -1 ) {
        charset = charset.left( posEnd );
      }
    }

  }

  return charset;


}

QString Mail::getTransferEncodingFromHeader() const {

  return scanHeader( "Content-Transfer-Encoding" );
}

QString Mail::getTransferEncoding( const QStringList& text ) const {

  const QString TAG( "Content-Transfer-Encoding" );

  return scanBodyPart( text, TAG );
}

void Mail::writeToMoveLog( FilterLog * log, QString account, QString mailbox )
{
  log->addMovedMail( getDateTime(), getFrom(), account, getSubject(), mailbox );
}

void Mail::setMarkAtNextViewRefresh( )
{
  markedByFilter = true;
}

void Mail::writeToDeleteLog( FilterLog * log, QString account, KindOfMailDeleting kindDelete )
{
  log->addDeletedMail( getDateTime(), getFrom(), account, getSubject(), kindDelete );
}

int Mail::compare( const Mail& other, MailSort_Type property)
{

  switch( property ) {

    //compare by state
    case MailSortState : {

      if( isNew() == other.isNew() ) return 0;
      else if( isNew() ) return 1;
      else return 0;

    }

    //compary by number
    case MailSortNumber : {

      if( getNumber() == other.getNumber() ) return 0;
      else if( getNumber() > other.getNumber() ) return 1;
      else return -1;
    }

    //comapre by account
    case MailSortAccount : {

      return QString::localeAwareCompare( getAccountName(), other.getAccountName() );
    }

    //compare by sender
    case MailSortFrom : {

      return QString::localeAwareCompare( getFrom(), other.getFrom() );
    }

    //compary by To
    case MailSortTo : {

      return QString::localeAwareCompare( getTo(), other.getTo() );
    }

    //compare by subject
    case MailSortSubject : {

      return QString::localeAwareCompare( getSubject(), other.getSubject() );
    }

    //compare by date
    case MailSortDate : {

      if( getDateTime() == other.getDateTime() ) return 0;
      else if( getDateTime() > other.getDateTime() ) return 1;
      else return -1;
    }

    //compare by size
    case MailSortSize : {

      if( getSize() == other.getSize() ) return 0;
      else if( getSize() > other.getSize() ) return 1;
      else return -1;
    }

    //compare by content
    case MailSortContent : {

      return QString::localeAwareCompare( getContent(), other.getContent() );
    }

    default : {
      return QString::localeAwareCompare( getAccountName(), other.getAccountName() );
    }

  }
}

QString Mail::getAccountName() const
{
  return accountName;
}

Mail& Mail::operator=( const Mail& other )
{
  unid = other.unid;

  subject = other.subject;

  header = other.header;

  size = other.size;

  number = other.number;

  _new = other._new;

  from = other.from;

  to = other.to;

  sendDate = other.sendDate;

  contentType = other.contentType;

  markedByFilter = other.markedByFilter;

  acc = other.acc;

  accountName = other.accountName;

  return ( *this );

}

QStringList Mail::trim(QStringList text) const
{
  //copy the text
  QStringList trimmed( text);

  while( trimmed.first().isEmpty() ) {
    trimmed.removeFirst();
  }

  while( trimmed.last().isEmpty() ) {
    trimmed.removeLast();
  }

  return trimmed;
}

void Mail::print(QStringList text) const
{
  QStringListIterator it( text );
  while( it.hasNext() ) {
    kdDebug() << it.next() << endl;
  }
}

QString Mail::getFilterName() const
{
  return filterName;
}

void Mail::setFilterName(QString filter)
{
  filterName = filter;
}


