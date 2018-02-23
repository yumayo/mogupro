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
	~cGameManager( ) = default;
public:
	void setup();
	void setTime( float allUserloadFinishedTime );
	void update( float delta );
	void draw( );
private:
	int GEM_MAX_NUM;
	float delta = 0.0F;
	float allUserloadFinishedTime = std::numeric_limits<float>::max( );
	float battleStartTime = std::numeric_limits<float>::max( );
	hardptr<Node::node> root;
	float gameTime;
	Sound::cIntroLoopableBGM introloopBGM;
public:
	std::string getLeftBattleTime();
	float getLeftBattleTimef();
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
	std::vector<int> getRedTeamKillData( );
	std::vector<int> getRedTeamDeathData( );
	std::vector<int> getRedTeamAppendGemData( );
	std::vector<int> getBlueTeamKillData( );
	std::vector<int> getBlueTeamDeathData( );
	std::vector<int> getBlueTeamAppendGemData( );
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
