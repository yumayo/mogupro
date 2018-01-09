#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Strategy/cDrill.h>
#include <Game/cFieldManager.h>
#include <Game/cClientAdapter.h>
#include <Game/cGemManager.h>
#include <Node/renderer.hpp>
#include <Game/cGameManager.h>
void Game::cPlayerManager::playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	//生成
	for (int i = 0; i < player_number; i++) {
		//通信で代入
		if (i == active_player_id) {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, true, 0, 0, static_cast<Game::Player::Team>(teams[i])));
			//アクティブユーザに代入
			active_player = players[i];
			this->active_player_id = active_player_id;
			this->active_player_team_id = teams[i];
		}
		else {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, false, 0, 0, static_cast<Game::Player::Team>(teams[i])));
		}
	}
}

void Game::cPlayerManager::playerDrillMove(const float & delta_time)
{
	//CAMERA->shakeCamera(0.1f, 0.1f);
	//カメラの方向に移動
	active_player->move(CAMERA->getCameraLook() * active_player->getDrillSpeed());
}

void Game::cPlayerManager::playerAttack(const float & delta_time)
{
	if ( active_player->isWatching( ) ) return; // yumayo
	if (active_player->isDead())return;

	//メイン攻撃 R2 or 右クリック
	if (ENV->pushKey(ci::app::MouseEvent::RIGHT_DOWN) || ENV->isPadPush(ENV->BUTTON_8))
	{
		cClientAdapter::getInstance()->sendPlayerAttack(active_player_id, 1);
	}
	if (ENV->pullKey(ci::app::MouseEvent::RIGHT_DOWN) || ENV->isPadPull(ENV->BUTTON_8))
	{
		cClientAdapter::getInstance()->sendPlayerAttack(active_player_id, 2);
	}
}

