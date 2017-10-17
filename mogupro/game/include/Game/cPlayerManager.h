#pragma once
#include <Utility/cSingletonAble.h>
namespace Game
{
	namespace Player {
		class cPlayer;
	}
class cPlayerManager : public ::Utility::cSingletonAble<cPlayerManager>
{
private:
    //全プレイヤーリスト
    std::vector<std::shared_ptr<Player::cPlayer>> player;
    //自分が操作するプレイヤー
    std::shared_ptr<Player::cPlayer> active_player;

    bool mouse_on = false;
    void playerInstance();
    void playerNormalMove(const float& delta_time);
	void playerDrillMove(const float& delta_time);
	void playerMove(const float& delta_time);
public:
    void setup();
    void update( const float& delta_time );
    void draw();
};
}
