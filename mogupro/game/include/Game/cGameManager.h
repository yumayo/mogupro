#pragma once
#include <Utility/cSingletonAble.h>
#include <map>
#include <Node/node.h>
#include <Sound/cIntroLoopableBGM.h>
#include <boost/optional.hpp>
#include <Utility/cStateMachineBehaviour.h>
namespace Game
{
class cGameManager : public Utility::cSingletonAble<cGameManager>
{
public:
	cGameManager( );
	~cGameManager( );
public:
	void setTime( float allUserloadFinishedTime );
	std::string getLeftBattleTime( );
	void update( float delta );
	void draw( );
private:
	float delta = 0.0F;
	float allUserloadFinishedTime = std::numeric_limits<float>::max( );
	float battleStartTime = std::numeric_limits<float>::max( );
	hardptr<Node::node> root;
	Sound::cIntroLoopableBGM introloopBGM;
public:
	bool isInGame( );
	void addRedCannonPower( int value );
	void addBlueCannonPower( int value );
	int winTeam( );
	void appendItem( int type );
	void useItem( );
	void kill( int playerId );
	void death( int playerId );
	void appendGem( int playerId, int gemNum );
	std::pair<int, int> getResult( );
	std::map<int, int> redTeamKillNum( );
	std::map<int, int> redTeamDeathNum( );
	std::map<int, int> redTeamAppendGemNum( );
	std::map<int, int> blueTeamKillNum( );
	std::map<int, int> blueTeamDeathNum( );
	std::map<int, int> blueTeamAppendGemNum( );
private:
	int redCannonPower = 0;
	int blueCannonPower = 0;
	boost::optional<int> currentItem;
	boost::optional<int> nextItem;
	std::map<int, int> killNum;
	std::map<int, int> deathNum;
	std::map<int, int> appendGemNum;
	Utility::cStateMachineBehaviour sMac;
};
}