ci::vec3 Game::cPlayerManager::playerNormalMoveKey(const float& delta_time)
{
	//プレイヤーの速度
	float player_speed = active_player->getSpeed() * delta_time;

	ci::vec3 keybord_velocity = ci::vec3(0);

	float x_axis = 0;
	float z_axis = 0;
	//斜め用スタック
	int diagonal = 0;

	//リセット
	if (ENV->pressKey(ci::app::KeyEvent::KEY_r)) {
		active_player->resetPos();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		z_axis = active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		z_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		x_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		x_axis = active_player->getSpeed();
		diagonal++;
	}

	if ( active_player->isWatching( ) )
	{
		if ( ENV->pressKey( ci::app::KeyEvent::KEY_LSHIFT ) ) {
			keybord_velocity.y = -active_player->getSpeed( );
		}

		if ( ENV->pressKey( ci::app::KeyEvent::KEY_SPACE ) ) {
			keybord_velocity.y = active_player->getSpeed( );
		}
	}

	keybord_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	keybord_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));

	if (diagonal >= 2) {
		std::sqrtf(keybord_velocity.x);
		std::sqrtf(keybord_velocity.z);
	}

	if ( active_player->isWatching( ) )
		;
	else
	{
		//ジャンプはMoveの後に呼ぶ
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_SPACE ) ) {
			active_player->jump( true );
		}
	}

	return keybord_velocity;
}
ci::vec3 Game::cPlayerManager::playerNormalMovePad(const float & delta_time)
{
	ci::vec3 pad_velocity = ci::vec3(0);

	float x_axis = -50 * ENV->getPadAxis(0) * delta_time * active_player->getSpeed();
	float z_axis = -50 * ENV->getPadAxis(1) * delta_time * active_player->getSpeed();

	pad_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	pad_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));
	return pad_velocity;
}
void Game::cPlayerManager::playerMove(const float & delta_time)
{

	//カメラのマウス操作ON　OFF
	if (ENV->pushKey(ci::app::KeyEvent::KEY_ESCAPE)) {
		ENV->setMouseControl(mouse_on);
		if (mouse_on == true) {
			mouse_on = false;
		}
		else {
			mouse_on = true;
		}
	}
	if (!active_player->isDead()) {
		CAMERA->addCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));
	}
	// ターゲットがいたらプレイヤーは動けません。
	if ( watching_target_player_id != -1 ) return;
	ENV->setMouseCursorAvtive(!active_player->isDead());
	//プレイヤーが死んでいたらカメラ以外操作不能
	if (active_player->isDead())return;

	CAMERA->addCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.07f), ENV->getPadAxis(3)*(-0.07f)));
	
	//大砲にジェムを入れる
	auto cannon = cStrategyManager::getInstance()->getCannons()[static_cast<Player::Team>(active_player->getWhichTeam())];
	if (cannon->getAABB().intersects(active_player->getAABB())) {
		cannon->receivePlayerGem(active_player->getgems.size(), active_player_id);
		cGemManager::getInstance()->deleteFragmentGems(active_player->getgems);
		active_player->gem_production_end.clear();
		active_player->getgems.clear();
	}


	keyMove(delta_time);

	//パッドの時に攻撃が上書きされないように注意
	padMove(delta_time);

	//掘削中は動き方が変わる
	if (active_player->isDrilling()) {
		playerDrillMove(delta_time);
	}
	else {
		ci::vec3 buf_axis = ci::vec3(0);
		buf_axis += playerNormalMoveKey(delta_time);
		buf_axis += playerNormalMovePad(delta_time);
		active_player->move(buf_axis);
	}

}
void Game::cPlayerManager::padMove(const float & delta_time)
{
	if ( active_player->isWatching( ) )
		;
	else
	{
		//掘る L1
		if ( ENV->isPadPress( ENV->BUTTON_5 ) &&
			 Game::Field::WORLD_SIZE.y + 1 > active_player->getPos( ).y ) {
			active_player->Drilling( true );
		}

		//サブ武器 R1
		if ( ENV->isPadPush( ENV->BUTTON_7 ) ) {
			active_player->useSubWeapon.useWeapon( active_player_id );
		}

		//ジャンプ X
		if ( ENV->isPadPush( ENV->BUTTON_1 ) ) {
			active_player->jump( true );
		}
	}

	//ダッシュ □
	if (ENV->isPadPull(ENV->BUTTON_3)) {
		active_player->setDefaultSpeed();
	}
	if (ENV->isPadPress(ENV->BUTTON_3)) {
		active_player->setSpeed(10.0f);
	}
	

}
void Game::cPlayerManager::keyMove(const float & delta_time)
{
	if ( active_player->isWatching( ) )
		;
	else
	{
		//掘削中はtrue 
		if ( ENV->pressKey( ci::app::MouseEvent::LEFT_DOWN ) &&
			 Game::Field::WORLD_SIZE.y + 1 > active_player->getPos( ).y ) {
			active_player->Drilling( true );
		}
		else {
			active_player->Drilling( false );
		}
		if ( ENV->pullKey( ci::app::MouseEvent::LEFT_DOWN ) ) {
			active_player->Drilling( false );
		}
	}

	//ダッシュ
	if ( ENV->pullKey( ci::app::KeyEvent::KEY_LCTRL ) ) {
		active_player->setDefaultSpeed( );
	}
	if ( ENV->pushKey( ci::app::KeyEvent::KEY_LCTRL ) ) {
		active_player->setSpeed( 10.0f );
	}

	//G-BACK ふぅうううううはははははは
	if (ENV->pressKey(ci::app::KeyEvent::KEY_l)) {
		active_player->receiveDamage(10.0f, 5);
	}
	
	if (!active_player->isDead()) {
		if (ENV->pressKey(ci::app::KeyEvent::KEY_UP)) {
			CAMERA->addCameraAngle(ci::vec2(0, 0.05f));
		}
		if (ENV->pressKey(ci::app::KeyEvent::KEY_DOWN))
			CAMERA->addCameraAngle(ci::vec2(0, -0.05f));

		if (ENV->pressKey(ci::app::KeyEvent::KEY_RIGHT))
			CAMERA->addCameraAngle(ci::vec2(-0.05f, 0));

		if (ENV->pressKey(ci::app::KeyEvent::KEY_LEFT))
			CAMERA->addCameraAngle(ci::vec2(0.05f, 0));
	}
	
	if ( active_player->isWatching( ) )
		;
	else
	{
		///////////////////デバックでライトボムを増やす
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_h ) ) {
			active_player->useSubWeapon.addSubWeapon( Game::Weapons::SubWeapon::LIGHT_BOMB );
		}
		/////////////////アイテムを使う
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_g ) ) {
			active_player->useSubWeapon.useWeapon( active_player_id );
		}
		///////////////////
	}
}
void Game::cPlayerManager::killCamera(const float & delta_time)
{
	if (!active_player->isDead())return;
	if (active_player->getRespawnCount() > 1.5f) {
		for (auto& it : players) {
			if (active_player->getDamagedId() == it->getPlayerId()) {

				CAMERA->refPosition = it->getPos() + ci::vec3(0, 0, 0);
			}
		}
	}
}
void Game::cPlayerManager::watchingCamera( const float & delta_time )
{
	if ( !isActivePlayerWatching( ) ) return;
	if ( ENV->pushKey( ci::app::KeyEvent::KEY_1 ) )
	{
		watching_target_player_id = 0;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_2 ) )
	{
		watching_target_player_id = 1;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_3 ) )
	{
		watching_target_player_id = 2;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_4 ) )
	{
		watching_target_player_id = 4; // 3は観戦者
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_5 ) )
	{
		watching_target_player_id = 5;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_6 ) )
	{
		watching_target_player_id = 6;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_ESCAPE ) )
	{
		watching_target_player_id = -1;
	}
	// 7は観戦者
}
void Game::cPlayerManager::setPlayersPosition(std::vector<ci::vec3> positions)
{
	for (int i = 0; i < players.size(); i++) {
		ci::vec3 vec = positions[i] - players[i]->getPos();
		players[i]->move(vec);
	}
}
//プレイヤーのコリジョンスピードはコリジョンマネージャの
//後に呼ぶ
void Game::cPlayerManager::playerCollisionAfterUpdate(const float& delta_time)
{
	playerAttack(delta_time);
	for (auto& it : players) {
		it->setColliderSpeed();
		it->gemsUpdate(delta_time);
		it->weaponUpdae(delta_time);
	}
	if ( active_player->isWatching( ) )
	{
		if ( isTargetWatching( ) )
		{
			CAMERA->refPosition = active_player->getPos( );
		}
		else
		{
			CAMERA->refPosition = getPlayer( watching_target_player_id )->getPos( );
		}
	}
	else
	{
		if ( !active_player->isDead( ) ) {
			CAMERA->refPosition = active_player->getPos( ) + ci::vec3( 0, 0, 0 );
		}
	}
}
void Game::cPlayerManager::setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	playerInstance(positions, player_number, active_player_id, teams);
	//ポジションの参照とカメラのズームを設定
	for (auto& it : players) {
		it->setup();
	}

	root = Node::node::create( );
	root->set_content_size( cinder::app::getWindowSize( ) );
	root->set_scale( cinder::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );

	hintRoot = Node::node::create( );
	hintRoot->set_content_size( cinder::app::getWindowSize( ) );
	hintRoot->set_scale( cinder::vec2( 1, -1 ) );
	hintRoot->set_position( hintRoot->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );

	hintRenderBase = hintRoot->add_child( Node::node::create( ) );
	hintRenderBase->set_position( hintRoot->get_content_size( ) * ci::vec2( 0.5F, 0.8F ) );

	for ( auto& it : players )
	{
		if ( getActivePlayerTeamId( ) != it->getWhichTeam( ) ) continue;
		if ( it->isWatching( ) ) continue;

		auto l = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		ci::Frustum fru( CAMERA->getCamera( ) );
		auto aabb = it->getAABB( );
		l->set_block_visible( !fru.intersects( aabb ) );
		auto pos2D = CAMERA->getCamera( ).worldToScreen( it->getPos( ) + ci::vec3( 0, aabb.getSize( ).y / 2.0F, 0 ), cinder::app::getWindowWidth( ), cinder::app::getWindowHeight( ) );
		l->set_position( pos2D + ci::vec2( 5 ) );
		l->set_tag( it->getPlayerId( ) );
		l->set_text( u8"もぐら" + std::to_string( it->getPlayerId( ) ) );
		l->set_color( ci::ColorA( 0, 0, 0, 1 ) );

		auto instance = l->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		instance->set_text( u8"もぐら" + std::to_string( it->getPlayerId( ) ) );
		instance->set_position( ci::vec2( -5 ) );
	}
}
void Game::cPlayerManager::update(const float& delta_time)
{
	playerMove( delta_time );
	for (auto& it : players) {
		it->update(delta_time);
	}
	killCamera(delta_time);
	watchingCamera(delta_time);
	hintNearBlock( );
	hintNearGemStone( );
	hintTransportGem( );
	cClientAdapter::getInstance()->sendPlayer(active_player->getPos(), ci::vec2(active_player->getRotateX(), active_player->getRotateY()));
}

