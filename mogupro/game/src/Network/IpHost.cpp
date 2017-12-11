#include <Network/IpHost.h>
#include <winsock2.h>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
#include <cinder/app/App.h>
namespace Network
{
static bool gIsHostData = false;
static char gNameHost[256], gIpHost[16];

static bool setupHostData( )
{
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 )
	{
		return false;
	}

	//
	// https://github.com/tanaka0079/c/blob/master/winsock/get_ip_host.c
	// ローカルIPアドレスとパソコンの名前を取得します。
	{
		HOSTENT *lpHost;
		IN_ADDR inaddr;
		gethostname( gNameHost, ( int )sizeof( gNameHost ) );

		lpHost = gethostbyname( gNameHost );
		for ( int i = 0; lpHost->h_addr_list[i]; i++ )
		{
			memcpy( &inaddr, lpHost->h_addr_list[i], 4 );
			strcpy( gIpHost, inet_ntoa( inaddr ) );
		}
	}
	// ~ローカルIPアドレスとパソコンの名前を取得します。

	WSACleanup( );

	return true;
}
std::string getNameHost( )
{
	if ( !gIsHostData )
	{
		if ( !setupHostData( ) )
		{
			return "";
		}
		gIsHostData = true;
	}
	return gNameHost;
}
std::string getLocalIpAddressHost( )
{
	if ( !gIsHostData )
	{
		if ( !setupHostData( ) )
		{
			return "";
		}
		gIsHostData = true;
	}
	return gIpHost;
}
}
