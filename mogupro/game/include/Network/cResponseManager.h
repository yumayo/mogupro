#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <map>
#include <Network/Packet/Response.hpp>
#include <boost/optional.hpp>
namespace Network
{
class cResponseManager : public Utility::cSingletonAble<cResponseManager>
{
    // P=====BEGIN=====P
public:
    boost::optional<Packet::Response::cResString> getResString( );
    void ungetResString( Packet::Response::cResString&& data );
    bool isNewResString( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResString> mResString;
    std::map<ubyte4, double> mResStringSequenceIds;
public:
    boost::optional<Packet::Response::cResConnect> getResConnect( );
    void ungetResConnect( Packet::Response::cResConnect&& data );
    bool isNewResConnect( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResConnect> mResConnect;
    std::map<ubyte4, double> mResConnectSequenceIds;
public:
    boost::optional<Packet::Response::cResGetJemSeed> getResGetJemSeed( );
    void ungetResGetJemSeed( Packet::Response::cResGetJemSeed&& data );
    bool isNewResGetJemSeed( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResGetJemSeed> mResGetJemSeed;
    std::map<ubyte4, double> mResGetJemSeedSequenceIds;
public:
    boost::optional<Packet::Response::cResGetJemPoint> getResGetJemPoint( );
    void ungetResGetJemPoint( Packet::Response::cResGetJemPoint&& data );
    bool isNewResGetJemPoint( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResGetJemPoint> mResGetJemPoint;
    std::map<ubyte4, double> mResGetJemPointSequenceIds;
public:
    boost::optional<Packet::Response::cResMakeRoom> getResMakeRoom( );
    void ungetResMakeRoom( Packet::Response::cResMakeRoom&& data );
    bool isNewResMakeRoom( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResMakeRoom> mResMakeRoom;
    std::map<ubyte4, double> mResMakeRoomSequenceIds;
public:
    boost::optional<Packet::Response::cResInRoom> getResInRoom( );
    void ungetResInRoom( Packet::Response::cResInRoom&& data );
    bool isNewResInRoom( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResInRoom> mResInRoom;
    std::map<ubyte4, double> mResInRoomSequenceIds;
public:
    boost::optional<Packet::Response::cResWantTeamIn> getResWantTeamIn( );
    void ungetResWantTeamIn( Packet::Response::cResWantTeamIn&& data );
    bool isNewResWantTeamIn( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResWantTeamIn> mResWantTeamIn;
    std::map<ubyte4, double> mResWantTeamInSequenceIds;
public:
    boost::optional<Packet::Response::cResCheckBeginGame> getResCheckBeginGame( );
    void ungetResCheckBeginGame( Packet::Response::cResCheckBeginGame&& data );
    bool isNewResCheckBeginGame( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResCheckBeginGame> mResCheckBeginGame;
    std::map<ubyte4, double> mResCheckBeginGameSequenceIds;
public:
    boost::optional<Packet::Response::cResCheckMember> getResCheckMember( );
    void ungetResCheckMember( Packet::Response::cResCheckMember&& data );
    bool isNewResCheckMember( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResCheckMember> mResCheckMember;
    std::map<ubyte4, double> mResCheckMemberSequenceIds;
public:
    boost::optional<Packet::Response::cResEndGamemainSetup> getResEndGamemainSetup( );
    void ungetResEndGamemainSetup( Packet::Response::cResEndGamemainSetup&& data );
    bool isNewResEndGamemainSetup( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResEndGamemainSetup> mResEndGamemainSetup;
    std::map<ubyte4, double> mResEndGamemainSetupSequenceIds;
public:
    boost::optional<Packet::Response::cResSetGamestartTimer> getResSetGamestartTimer( );
    void ungetResSetGamestartTimer( Packet::Response::cResSetGamestartTimer&& data );
    bool isNewResSetGamestartTimer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResSetGamestartTimer> mResSetGamestartTimer;
    std::map<ubyte4, double> mResSetGamestartTimerSequenceIds;
public:
    boost::optional<Packet::Response::cResCannonPower> getResCannonPower( );
    void ungetResCannonPower( Packet::Response::cResCannonPower&& data );
    bool isNewResCannonPower( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResCannonPower> mResCannonPower;
    std::map<ubyte4, double> mResCannonPowerSequenceIds;
public:
    boost::optional<Packet::Response::cResResult> getResResult( );
    void ungetResResult( Packet::Response::cResResult&& data );
    bool isNewResResult( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Response::cResResult> mResResult;
    std::map<ubyte4, double> mResResultSequenceIds;
    // P=====END=====P
};
}
