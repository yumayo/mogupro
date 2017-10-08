#pragma once
#include <Utility/cSingletonAble.h>
#include <string>
#include <array>
#include <thread>
#include <Utility/cRecursionUsableMutex.h>
#include <Network/NetworkDefines.h>
#include <boost/asio.hpp>
#include <memory>
#include <deque>
namespace Network
{
class cUDP
{
public:
    cUDP( );
    void write( std::string ipaddress, int port, size_t sendDataByteNumber, char const * sendData );
    void close( );
    void open( );
    void open( int port );
    // socketにあるデータを全て削除します。
    void clearRemoteBuffer( );
    // socketにデータがあるかどうか。
    bool emptyRemoteBuffer( );
    // 古いデータから返します。
    // その時に一緒にそのデータは削除されます。
    cPacketRaw&& popRemoteBuffer( );
private:
    void receive( );
private:
    // 受信部は別スレッドで動かします。
    bool mIsPause = false;
    std::thread mUpdateIoService;
    Utility::cRecursionUsableMutex mMutex;

    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mUdpSocket;

    // データを送って来た相手のipaddressやportが入ります。
    boost::asio::ip::udp::endpoint mRemoteEndpoint;

    // 送られてきたデータが入ります。
    // 物理層で65536以上は送れない事になっているのでいっぱいになることはありません。
    cBuffer mRemoteBuffer;
    
    // 受信してきた相手のipaddressやportを溜めておきます。
    std::deque<boost::asio::ip::udp::endpoint> mRemoteEndpoints;
    // 受信データを溜めておきます。
    std::deque<cPacketRaw> mRemoteBuffers;
};
}