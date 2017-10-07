#pragma once
#include <Utility/cSingletonAble.h>
#include <string>
#include <thread>
#include <Utility/cRecursionUsableMutex.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <memory>
namespace Network
{
class cUDP
{
public:
    cUDP( );
    cUDP( int port );
    void write( std::string ipaddress, int port, char const* sendData, size_t sendDataByteNumber );
    void close( );
    void open( );
    void open( int port );
private:
    void receive( );
private:
    std::thread mUpdateIoService;
    bool mIsPause = false;
    Utility::cRecursionUsableMutex mMutex;

    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mUdpSocket;
    boost::asio::ip::udp::endpoint mRemoteEndpoint;
    boost::array<char, 65536> mRemoteBuffer;

    std::map<boost::asio::ip::udp::endpoint, std::vector<boost::array<char, 65536>>> mReceiveBuffers;
};
}