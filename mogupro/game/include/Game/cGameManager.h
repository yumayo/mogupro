#pragma once
#include <Utility/cSingletonAble.h>
#include <map>
#include <Node/node.h>
#include <Sound/cIntroLoopableBGM.h>
#include <boost/optional.hpp>
namespace Game
{
class cGameManager : public Utility::cSingletonAble<cGameManager>
{
public:
	cGameManager( );
	~cGameManager( );
public:
	enum class State
	{
		//
		INIT,
		// 自分がロード中や他のプレイヤーのロードを待っている状態
		LOAD,
		// 自分のチームの確認をしている状態
		MY_TEAM,
		//　敵のチームの確認をしている状態
		ENEMY_TEAM,
		// プレイヤーにカメラが移動し、本当にバトルが開始されるまでの間
		READY,
		// ゲーム中
		BATTLE,
		// ゲームが終了しリザルトへ移行中
		BATTLE_END,
		// リザルト発表中
		RESULT,
	};
public:
	// ゲーム開始パケットが届いたらステートごとの絶対移行時間を代入してください。
	void setTime( float loadTime );
	std::string getLeftBattleTime( );
	void preUpdate( float delta );
	void update( float delta );
	void draw( );
private:
	void skipReady( );
	void skipBattle( );
	void addPreUpdate( State state, std::function<void( float )> method );
	void addUpdate( State state, std::function<void( float )> method );
	void next( );
	// 現在のステート
	State state = State::INIT;
	// 一フレーム前のステート
	State prevState = state;
	// ステートが移った瞬間
	bool flash = false;

	bool timeEmpty( );

	// ステートごとの絶対移行時間。
	// ゲームメインに移動した時点で明確に決めておきます。
	std::map<State, float> shiftSeconds;

	// 
	std::map<State, std::function<void( float )>> mUpdates;
	// 
	std::map<State, std::function<void( float )>> mPreUpdates;

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
};
}
