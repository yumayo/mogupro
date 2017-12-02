#pragma once
#include <Utility/cSingletonAble.h>
#include <string>
#include <array>
#include <thread>
#include <mutex>
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
	~cUDP( );
    void write( cNetworkHandle const& networkHandle, size_t sendDataByteNumber, char const * const sendData );
    void close( );
    void open( );
    void open( int port );
    // socketにあるデータを全て削除します。
    void clearChunk( );
    // socketにデータがあるかどうか。
    bool emptyChunk( );
    // 古いデータから返します。
    // その時に一緒にそのデータは削除されます。
    cPacketChunk popChunk( );
private:
    void receive( );

private:
    // 受信部は別スレッドで動かします。
    bool mIsPause = false;
    std::thread mThreadIoService;

    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mUdpSocket;

private:
    // 受信データのMutex。
    std::mutex mDataMutex;

    // データを送って来た相手のipaddressやportが入ります。
    boost::asio::ip::udp::endpoint mRemoteEndpoint;

    // 送られてきたデータが入ります。
    // 物理層で65536以上は送れない事になっているのでいっぱいになることはありません。
    cBuffer mRemoteBuffer;
    
    // 送られてきたデータや、送って来た相手の情報を保存しておきます。
    std::deque<cPacketChunk> mCacheEndpoints;
};
}