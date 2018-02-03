#pragma once
#include <Utility/cSingletonAble.h>
#include <map>
#include <Node/node.h>
#include <dessUI/UIPlate/TutorialUI/TutorialUI.h>
namespace Game
{
class cTutorialManager : public Utility::cSingletonAble<cTutorialManager>
{
public:
	cTutorialManager( );
	~cTutorialManager( ) = default;
public:
	enum class State
	{
		//説明
		Description,
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
	void draw2D();
	bool getTutorialStan() { return ui.getTutorialStan(); }
private:
	void skipReady( );
	void addPreUpdate( State state, std::function<void( float )> method );
	void addUpdate( State state, std::function<void( float )> method );
	void next( );
	//ループの開始と終わりはまだ未実装です０、０をいれてね
	void playBgm( const std::string& name, const float gain, const float loopbegin, const float loopend );
	void stopBgm( const std::string& name );
	
	TutorialUI ui;
	// 現在のステート
	State state = State::Description;
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
