#include <Network/IpHost.h>
#include <winsock2.h>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
namespace Network
{
static bool gIsHostData = false;
static char gNameHost[256], gIpHost[16], gPhysicalAddress[18];

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

	//
	// http://d.hatena.ne.jp/osyo-manga/20130415/1366026842
	// 物理アドレスの取得
	{
		std::vector<std::vector<unsigned char>> result;
		DWORD bufLen = 0;
		std::vector<unsigned char> buf;
		GetAdaptersAddresses( 0, 0, 0, 0, &bufLen );
		if ( bufLen ) {
			buf.resize( bufLen, 0 );
			IP_ADAPTER_ADDRESSES* ptr = reinterpret_cast<IP_ADAPTER_ADDRESSES*>( &buf[0] );
			DWORD err = GetAdaptersAddresses( 0, 0, 0, ptr, &bufLen );
			if ( err == NO_ERROR )
			{
				while ( ptr )
				{
					if ( ptr->PhysicalAddressLength )
					{
						std::vector<BYTE> buffer( ptr->PhysicalAddressLength );
						std::copy
						(
							ptr->PhysicalAddress,
							ptr->PhysicalAddress + ptr->PhysicalAddressLength,
							buffer.begin( )
						);
						result.push_back( std::move( buffer ) );
					}
					ptr = ptr->Next;
				}
			}
		}
		std::stringstream ss;
		for ( auto& tip : result )
		{
			for ( int i = 0; i < tip.size( ); ++i )
			{
				ss << std::setw( 2 ) << std::setfill( '0' ) << std::hex << int( tip[i] ) << ( i != ( tip.size( ) - 1 ) ) ? "-" : "";
			}
		}
		memset( gPhysicalAddress, 0, sizeof( gPhysicalAddress ) );
		strcpy( gPhysicalAddress, ss.str( ).c_str( ) );
	}
	// ~物理アドレスの取得

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
std::string getPhysicalAddressHost( )
{
	if ( !gPhysicalAddress )
	{
		if ( !setupHostData( ) )
		{
			return "";
		}
		gIsHostData = true;
	}
	return gPhysicalAddress;
}
}
