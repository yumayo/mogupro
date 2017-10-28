#pragma once
#include <Utility/cSingletonAble.h>
#include <Game/Player/cPlayer.h>
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

    bool mouse_on = false;
    void playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams);
    void playerNormalMove(const float& delta_time);
	void playerDrillMove(const float& delta_time);
	void playerMove(const float& delta_time);
	void padMove(const float & delta_time);
public:
    int getActivePlayerId( )
    {
        return active_player_id;
    }
    int getActivePlayerTeamId( )
    {
        return active_player_team_id;
    }
	void setPlayersPosition(std::vector<ci::vec3> positions);
	std::vector<std::shared_ptr<Player::cPlayer>> getPlayers() {
		return players;
	}
	std::shared_ptr<Player::cPlayer> getActivePlayer() {
		return active_player;
	}
    void setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams);
    void update( const float& delta_time );
    void draw();
};
}
