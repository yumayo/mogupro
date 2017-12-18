#pragma once
#include <Network/Packet/cPacketBase.h>
#include <queue>
#include <map>
#include <boost/optional.hpp>
#include <cinder/app/App.h>
namespace Network
{
template <class Ty>
class cPacketQueue
{
public:
	boost::optional<Ty> get( )
	{
		if ( mPacket.empty( ) )
		{
			auto it = mSequenceIds.begin( );
			while ( it != mSequenceIds.end( ) )
			{
				if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
				{
					mSequenceIds.erase( it++ );
				}
				else ++it;
			}
			return boost::none;
		}
		else
		{
			auto top = mPacket.front( );
			mPacket.pop( );
			return top;
		}
	}
	void unget( Ty&& data )
	{
		mPacket.push( std::move( data ) );
	}
	bool isNew( Packet::PacketHeader const& header )
	{
		if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
		auto status = mSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
		status.first->second = cinder::app::getElapsedSeconds( );
		return status.second;
	}
private:
	std::queue<Ty> mPacket;
	std::map<ubyte4, double> mSequenceIds;
};
}