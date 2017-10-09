#pragma once
#include <Network/NetworkDefines.h>
#include <Network/Packet/PacketId.h>
#include <tuple>
namespace Network
{
namespace Packet
{
#pragma pack(1)
struct PacketHeader
{
    // 命令パケットの番号が入ります。
    PacketId mPacketId = PacketId::INVALID_NUMBER;
    // 送るたびにシーケンス番号を増やしていきます。
    // 古いパケットを落としたり、パケットロスの判定などに使います。
    ubyte4 mSequenceId = 0;
    // ペイロードとして渡すサイズを格納します。
    // このクラスや、継承先のサイズも含みます。
    ubyte2 mPacketByte = 0;
};
#pragma pack()
template <class Ty, PacketId packetId>
class cPacketBase
{
public:
    virtual ~cPacketBase( ) {}
    void onReceive( cPacketRaw const& raw )
    {
        ubyte2 const& packetByte = std::get<0>( raw );
        cBuffer const& buffer = std::get<1>( raw );

        // 先にヘッダーを読み込む。
        // 子供に破壊されたら最も後もないので。
        memcpy( &mHeader, buffer.data( ), sizeof( PacketHeader ) );

        // 次に子供にやってもらう。
        packetImport( packetByte - sizeof( PacketHeader ), buffer.data( ) + sizeof( PacketHeader ) );
    }
    cPacketRaw&& createPacket( )
    {
        cPacketRaw raw;
        ubyte2& packetByte = std::get<0>( raw );
        cBuffer& buffer = std::get<1>( raw );

        // 子供のパケットを先に詰めてもらう。
        // 子供にヘッダーパケットを壊されて謎のエラーが出ても嫌なので。
        char* childHeader = buffer.data( ) + sizeof( PacketHeader );
        ubyte2 childPacketByte = packetExport( childHeader );

        // ヘッダーパケットを追加する。
        PacketHeader header;
        header.mPacketId = packetId;
        header.mSequenceId = ++sequenceCount;
        header.mPacketByte = childPacketByte + sizeof( PacketHeader );
        memcpy( buffer.data( ), &header, sizeof( PacketHeader ) );

        // 最後にrawデータのサイズを持たせる。
        // これは65535分のいくつ使ったのかという情報がないと送る時にわからなくなるから。
        packetByte = header.mPacketByte;

        return std::move( raw );
    }
    //!@ LookMe : 継承先で受信したパケットをもとに構成し直してください。
    // ヘッダーデータは含まれません。
    // packetByteもヘッダー分は含まれません。
    // ※この関数が呼ばれた後すぐに受信パケットが削除されます。
    // ポインタを保存とかわけのわからんことをしないように。
    // ちゃんとコピーを取ってください。
    virtual void packetImport( ubyte2 packetByte, char const* const packet ) = 0;
    //!@ LookMe : 継承先で送信するパケットを構成してください。
    // ヘッダーデータは含まなくて大丈夫です。
    // 返り値は継承先の送りたいパケットサイズ。
    virtual ubyte2 packetExport( char* const packet ) = 0;
    ubyte4 getSequenceId( )
    {
        return mHeader.mSequenceId;
    }
    PacketId getPacketId( )
    {
        return mHeader.mPacketId;
    }
    ubyte2 getPacketByte( )
    {
        return mHeader.mPacketByte;
    }
protected:
    // シーケンス番号を保存します。
    // 継承先のクラスごとに生成されます。
    static ubyte4 sequenceCount;

    PacketHeader mHeader;
};
template <class Ty, PacketId packetId>
ubyte4 cPacketBase<Ty, packetId>::sequenceCount = 0;
}
}