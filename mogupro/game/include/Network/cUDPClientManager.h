#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <Node/node.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    cUDPClientManager( );
public:
    template <class Ty, Packet::PacketId packetId>
    void send( Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        cPacketBuffer packetBuffer;
        packetBase->createPacket( packetBuffer );

        sendDataBufferAdd( packetBuffer );

        delete packetBase;
        packetBase = nullptr;
    }
public:
    void close( );
    void open( );
    bool isConnected( );
	 void connectMatchingServer( );
    void connect( std::string const& ipAddress );
    void connectOfflineServer( );
    void update( float delta );

    // ����������������������������������������������������������������������������
    // ���R:�c
private:
    void updateSend( );
    void updateRecv( );
private:
    void connection( );
    void ping( );
private:
    void sendDataBufferAdd( cPacketBuffer const& packetBuffer );
private:
    cUDP mSocket;
    std::vector<char> mSendDataBuffer;
    cNetworkHandle mConnectServerHandle;
    hardptr<Node::node> mRoot;

    // �T�[�o�[�Ƃ̐ڑ����ێ�����Ă���̂���ۏ؂��܂��B
    // 5�b�ȏ㉞�����Ȃ��ꍇ�͐ؒf���܂��B
    float mCloseSecond;

    // �T�[�o�[�ɐڑ��ł�������ۏ؂��܂��B
    // 5�b�ȏ㉞�����Ȃ��ꍇ�͐ؒf���܂��B
    float mConnectSecond;

	 bool mFoundMatchingServer;
	 std::function<void( std::string const& )> mOnFoundMatchingServer;
};
}