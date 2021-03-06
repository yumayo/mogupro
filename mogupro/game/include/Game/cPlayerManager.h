#pragma once
#include <Utility/cSingletonAble.h>
#include <Game/Player/cPlayer.h>
#include <Utility/cStateMachineBehaviour.h>	
namespace Game
{
	
class cPlayerManager : public ::Utility::cSingletonAble<cPlayerManager>
{
private:
    //全プレイヤーリスト
    std::vector<std::shared_ptr<Player::cPlayer>> players;
    //自分が操作するプレイヤー
    std::shared_ptr<Player::cPlayer> active_player;

    int active_player_id;
    int active_player_team_id;

	int debug_send_count = 0;
	int debug_recieve_count = 0;

	int watching_target_player_id = -1;

    bool mouse_on = false;
    void playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams, std::vector<std::string> const& names);
	ci::vec3 playerNormalMoveKey(const float& delta_time);
	ci::vec3 playerNormalMovePad(const float& delta_time);
	void playerDrillMove(const float& delta_time);
	void playerAttack(const float& delta_time);
	void playerMove(const float& delta_time);
	void padMove(const float & delta_time);
	void keyMove(const float & delta_time);
	Utility::cStateMachineBehaviour backOnTheGroundStater;
	hardptr<Node::node> backOnTheGroundNode;
	void setupBackOnTheGround();
	void updateBackOnTheGround(const float & delta_time);
	void killCamera( const float & delta_time );
	void watchingCamera(const float & delta_time);
public:

	int getDebugRecieveCount() {
		return debug_recieve_count;
	}
	int getDebugSendCount() {
		return debug_send_count;
	}

    int getActivePlayerId( )
    {
        return active_player_id;
    }
    int getActivePlayerTeamId( )
    {
        return active_player_team_id;
    }
	bool isActivePlayerWatching( )
	{
		return active_player->isWatching( );
	}
	int getWatchingTargetPlayerId( )
	{
		return watching_target_player_id;
	}
	bool isTargetWatching( )
	{
		return watching_target_player_id == -1;
	}
	void setPlayersPosition(std::vector<ci::vec3> positions);
	
	//idに応じたプレイヤーを返します
	std::shared_ptr<Player::cPlayer> getPlayer(int player_id) {
		for (int i = 0; i < players.size(); i++) {
			if (players[i]->getPlayerId() == player_id) {
				return players[i];
			}
		}

		//idが合わなかったらnull
		return nullptr;
	}

	std::vector<std::shared_ptr<Player::cPlayer>> getPlayers() {
		return players;
	}
	std::shared_ptr<Player::cPlayer> getActivePlayer() {
		return active_player;
	}
	//コリジョンの後に呼ぶアップデート
	void playerCollisionAfterUpdate(const float& delta_time);
	void receiveAddCannonPower(int playerId);
	// カメラの更新後に呼ぶアップデート
	void cameraAfterUpdate( const float& delta_time );
    void setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams, std::vector<std::string> const& names);
    void update( const float& delta_time );
    void draw();
};
}