void Game::cPlayerManager::draw()
{
	for (auto& it : players) {
		it->draw();
	}
}

void Game::cPlayerManager::draw2D( )
{
	root->sort_children( [ this ] ( hardptr<Node::node>& a, hardptr<Node::node>& b )
	{
		auto depthA = CAMERA->getCamera( ).worldToEyeDepth( getPlayer( a->get_tag( ) )->getPos() );
		auto depthB = CAMERA->getCamera( ).worldToEyeDepth( getPlayer( b->get_tag( ) )->getPos( ) );
		return depthA < depthB;
	} );

	for ( auto& it : players )
	{
		if ( getActivePlayerTeamId( ) != it->getWhichTeam( ) ) continue;
		if ( it->isWatching( ) ) continue;
		auto p = root->get_child_by_tag( it->getPlayerId( ) );
		ci::Frustum fru( CAMERA->getCamera( ) );
		auto aabb = it->getAABB( );
		p->set_block_visible( !fru.intersects( aabb ) );
		auto pos2D = CAMERA->getCamera( ).worldToScreen( it->getPos( ) + ci::vec3(0, aabb.getSize().y / 2.0F, 0), cinder::app::getWindowWidth( ), cinder::app::getWindowHeight( ) );
		p->set_position( pos2D + ci::vec2( 5 ) );
	}

	root->entry_render( ci::mat4( ) );

	if ( cGameManager::getInstance( )->isInGame( ) )
	{
		hintRoot->entry_render( ci::mat4( ) );
	}
}

