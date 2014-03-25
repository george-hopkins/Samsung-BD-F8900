/*
  Copyright (c) 2005-2008 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifdef _WIN32
# include "../config.h.win"
#elif defined( _WIN32_WCE )
# include "../config.h.win"
#else
# include "config.h"
#endif

#include "gloox.h"
#include "dns.h"

#ifndef _WIN32_WCE
# include <sys/types.h>
# include <sstream>
#endif

#include <stdio.h>

#if !defined( _WIN32 ) && !defined( _WIN32_WCE )
# include <netinet/in.h>
# include <arpa/nameser.h>
# include <resolv.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <unistd.h>
#endif

#ifdef _WIN32
# include <winsock.h>
#elif defined( _WIN32_WCE )
# include <winsock2.h>
#endif

#ifdef HAVE_WINDNS_H
# include <windns.h>
#endif

#define SRV_COST    (RRFIXEDSZ+0)
#define SRV_WEIGHT  (RRFIXEDSZ+2)
#define SRV_PORT    (RRFIXEDSZ+4)
#define SRV_SERVER  (RRFIXEDSZ+6)
#define SRV_FIXEDSZ (RRFIXEDSZ+6)

#ifndef T_SRV
# define T_SRV 33
#endif

// mingw
#ifndef DNS_TYPE_SRV
# define DNS_TYPE_SRV 33
#endif

#ifndef C_IN
# define C_IN 1
#endif

#ifndef INVALID_SOCKET
# define INVALID_SOCKET -1
#endif

#define XMPP_PORT 5222

namespace gloox
{

#if defined( HAVE_RES_QUERYDOMAIN ) && defined( HAVE_DN_SKIPNAME ) && defined( HAVE_RES_QUERY )
  DNS::HostMap DNS::resolve( const std::string& service, const std::string& proto,
                             const std::string& domain, const LogSink& logInstance )
  {
    buffer srvbuf;
    bool error = false;

    const std::string dname = "_" +  service + "._" + proto;

    if( !domain.empty() )
      srvbuf.len = res_querydomain( dname.c_str(), const_cast<char*>( domain.c_str() ),
                                    C_IN, T_SRV, srvbuf.buf, NS_PACKETSZ );
    else
      srvbuf.len = res_query( dname.c_str(), C_IN, T_SRV, srvbuf.buf, NS_PACKETSZ );

    if( srvbuf.len < 0 )
      return defaultHostMap( domain, logInstance );

    HEADER* hdr = (HEADER*)srvbuf.buf;
    unsigned char* here = srvbuf.buf + NS_HFIXEDSZ;

    if( ( hdr->tc ) || ( srvbuf.len < NS_HFIXEDSZ ) )
      error = true;

    if( hdr->rcode >= 1 && hdr->rcode <= 5 )
      error = true;

    if( ntohs( hdr->ancount ) == 0 )
      error = true;

    if( ntohs( hdr->ancount ) > NS_PACKETSZ )
      error = true;

    int cnt;
    for( cnt = ntohs( hdr->qdcount ); cnt>0; --cnt )
    {
      int strlen = dn_skipname( here, srvbuf.buf + srvbuf.len );
      here += strlen + NS_QFIXEDSZ;
    }

    unsigned char *srv[NS_PACKETSZ];
    int srvnum = 0;
    for( cnt = ntohs( hdr->ancount ); cnt>0; --cnt )
    {
      int strlen = dn_skipname( here, srvbuf.buf + srvbuf.len );
      here += strlen;
      srv[srvnum++] = here;
      here += SRV_FIXEDSZ;
      here += dn_skipname( here, srvbuf.buf + srvbuf.len );
    }

    if( error )
    {
      return defaultHostMap( domain, logInstance );
    }

    // (q)sort here

    HostMap servers;
    for( cnt=0; cnt<srvnum; ++cnt )
    {
      name srvname;

      if( ns_name_ntop( srv[cnt] + SRV_SERVER, (char*)srvname, NS_MAXDNAME ) < 0 )
        printf( "handle this error!\n" );

      unsigned char* c = srv[cnt] + SRV_PORT;

      servers.insert( std::make_pair( (char*)srvname, ntohs( c[1] << 8 | c[0] ) ) );
    }

    return servers;
  }

#elif defined( _WIN32 ) && defined( HAVE_WINDNS_H )
  DNS::HostMap DNS::resolve( const std::string& service, const std::string& proto,
                             const std::string& domain, const LogSink& logInstance )
  {
    const std::string dname = "_" +  service + "._" + proto + "." + domain;
    bool error = false;

    DNS::HostMap servers;
    DNS_RECORD *pRecord;
    if( DnsQuery( dname.c_str(), DNS_TYPE_SRV, DNS_QUERY_STANDARD, NULL, &pRecord, NULL ) == ERROR_SUCCESS )
    {
      DNS_RECORD *pRec = pRecord;
      do
      {
        if( pRec->wType == DNS_TYPE_SRV )
        {
          servers[pRec->Data.SRV.pNameTarget] = pRec->Data.SRV.wPort;
        }
        pRec = pRec->pNext;
      }
      while( pRec != NULL );
      DnsRecordListFree( pRecord, DnsFreeRecordList );
    }
    else
      error = true;

    if( error || !servers.size() )
    {
      servers = defaultHostMap( domain, logInstance );
    }

    return servers;
  }

#else
  DNS::HostMap DNS::resolve( const std::string& /*service*/, const std::string& /*proto*/,
                             const std::string& domain, const LogSink& logInstance )
  {
    logInstance.log( LogLevelWarning, LogAreaClassDns,
                    "notice: gloox does not support SRV records on this platform. Using A records instead." );
    return defaultHostMap( domain, logInstance );
  }
