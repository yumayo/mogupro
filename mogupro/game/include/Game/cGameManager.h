#pragma once
#include <Utility/cSingletonAble.h>
#include <map>
#include <Node/node.h>
namespace Game
{
class cGameManager : public Utility::cSingletonAble<cGameManager>
{
public:
	cGameManager( );
	~cGameManager( ) = default;
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
	void addPreUpdate( State state, std::function<void( float )> method );
	void addUpdate( State state, std::function<void( float )> method );
	void next( );
	//ループの開始と終わりはまだ未実装です０、０をいれてね
	void playBgm( const std::string& name, const float gain, const float loopbegin, const float loopend );
	void stopBgm( const std::string& name );
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
};
}