void Game::cPlayerManager::hintNearBlock( )
{
	if ( mHintNearBlock = cFieldManager::getInstance( )->isBreakBlock( active_player->getPos( ) + ( glm::normalize( CAMERA->getCamera( ).getViewDirection( ) ) * ci::vec3( active_player->getStatus( ).drill_speed / 3 ) ), 1 ) )
	{
		if ( !mHintNearGemStone && !mHintTransportGem )
		{
			hintRenderBase->remove_all_children( );
			auto l = hintRenderBase->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
			l->set_text( u8"左クリックで地面を掘れるぞ" );
		}
	}
}

void Game::cPlayerManager::hintNearGemStone( )
{
	static int i = 0; i++; if ( i % 5 != 0 ) return;
	bool isHit = false;
	auto const& gems = cGemManager::getInstance( )->getGemStones( );
	for ( auto& gem : gems )
	{
		auto pos = active_player->getPos( ) + glm::normalize( CAMERA->getCamera( ).getViewDirection( ) * ci::vec3( 1, 0, 1 ) ) * 3.0F;
		if ( glm::distance( pos, gem->getPos( ) ) < 2.0F )
		{
			isHit = true;
			break;
		}
	}
	if ( mHintNearGemStone = isHit )
	{
		if ( !mHintTransportGem )
		{
			hintRenderBase->remove_all_children( );
			auto l = hintRenderBase->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
			l->set_text( u8"右クリックで宝石を採れるぞ" );
		}
	}
}

void Game::cPlayerManager::hintTransportGem( )
{
	if ( mHintTransportGem && active_player->getgems.empty( ) )
	{
		hintRenderBase->remove_all_children( );
		auto l = hintRenderBase->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		l->set_text( u8"よくやった" );
		l->run_action( Node::Action::sequence::create( Node::Action::delay::create( 1.5F ), Node::Action::call_func::create( [ this ] { mHintTransportGem = false; } ),
													   Node::Action::remove_self::create( ) ) );
	}
	if ( mHintTransportGem = !active_player->getgems.empty( ) )
	{
		hintRenderBase->remove_all_children( );
		auto l = hintRenderBase->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		l->set_text( u8"宝石を大砲に持って帰ろう" );
	}
}
