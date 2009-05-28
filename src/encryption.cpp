//
// C++ Implementation: encryption
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "encryption.h"

//used in crypt() and decrypt()
static const char scramble1 [50] = "C6FDC7A1EDFBB6FEE3DBF5BEBAEFDDF7ABC6FDC7A1EDFBB6";
static const char hexstr [17] = "0123456789ABCDEF";

int Encryption::hexbyt( const char c )
{
  if( c >= '0' && c <= '9' )
    return c - '0';
  else
    return c - 'A' + 10;
}

const QString Encryption::crypt( const KUrl& url )
{
  char result[50];
  char scramble2[50];
  QString hexresult;

  memset (result, 0, 50);
  memset (scramble2, 0, 50);
  int pos = url.pass().length () + 1;
  int free = 50 - pos;

  if( url.user().length() <= free )
  {
    strcpy( &scramble2[pos], url.user().toLatin1() );
    pos += url.user().length();
    free -= url.user().length();
  }
  else
  {
    memcpy( &scramble2[pos], url.user().toLatin1(), free );
    free = 0;
  }

  if( url.host().length() <= free )
  {
    strcpy( &scramble2[pos], url.host().toLatin1() );
    pos += url.host().length();
    free -= url.host().length();
  }
  else
  {
    memcpy( &scramble2[pos], url.host().toLatin1(), free );
    free = 0;
  }

  memcpy( result, url.pass().toLatin1(), url.pass().length() );
  for (int i = 0; i <= 31; i++)
  {
    result[i] = (char)( result[i] ^ ( scramble1[i] ^ scramble2[i] ) );
    hexresult += hexstr[ result[i] / 16 ];
    hexresult += hexstr[ result[i] % 16 ];
  }

  return hexresult;
}

const QString Encryption::decrypt( const QString& pass )
{
  char result[50];

  memset( result, 0, 50 );
  int i;
  for( i = 0; i <= 31; i++ )
  {
    result[i] = (char)hexbyt( pass[ i * 2 ].toLatin1() ) * 16 + hexbyt( pass[ i * 2 + 1 ].toLatin1() );
    result[i] = (char)( result[i] ^ scramble1[i] );
  }

  return result;
}