#endif

  DNS::HostMap DNS::defaultHostMap( const std::string& domain, const LogSink& logInstance )
  {
    HostMap server;

    logInstance.log( LogLevelWarning, LogAreaClassDns, "notice: no SRV record found for " + domain
                                                       + ", using default port." );

    if( !domain.empty() )
      server[domain] = XMPP_PORT;

    return server;
  }

  int DNS::connect( const std::string& domain, const LogSink& logInstance )
  {
    HostMap hosts = resolve( domain, logInstance );
    if( hosts.size() == 0 )
      return -ConnDnsError;

    HostMap::const_iterator it = hosts.begin();
    for( ; it != hosts.end(); ++it )
    {
      int fd = DNS::connect( (*it).first, (*it).second, logInstance );
      if( fd >= 0 )
        return fd;
    }

    return -ConnConnectionRefused;
  }

  int DNS::getSocket()
  {
#ifdef _WIN32
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 )
      return -ConnDnsError;
#endif

#if 0
    struct protoent* prot;
    if( ( prot = getprotobyname( "tcp" ) ) == 0 )
    {
      cleanup();
      return -ConnDnsError;
    }
#endif
    int fd;
    //if( ( fd = socket( PF_INET, SOCK_STREAM, prot->p_proto ) ) == INVALID_SOCKET )
    if( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
    {
      cleanup();
      return -ConnConnectionRefused;
    }

#ifdef HAVE_SETSOCKOPT
    int val = 1;
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof( val ) );
#endif

    return fd;
  }

  int DNS::connect( const std::string& domain, unsigned short port, const LogSink& logInstance )
  {
    int fd = getSocket();
    if( fd < 0 )
      return fd;
#if 0
    struct hostent *h;
    if( ( h = gethostbyname( domain.c_str() ) ) == 0 )
    {
      cleanup();
      return -ConnDnsError;
    }
#endif

    struct hostent hostinfo,*h;
	char buf[1024];
	int ret;
    if( gethostbyname_r( domain.c_str(), &hostinfo, buf, sizeof(buf), &h, &ret ) )
    {
      cleanup();
      return -ConnDnsError;
    }

	if(h==NULL)
	{
		cleanup();
		return -ConnDnsError;
	}

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons( port );

    if( h->h_length != sizeof( struct in_addr ) )
    {
      cleanup();
      return -ConnDnsError;
    }
    else
    {
      memcpy( &target.sin_addr, h->h_addr, sizeof( struct in_addr ) );
    }

#ifndef _WIN32_WCE
    std::ostringstream oss;
#endif

    memset( target.sin_zero, '\0', 8 );

#if 0
    if( ::connect( fd, (struct sockaddr *)&target, sizeof( struct sockaddr ) ) == 0 )
    {
#ifndef _WIN32_WCE
      oss << "connecting to " << domain.c_str()
          << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ")";
      logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );
#endif
      return fd;
    }

#ifndef _WIN32_WCE
    oss << "connection to " << domain.c_str()
         << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ") failed";
    logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );
#endif

    closeSocket( fd );
    return -ConnConnectionRefused;
#endif

	// Turn the socket as non blocking so we can timeout on the connection
	if( fcntl( fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK ) == -1 )
	{
		oss << "can't put socket in non-blocking mode" << "[" << strerror(errno) << "]";
		logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );

		closeSocket( fd );
		return -ConnIoError;
	}

	if( ::connect( fd, (struct sockaddr *)&target, sizeof( struct sockaddr ) ) == -1 )
	{
	  if( errno!=EINPROGRESS ) 
	  {
		  oss << "connection to " << domain.c_str()
			   << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ") failed";
		  logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );
		  
		  closeSocket( fd );
		  return -ConnConnectionRefused;
	  }
	}

	fd_set set; 
	struct timeval tv;	  
	FD_ZERO( &set );	  
	FD_SET( fd, &set ); 
	tv.tv_sec = 5;  
	tv.tv_usec = 0;

	if( !select(fd+1, NULL, &set, NULL, &tv) )
	{
		oss << "connection to " << domain.c_str()
			 << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ") timeout";
		logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );
	
		closeSocket( fd );
		return -ConnConnectionRefused;
	}

	// Turn back the socket as blocking
	if( fcntl( fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK ) == -1 )
	{
		oss << "can't put socket back to blocking mode" << "[" << strerror(errno) << "]";
		logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );

		closeSocket( fd );
		return -ConnIoError;
	}	

	// Check if there were any error
	int err;
	socklen_t len = sizeof(socklen_t);
	ret = getsockopt(fd,SOL_SOCKET,SO_ERROR,(char *)&err,&len);
	if(ret < 0)
	{
		closeSocket( fd );
		return -ConnIoError;
	}
	if(err > 0) 
	{
		closeSocket( fd );
		return -ConnIoError;
	}	

	oss << "connecting to " << domain.c_str()
		<< " (" << inet_ntoa( target.sin_addr ) << ":" << port << ")";
	logInstance.log( LogLevelDebug, LogAreaClassDns, oss.str() );

	return fd;

  }

  void DNS::closeSocket( int fd )
  {
#ifndef _WIN32
    close( fd );
#else
    closesocket( fd );
#endif
  }

  void DNS::cleanup()
  {
#ifdef _WIN32
    WSACleanup();
#endif
  }

}
