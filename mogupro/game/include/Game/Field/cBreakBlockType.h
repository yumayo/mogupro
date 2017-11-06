#pragma once
#include <Network/NetworkDefines.h>
#include <set>
namespace Game
{
namespace Field
{

enum class BlockType
{
    AIR = 1 << 0,
    NORMAL = 1 << 1,
    HARD = 1 << 2,
    UNBREAKING = 1 << 3,
};

//* Žg‚¢•û *//
// 
// Game::Field::cBreakBlockType mBreakType; 
// 
// mBreakType.add( Game::Field::BlockType::NORMAL );
// mBreakType.add( Game::Field::BlockType::HARD );
// 
// Game::Field::cFieldManager->getInstance()->blockBreak( mPos, 1, mBreakType );

class cBreakBlockType
{
public:

    cBreakBlockType( const Network::ubyte1& type );
    cBreakBlockType( const BlockType& type );
    cBreakBlockType();
    ~cBreakBlockType();

    void add( const BlockType& type );
    bool find( const BlockType& type ) const;
    void erase( const BlockType& type );

    std::set<BlockType> getBreakType()const { return mType; }
    void setBreakType( const std::set<BlockType>& types ) { mType = types; }

    operator Network::ubyte1() const noexcept
    {
        Network::ubyte1 temp = 0;
        for ( auto& it : mType )
            temp |= static_cast<Network::ubyte1>( it );
        return temp;
    }

private:
    std::set<BlockType> mType;
};
}
}
