#include <Network/IpHost.h>
#include <winsock2.h>
namespace Network
{
static bool gIsHostData = false;
static char gNameHost[256], gIpHost[16];
static bool setupHostData( )
{
    // https://github.com/tanaka0079/c/blob/master/winsock/get_ip_host.c
    WSADATA wsaData;
    if ( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 ) {
        return false;
    }

    int i;
    HOSTENT *lpHost;
    IN_ADDR inaddr;
    gethostname( gNameHost, ( int )sizeof( gNameHost ) );

    lpHost = gethostbyname( gNameHost );
    for ( i = 0; lpHost->h_addr_list[i]; i++ )
    {
        memcpy( &inaddr, lpHost->h_addr_list[i], 4 );
        strcpy( gIpHost, inet_ntoa( inaddr ) );
    }
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
