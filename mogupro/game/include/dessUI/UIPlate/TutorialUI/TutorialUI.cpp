#include "TutorialUI.h"
#include <Utility/cInput.h>
#include <Game/cPlayerManager.h>
#include <Game/Field/FieldData.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Network/cUDPClientManager.h>
#include <Game/cFieldManager.h>
#include <Game/cCapsuleManager.h>
#include <Scene/cSceneManager.h>
#include <Resource/cSoundManager.h>
#include <Scene/Member/cTitle.h>
#include <Sound/Stereophonic.h>
#include <Game/cStrategyManager.h>
#include <Game/Weapons/MainWeapon/cFactory.h>
void TutorialUI::setup(const dess::SceneName & name)
{
	now_scene = TUTORIAL::MOVE;
	buf_scene = static_cast<int>(TUTORIAL::NONE);
	UIPlate::setup(name);
	textInit();
	tutorial_stan = true;
	ENV->enableKeyButton();
	ENV->enablePadAxis();
	ENV->enablePadButton();
	ENV->enableMouseButton();
}
//プレイヤー初期化
void TutorialUI::playerInit()
{
	Game::cPlayerManager::getInstance()->getActivePlayer()->Drilling(false);
	Game::cPlayerManager::getInstance()->getActivePlayer()->setPlayerVec(ci::vec3(0));
	Game::cPlayerManager::getInstance()->getActivePlayer()->settingPosition(ci::vec3(Game::Field::WORLD_SIZE.x / 2, Game::Field::WORLD_SIZE.y + 0.3f, 17.0F));
	Game::cPlayerManager::getInstance()->getActivePlayer()->move(ci::vec3(0, 0, 0.1f));
	CAMERA->setCameraAngle(cinder::vec2(0, -0.3f));
	Game::cFieldManager::getInstance()->blockAllReset();
	Game::cPlayerManager::getInstance()->getActivePlayer()->useSubWeapon.clearSubWeapon();
	Game::cPlayerManager::getInstance()->getActivePlayer()->update(10);
	Game::cPlayerManager::getInstance()->playerCollisionAfterUpdate(10);
}

void TutorialUI::tutorialNumbers(int num) {
	ui_data["TutorialPlate"]->setActive(true);
	ui_data["チュートリアル説明"]->setActive(true);
	for (int i = 1; i < 8; i++) {
		if (num == i) {
			ui_data[std::to_string(i) + "y"]->setActive(true);
			continue;
		}
		if (num < i) {
			ui_data[std::to_string(i) + "g"]->setActive(true);
			continue;
		}
		ui_data[std::to_string(i)]->setActive(true);
	}
}

//テキスト初期化
void TutorialUI::textInit()
{
	ui_data["操作説明"]->fontSetText(u8"操作説明");
	ui_data["移動説明初期"]->fontSetText(u8"移動");
	ui_data["移動説明"]->fontSetText(u8"左スティックを動かすと");
	ui_data["移動説明２"]->fontSetText(u8"前後左右へ移動可能です");
	ui_data["移動説明３"]->fontSetText(u8"移動してみましょう");
	
	ui_data["視点説明"]->fontSetText(u8"視点移動");
	ui_data["視点説明２"]->fontSetText(u8"右スティックを動かすと");
	ui_data["視点説明３"]->fontSetText(u8"視点を動かせます");
	ui_data["視点説明４"]->fontSetText(u8"視点を動かしてみましょう");

	ui_data["掘る説明"]->fontSetText(u8"掘って地面を移動する");
	ui_data["掘る説明２"]->fontSetText(u8"「LT」ボタンを長押しすると\n地面を掘ることができます。");
	ui_data["掘る説明３"]->fontSetText(u8"地面を掘って移動しましょう。");
	ui_data["掘る説明４"]->fontSetText(u8"左スティックで移動する\n方向を変えることができます。");

	ui_data["ジェム説明"]->fontSetText(u8"ジェムを集める");
	ui_data["ジェム説明２"]->fontSetText(u8"地面の中で光っているものを\n「ジェム」と言います\nジェムは破壊する事で取得できます。");
	ui_data["ジェム説明３"]->fontSetText(u8"壊す");
	ui_data["ジェム説明４"]->fontSetText(u8"「RB」ボタンで攻撃ができます\nジェムは地下にあるので、\n掘って近くまで行き、\n攻撃をすることで取得可能です");
	ui_data["ジェム説明５"]->fontSetText(u8"攻撃");
	ui_data["ジェム説明６"]->fontSetText(u8"地面の下にあるジェムを攻撃して回収しましょう");
	
	ui_data["攻撃説明"]->fontSetText(u8"敵を倒す");
	ui_data["攻撃説明２"]->fontSetText(u8"攻撃は敵を倒す事もできます。\n「RB」ボタンを長押しすると、\nチャージする事ができ、離すと遠くの敵も\n攻撃できる「チャージショット」になります。");
	ui_data["攻撃説明３"]->fontSetText(u8"目の前の敵を倒してください");
	
	ui_data["納品説明"]->fontSetText(u8"取得したジェムは、自分のチームの大砲に持っていきます。");
	ui_data["納品説明２"]->fontSetText(u8"大砲の近くに行くと大砲にジェムを\n入れることができます。\nジェムを入れると、画面左にある\nゲージが溜まって行きます。\nゲージを満タンにすれば勝利となります。");
	ui_data["納品説明３"]->fontSetText(u8"ジェムを大砲に入れましょう");

	ui_data["アイテム説明"]->fontSetText(u8"アイテム");
	ui_data["アイテム説明２"]->fontSetText(u8"地面にはアイテムが埋まっています。\n近づくことで取得ができます。");
	ui_data["アイテム説明３"]->fontSetText(u8"「LB」キーで使用可能です。\nどんどん使っていきましょう！");
	ui_data["アイテム説明４"]->fontSetText(u8"アイテムを取得してください。");
	ui_data["アイテム説明５"]->fontSetText(u8"アイテムを使用してください。");
	
	ui_data["最後説明"]->fontSetText(u8"このゲームは、「大砲のゲージが満タンに\nなった方が勝ち」ですので、積極的に\nジェムを集めていきましょう。");
	ui_data["最後説明２"]->fontSetText(u8"操作説明は以上になります。お疲れさまでした。");
	ui_data["次へ説明"]->fontSetText(u8"");
	ui_data["移動説明４"]->fontSetText(u8"「A」ボタンでジャンプを\nすることができます。");
	ui_data["チュートリアル説明"]->fontSetText(u8"チュートリアル");
	//文字を更新
	//かなり重いので必ず１フレームだけ呼ぶ
	for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
		if (ui_data[(*it)]->getUIType() == UITYPE::FontUI) {
			ci::TextLayout font_buf;
			font_buf.setColor(ci::ColorA(
				ui_data[(*it)]->fontGetColorR(),
				ui_data[(*it)]->fontGetColorG(),
				ui_data[(*it)]->fontGetColorB(),
				ui_data[(*it)]->fontGetColorA()));
			font_buf.setFont(font_data);
			font_buf.addLine(ui_data[(*it)]->fontGetText());

			font[(*it)] = font_buf;

			font_texture[(*it)] = ci::gl::Texture2d::create(font[(*it)].render(true));
		}
	}
}
void TutorialUI::enableNextButton() {
	ui_data["ボタンB"]->setActive(true);
	ui_data["次へ"]->setActive(true);
}
void TutorialUI::disableNextButton() {
	ui_data["ボタンB"]->setActive(false);
	ui_data["次へ"]->setActive(false);
}
void TutorialUI::tutorialMoveSetup(const float & delta_time)
{
	playerInit();
	tutorialNumbers(1);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["操作説明"]->setActive(true);
	ui_data["操作説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["移動"] = false;
	turo_flags["操作説明"] = false;
	turo_flags["移動終了"] = false;
	turo_flags["上"] = false;
	turo_flags["下"] = false;
	turo_flags["左"] = false;
	turo_flags["右"] = false;
	turo_flags["移動２個目"] = false;

	//カウントの初期化
	tuto_counts["移動説明の時間"] = 0.0f;
	tuto_counts["移動説明の時間２"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}
void TutorialUI::tutorialMoveUpdate(const float& delta_time)
{

	if (!turo_flags["操作説明"] && !ui_data["説明板"]->getActive()) {
		Sound::StereophonicManager::getInstance()->add("tutorial2", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		turo_flags["操作説明"] = true;
		ui_data["説明板初期"]->setActive(true);
		ui_data["説明板初期"]->setActive(false);
		ui_data["移動説明初期"]->setActive(true);
		ui_data["移動説明初期"]->setActive(false);
	}
	if (turo_flags["操作説明"] && !turo_flags["移動"] && !ui_data["説明板初期"]->getActive() && !turo_flags["移動２個目"]) {
		ui_data["移動板"]->setActive(true);
		ui_data["移動説明"]->setActive(true);
		ui_data["移動説明２"]->setActive(true);

		ui_data["コントローラ左"]->setActive(true);
		ui_data["スティック左"]->setActive(true);
		/*ui_data["W"]->setActive(true);
		ui_data["A"]->setActive(true);
		ui_data["S"]->setActive(true);
		ui_data["D"]->setActive(true);
		*/
		ui_data["up"]->setActive(true);
		ui_data["down"]->setActive(true);
		ui_data["left"]->setActive(true);
		ui_data["right"]->setActive(true);

		tuto_counts["移動説明の時間"] += delta_time;
	}
	if (tuto_counts["移動説明の時間"] > 3.0f && turo_flags["移動２個目"] == false) {
		enableNextButton();
		
	}

	if (tuto_counts["移動説明の時間"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["移動２個目"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["移動２個目"] = true;
		ui_data["移動説明"]->setActive(false);
		ui_data["移動説明２"]->setActive(false);
		ui_data["スティック左"]->setActive(false);
		/*ui_data["W"]->setActive(true);
		ui_data["A"]->setActive(true);
		ui_data["S"]->setActive(true);
		ui_data["D"]->setActive(true);
		*/
		ui_data["up"]->setActive(false);
		ui_data["down"]->setActive(false);
		ui_data["left"]->setActive(false);
		ui_data["right"]->setActive(false);
		

		ui_data["スティック設置"]->setActive(true);
		ui_data["移動説明４"]->setActive(true);
		ui_data["リング"]->setActive(true);
	}
	if (turo_flags["移動２個目"]) {
		tuto_counts["移動説明の時間２"] += delta_time;
	}
	if (tuto_counts["移動説明の時間２"] > 3.0f && turo_flags["移動"] == false) {
		enableNextButton();
		
	}

	if (tuto_counts["移動説明の時間２"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["移動"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["移動"] = true;

		ui_data["移動板"]->setActive(false);
	
		ui_data["コントローラ左"]->setActive(false);
		
		ui_data["スティック右"]->setActive(false);
		ui_data["移動説明４"]->setActive(false);
		ui_data["リング"]->setActive(false);
	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["移動"] == true
		&& turo_flags["移動終了"] == false) {
		ui_data["スティック設置"]->setActive(false);
		tutorial_stan = false;
		ui_data["説明板２"]->setActive(true);
		ui_data["移動説明３"]->setActive(true);
		auto player_vec = Game::cPlayerManager::getInstance()->getActivePlayer()->getPlayerVec();
		if (player_vec.z >= 0.1f) { turo_flags["上"] = true; }
		if (player_vec.z <= -0.1f) { turo_flags["下"] = true; }
		if (player_vec.x <= -0.1f) { turo_flags["左"] = true; }
		if (player_vec.x >= 0.1f) { turo_flags["右"] = true; }

		if (turo_flags["上"] &&
			turo_flags["下"] &&
			turo_flags["左"] &&
			turo_flags["右"]) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["移動終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::EYE;
		tutorial_stan = true;
	}
}
void TutorialUI::tutorialEyeSetup(const float & delta_time)
{
	playerInit();
	tutorialNumbers(2);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["視点説明"]->setActive(true);
	ui_data["視点説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["視点"] = false;
	turo_flags["視点終了"] = false;

	turo_flags["上"] = false;
	turo_flags["下"] = false;
	turo_flags["左"] = false;
	turo_flags["右"] = false;

	//カウントの初期化
	tuto_counts["視点説明"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}
void TutorialUI::tutorialEyeUpdate(const float & delta_time)
{
	if (!turo_flags["視点"] && !ui_data["説明板"]->getActive()) {

		ui_data["移動板"]->setActive(true);
		ui_data["視点説明２"]->setActive(true);
		ui_data["視点説明３"]->setActive(true);

		ui_data["コントローラ右"]->setActive(true);
		ui_data["スティック右"]->setActive(true);

		ui_data["up2"]->setActive(true);
		ui_data["down2"]->setActive(true);
		ui_data["left2"]->setActive(true);
		ui_data["right2"]->setActive(true);

		tuto_counts["視点説明"] += delta_time;
	}
	if (tuto_counts["視点説明"] > 3.0f && turo_flags["視点"] == false) {
		enableNextButton();
	}

	if (tuto_counts["視点説明"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["視点"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["視点"] = true;
		tutorial_stan = false;
		ui_data["移動板"]->setActive(false);
		ui_data["視点説明２"]->setActive(false);
		ui_data["視点説明３"]->setActive(false);

		ui_data["コントローラ右"]->setActive(false);
		ui_data["スティック右"]->setActive(false);

		ui_data["up2"]->setActive(false);
		ui_data["down2"]->setActive(false);
		ui_data["left2"]->setActive(false);
		ui_data["right2"]->setActive(false);

	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["視点"] == true
		&& turo_flags["視点終了"] == false) {
	
		ui_data["説明板２"]->setActive(true);
		ui_data["視点説明４"]->setActive(true);
		auto player_vec = CAMERA->getCameraAngle();
		if (player_vec.y >= 0.1f) { turo_flags["上"] = true; }
		if (player_vec.y <= -0.1f) { turo_flags["下"] = true; }
		if (player_vec.x <= -0.1f) { turo_flags["左"] = true; }
		if (player_vec.x >= 0.1f) { turo_flags["右"] = true; }

		if (turo_flags["上"] &&
			turo_flags["下"] &&
			turo_flags["左"] &&
			turo_flags["右"]) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["視点終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::DRILL;
		tutorial_stan = true;
	}


}

void TutorialUI::tutorialDrillSetup(const float & delta_time) {
	playerInit();
	tutorialNumbers(3);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["掘る説明"]->setActive(true);
	ui_data["掘る説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["掘る"] = false;
	turo_flags["掘る２個目"] = false;
	turo_flags["掘る終了"] = false;

	//カウントの初期化
	tuto_counts["掘る説明１個目"] = 0.0f;
	tuto_counts["掘る説明２個目"] = 0.0f;
	tuto_counts["掘った時間"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}
void TutorialUI::tutorialDrillUpdate(const float & delta_time){

	if (!turo_flags["掘る"] && !ui_data["説明板"]->getActive() && turo_flags["掘る２個目"] == false) {

		ui_data["移動板"]->setActive(true);
		ui_data["掘る説明２"]->setActive(true);

		ui_data["コントローラLR"]->setActive(true);
		ui_data["コントローラL2"]->setActive(true);

		ui_data["掘ってる画像"]->setActive(true);

		tuto_counts["掘る説明１個目"] += delta_time;
	}
	if (tuto_counts["掘る説明１個目"] > 3.0f && turo_flags["掘る２個目"] == false) {
		enableNextButton();
	}

	if (tuto_counts["掘る説明１個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["掘る２個目"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["掘る２個目"] = true;
		ui_data["掘る説明２"]->setActive(false);
		ui_data["コントローラLR"]->setActive(false);

		ui_data["コントローラL2"]->setActive(false);
		ui_data["掘ってる画像"]->setActive(false);


		ui_data["掘る説明４"]->setActive(true);
		ui_data["コントローラ左"]->setActive(true);
		ui_data["スティック左"]->setActive(true);

		ui_data["up"]->setActive(true);
		ui_data["down"]->setActive(true);
		ui_data["left"]->setActive(true);
		ui_data["right"]->setActive(true);
	}
	if (turo_flags["掘る２個目"]) {
		tuto_counts["掘る説明２個目"] += delta_time;
	}
	if (tuto_counts["掘る説明２個目"] > 3.0f && turo_flags["掘る"] == false) {
		enableNextButton();
	}
	if (tuto_counts["掘る説明２個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["掘る"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["掘る"] = true;
		tutorial_stan = false;
		ui_data["移動板"]->setActive(false);

		ui_data["掘る説明４"]->setActive(false);
		ui_data["コントローラ左"]->setActive(false);
		ui_data["スティック左"]->setActive(false);

		ui_data["up"]->setActive(false);
		ui_data["down"]->setActive(false);
		ui_data["left"]->setActive(false);
		ui_data["right"]->setActive(false);
	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["掘る"] == true
		&& turo_flags["掘る終了"] == false) {
		
		ui_data["説明板２"]->setActive(true);
		ui_data["掘る説明３"]->setActive(true);
		if (Game::cPlayerManager::getInstance()->getActivePlayer()->isDrilling()) {
			tuto_counts["掘った時間"] += delta_time;
		}

		if (tuto_counts["掘った時間"] > 6) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["掘る終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::GEM;
		tutorial_stan = true;
	}
}

void TutorialUI::tutorialGemSetup(const float & delta_time) {
	playerInit();
	tutorialNumbers(4);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["ジェム説明"]->setActive(true);
	ui_data["ジェム説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["ジェム"] = false;
	turo_flags["ジェム２個目"] = false;
	turo_flags["ジェム終了"] = false;

	//カウントの初期化
	tuto_counts["ジェム説明１個目"] = 0.0f;
	tuto_counts["ジェム説明２個目"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
	for (auto& it : Game::cPlayerManager::getInstance()->getActivePlayer()->getgems) {
		it->setPos(ci::vec3(0));
	}
	Game::cPlayerManager::getInstance()->getActivePlayer()->getgems.clear();
}
void TutorialUI::tutorialGemUpdate(const float & delta_time) {
	if (!turo_flags["ジェム"] && !ui_data["説明板"]->getActive() && turo_flags["ジェム２個目"] == false) {

		ui_data["移動板"]->setActive(true);
		ui_data["ジェム説明２"]->setActive(true);
		ui_data["ジェム説明３"]->setActive(true);


		ui_data["ジェム取得画像"]->setActive(true);
		ui_data["ジェム取得画像２"]->setActive(true);

		tuto_counts["ジェム説明１個目"] += delta_time;
	}
	if (tuto_counts["ジェム説明１個目"] > 3.0f && turo_flags["ジェム２個目"] == false) {
		enableNextButton();
	}

	if (tuto_counts["ジェム説明１個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["ジェム２個目"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["ジェム２個目"] = true;
		ui_data["ジェム説明２"]->setActive(false);
		ui_data["ジェム説明３"]->setActive(false);


		ui_data["ジェム取得画像"]->setActive(false);
		ui_data["ジェム取得画像２"]->setActive(false);


		ui_data["ジェム説明４"]->setActive(true);
		ui_data["ジェム説明５"]->setActive(true);
		ui_data["コントローラLR"]->setActive(true);
		ui_data["コントローラR"]->setActive(true);
		ui_data["ジェム取得画像３"]->setActive(true);
	}
	if (turo_flags["ジェム２個目"]) {
		tuto_counts["ジェム説明２個目"] += delta_time;
	}
	if (tuto_counts["ジェム説明２個目"] > 3.0f && turo_flags["ジェム"] == false) {
		enableNextButton();
	}
	if (tuto_counts["ジェム説明２個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["ジェム"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["ジェム"] = true;

		ui_data["移動板"]->setActive(false);

		ui_data["ジェム説明４"]->setActive(false);
		ui_data["ジェム説明５"]->setActive(false);
		ui_data["コントローラLR"]->setActive(false);
		ui_data["コントローラR"]->setActive(false);
		ui_data["ジェム取得画像３"]->setActive(false);
	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["ジェム"] == true
		&& turo_flags["ジェム終了"] == false) {
		tutorial_stan = false;

		ui_data["説明板２"]->setActive(true);
		ui_data["ジェム説明６"]->setActive(true);
		
		if (Game::cPlayerManager::getInstance()->getActivePlayer()->getgems.size() > 6) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["ジェム終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::DELIVERY;
		tutorial_stan = true;
	}
}

void TutorialUI::tutorialAttackSetup(const float & delta_time)
{
	playerInit();
	tutorialNumbers(5);
	auto packet = new Network::Packet::Request::cReqPlayer();
	packet->mFormat.playerId = 5;
	packet->mFormat.position = ci::vec3(Game::Field::WORLD_SIZE.x / 2, Game::Field::WORLD_SIZE.y + 0.5f, 23.0F);
	packet->mFormat.rotation = ci::quat(ci::vec3(0,M_PI,0));
	Network::cUDPClientManager::getInstance()->send(packet);

	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["攻撃説明"]->setActive(true);
	ui_data["攻撃説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["攻撃"] = false;
	turo_flags["攻撃終了"] = false;

	//カウントの初期化
	tuto_counts["攻撃説明"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}

void TutorialUI::tutorialAttackUpdate(const float & delta_time)
{
	if (!turo_flags["攻撃"] && !ui_data["説明板"]->getActive()) {

		ui_data["移動板"]->setActive(true);
		ui_data["攻撃説明２"]->setActive(true);

		ui_data["コントローラLR"]->setActive(true);
		ui_data["コントローラR"]->setActive(true);

		ui_data["攻撃画像"]->setActive(true);

		tuto_counts["攻撃説明"] += delta_time;
	}
	if (tuto_counts["攻撃説明"] > 3.0f && turo_flags["攻撃"] == false) {
		enableNextButton();
	}

	if (tuto_counts["攻撃説明"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["攻撃"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["攻撃"] = true;
		tutorial_stan = false;
		ui_data["移動板"]->setActive(false);
		ui_data["攻撃説明２"]->setActive(false);

		ui_data["コントローラLR"]->setActive(false);
		ui_data["コントローラR"]->setActive(false);

		ui_data["攻撃画像"]->setActive(false);

	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["攻撃"] == true
		&& turo_flags["攻撃終了"] == false) {
		
		ui_data["説明板２"]->setActive(true);
		ui_data["攻撃説明３"]->setActive(true);


		auto enemy = Game::cPlayerManager::getInstance()->getPlayer(5);
		if (enemy->isDead()) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["攻撃終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::ITEM;
		tutorial_stan = true;
	}
}

void TutorialUI::tutorialDeliverySetup(const float & delta_time)
{
	playerInit();
	tutorialNumbers(6);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["納品説明"]->setActive(true);
	ui_data["納品説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["納品"] = false;
	turo_flags["納品２"] = false;
	turo_flags["納品終了"] = false;

	//カウントの初期化
	tuto_counts["納品説明"] = 0.0f;
	tuto_counts["納品説明２"] = 0.0f;
	auto cannon = Game::cStrategyManager::getInstance()->getCannons()[static_cast<Game::Player::Team>(Game::cPlayerManager::getInstance()->getActivePlayer()->getWhichTeam())];
	cannon->setGemNum(0);
	c_Easing::apply(camera_angle_buf.x, -3 * M_PI / 2, EasingFunction::Linear, 10);
	c_Easing::apply(camera_angle_buf.y, -0.7f, EasingFunction::Linear, 10);
	CAMERA->setCameraFar(cannon_far);
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}

void TutorialUI::tutorialDeliveryUpdate(const float & delta_time)
{
	if (!turo_flags["納品"]) {
		CAMERA->setCameraAngle(camera_angle_buf);
		CAMERA->refPosition = Game::Field::CANNON_POINT[0];
	}

	if (!turo_flags["納品"] && !ui_data["説明板"]->getActive() && turo_flags["納品２"] == false) {

		ui_data["移動板"]->setActive(true);
		ui_data["納品説明２"]->setActive(true);


		ui_data["納品画像"]->setActive(true);
		ui_data["納品画像２"]->setActive(true);
		ui_data["納品画像３"]->setActive(true);

		tuto_counts["納品説明"] += delta_time;
	}
	if (tuto_counts["納品説明"] > 3.0f && turo_flags["納品"] == false) {
		enableNextButton();
	}

	if (tuto_counts["納品説明"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["納品"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["納品"] = true;
		tutorial_stan = false;
		ui_data["移動板"]->setActive(false);
		ui_data["納品説明２"]->setActive(false);
		
		ui_data["納品画像"]->setActive(false);
		ui_data["納品画像２"]->setActive(false);
		ui_data["納品画像３"]->setActive(false);

		CAMERA->setCameraFar(player_far);
	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["納品"] == true
		&& turo_flags["納品終了"] == false) {

		ui_data["説明板２"]->setActive(true);
		ui_data["納品説明３"]->setActive(true);
		

		auto enemy = Game::cPlayerManager::getInstance()->getPlayer(5);
		if (Game::cStrategyManager::getInstance()->getCannons()[Game::cPlayerManager::getInstance()->getActivePlayer()->getWhichTeam()]->getGEmNum() > 0) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["納品終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::ATTACK;
		tutorial_stan = true;
	}
}

void TutorialUI::tutorialItemSetup(const float & delta_time) {
	playerInit();
	tutorialNumbers(7);
	auto packet = new Network::Packet::Request::cReqPlayer();
	packet->mFormat.playerId = 5;
	packet->mFormat.position = ci::vec3(Game::Field::WORLD_SIZE.x / 2 - 0.5F, Game::Field::WORLD_SIZE.y + 1.0F, Game::Field::WORLD_SIZE.z - 8.0F );
	packet->mFormat.rotation = ci::quat();
	Network::cUDPClientManager::getInstance()->send(packet);

	auto item_pos1 = ci::vec3(Game::Field::WORLD_SIZE.x / 2+2, Game::Field::WORLD_SIZE.y + 0.5f, 20.0F);
	auto item_pos2 = ci::vec3(Game::Field::WORLD_SIZE.x / 2-2, Game::Field::WORLD_SIZE.y + 0.5f, 20.0F);
	Game::cCapsuleManager::getInstance()->createCapsule(item_pos1, Game::Weapons::SubWeapon::LIGHT_BOMB, 99);
	Game::cCapsuleManager::getInstance()->createCapsule(item_pos2, Game::Weapons::SubWeapon::QUARRY, 100);
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["アイテム説明"]->setActive(true);
	ui_data["アイテム説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["アイテム"] = false;
	turo_flags["アイテム２個目"] = false;
	turo_flags["アイテム終了"] = false;
	turo_flags["アイテム３個目"] = false;

	turo_flags["アイテム所持状態"] = false;

	//カウントの初期化
	tuto_counts["アイテム説明１個目"] = 0.0f;
	tuto_counts["アイテム説明２個目"] = 0.0f;
	tuto_counts["アイテム説明３個目"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}
void TutorialUI::tutorialItemUpdate(const float & delta_time) {

	if (!turo_flags["アイテム"] && !ui_data["説明板"]->getActive() && turo_flags["アイテム２個目"] == false) {

		ui_data["移動板"]->setActive(true);
		ui_data["アイテム説明２"]->setActive(true);

		ui_data["アイテム画像"]->setActive(true);

		tuto_counts["アイテム説明１個目"] += delta_time;
	}
	if (tuto_counts["アイテム説明１個目"] > 3.0f && turo_flags["アイテム２個目"] == false) {
		enableNextButton();
	}

	if (tuto_counts["アイテム説明１個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["アイテム２個目"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["アイテム２個目"] = true;
		ui_data["アイテム説明２"]->setActive(false);

		
		ui_data["アイテム画像"]->setActive(false);

		ui_data["アイテム画像２"]->setActive(true);
		ui_data["アイテム画像３"]->setActive(true);

		ui_data["アイテム説明３"]->setActive(true);
		ui_data["コントローラLR"]->setActive(true);
		ui_data["コントローラL"]->setActive(true);
	}
	if (turo_flags["アイテム２個目"]) {
		tuto_counts["アイテム説明２個目"] += delta_time;
	}
	if (tuto_counts["アイテム説明２個目"] > 3.0f && turo_flags["アイテム"] == false) {
		enableNextButton();
	}
	if (tuto_counts["アイテム説明２個目"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["アイテム"] == false) {
		disableNextButton();
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		turo_flags["アイテム"] = true;
		
		ui_data["移動板"]->setActive(false);

		ui_data["アイテム画像２"]->setActive(false);
		ui_data["アイテム画像３"]->setActive(false);

		ui_data["アイテム説明３"]->setActive(false);
		ui_data["コントローラLR"]->setActive(false);
		ui_data["コントローラL"]->setActive(false);
	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["アイテム"] == true
		&& turo_flags["アイテム終了"] == false) {
		tutorial_stan = false;
		ui_data["説明板２"]->setActive(true);
		ui_data["アイテム説明４"]->setActive(true);

		auto item_size = Game::cPlayerManager::getInstance()->getActivePlayer()->useSubWeapon.getItemNum();

		if (item_size > 0) {
			turo_flags["アイテム所持状態"] = true;
			ui_data["アイテム説明４"]->setActive(false);
			ui_data["アイテム説明５"]->setActive(true);
		}

		if (turo_flags["アイテム所持状態"] && item_size <= 0) {
			ui_data["OK!"]->setActive(true);
			Sound::StereophonicManager::getInstance()->add("ok", ci::app::getAssetPath("SE/Tutorial/ok.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
			tutorial_stan = true;
			turo_flags["アイテム終了"] = true;
		}
	}
	if (turo_flags["アイテム終了"]) {
		tuto_counts["アイテム説明３個目"] += delta_time;
	}
	if (tuto_counts["アイテム説明３個目"] > 3 &&
		turo_flags["アイテム説明３個目"] == false) {
		turo_flags["アイテム説明３個目"] = true;
		ui_data["FadePlateIn"]->setActive(false);
		for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
			ui_data[(*it)]->setActive(false);
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::END;
		tutorial_stan = true;
	}
}
void TutorialUI::tutorialEndSetup(const float & delta_time) {
	playerInit();
	tutorialNumbers(8);
	ui_data["移動板"]->setActive(true);
	ui_data["最後説明"]->setActive(true);
	ui_data["最後画像"]->setActive(true);
	ui_data["最後画像２"]->setActive(true);
	ui_data["FadePlateIn"]->setActive(true);

	turo_flags["最後説明"] = false;
	turo_flags["最後説明２"] = false;
	//カウントの初期化
	tuto_counts["最後説明"] = 0.0f;
	tuto_counts["最後説明２"] = 0.0f;
	Sound::StereophonicManager::getInstance()->add("tutorial", ci::app::getAssetPath("SE/Tutorial/tutorial.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
}

void TutorialUI::tutorialEndUpdate(const float & delta_time)
{
	tuto_counts["最後説明"] += delta_time;

	if (tuto_counts["最後説明"] > 3.0f && turo_flags["最後説明"] == false) {
		enableNextButton();
	}

	if (tuto_counts["最後説明"] > 3.0f
		&& (ENV->pushKey(ci::app::KeyEvent::KEY_h)
			|| ENV->isPadPush(ENV->BUTTON_2))
		&& turo_flags["最後説明"] == false) {
		Sound::StereophonicManager::getInstance()->add("mission", ci::app::getAssetPath("SE/Tutorial/mission.wav").string(), Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());
		disableNextButton();
		turo_flags["最後説明"] = true;

		ui_data["移動板"]->setActive(false);
		ui_data["最後説明"]->setActive(false);
		ui_data["最後画像"]->setActive(false);
		ui_data["最後画像２"]->setActive(false);
	}
	if (turo_flags["最後説明"] == true) {
		tuto_counts["最後説明２"] += delta_time;
	}
	if (turo_flags["最後説明２"] == false
		&& tuto_counts["最後説明２"] > 1) {
		turo_flags["最後説明２"] = true;
		ui_data["最後説明２"]->setActive(true);
		ui_data["説明板３"]->setActive(true);
	}

	if (tuto_counts["最後説明２"] > 7) {
		ui_data["FadePlateIn"]->setActive(false);

	}
	
	
	if (!ui_data["FadePlateIn"]->getActive()) {
		Resource::cSoundManager::getInstance()->findBgm("トロピカル無職.wav").stop();
		Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
	}
}

void TutorialUI::update(const float& delta_time)
{
	if (tutorial_stan) {
		Game::cPlayerManager::getInstance()->getActivePlayer()->getMainWeapon()->pullCall();
		Game::cPlayerManager::getInstance()->getActivePlayer()->getMainWeapon()->update(delta_time);
	}
	if (buf_scene != static_cast<int>(now_scene)) {
		buf_scene = static_cast<int>(now_scene);
		switch (now_scene) {
		case TUTORIAL::MOVE:
			tutorialMoveSetup(delta_time);
			break;
		case TUTORIAL::EYE:
			tutorialEyeSetup(delta_time);
			break;
		case TUTORIAL::DRILL:
			tutorialDrillSetup(delta_time);
			break;
		case TUTORIAL::GEM:
			tutorialGemSetup(delta_time);
			break;
		case TUTORIAL::ATTACK:
			tutorialAttackSetup(delta_time);
			break;
		case TUTORIAL::DELIVERY:
			tutorialDeliverySetup(delta_time);
			break;
		case TUTORIAL::ITEM:
			tutorialItemSetup(delta_time);
			break;
		case TUTORIAL::END:
			tutorialEndSetup(delta_time);
			break;
		}
	}

	switch (now_scene) {
	case TUTORIAL::MOVE:
		tutorialMoveUpdate(delta_time);
		break;
	case TUTORIAL::EYE:
		tutorialEyeUpdate(delta_time);
		break;
	case TUTORIAL::DRILL:
		tutorialDrillUpdate(delta_time);
		break;
	case TUTORIAL::GEM:
		tutorialGemUpdate(delta_time);
		break;
	case TUTORIAL::ATTACK:
		tutorialAttackUpdate(delta_time);
		break;
	case TUTORIAL::DELIVERY:
		tutorialDeliveryUpdate(delta_time);
		break;
	case TUTORIAL::ITEM :
		tutorialItemUpdate(delta_time);
		break;
	case TUTORIAL::END:
		tutorialEndUpdate(delta_time);
		break;
	}
	
	UIPlate::update(delta_time);
}

void TutorialUI::draw()
{
	for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
		if (!ui_data[(*it)]->getActive()) continue;
		
		if (ui_data[(*it)]->getUIType() == UITYPE::NormalUI ||
			ui_data[(*it)]->getUIType() == UITYPE::CollisionUI ||
			ui_data[(*it)]->getUIType() == UITYPE::GaugeUI ||
			ui_data[(*it)]->getUIType() == UITYPE::AnimationUI
			) {
			ci::gl::color(ui_data[(*it)]->getColorR(), ui_data[(*it)]->getColorG(), ui_data[(*it)]->getColorB(), ui_data[(*it)]->getColorA());
			textures[(*it)]->bind();

			ci::Rectf drawRect(ci::vec2(
				ui_data[(*it)]->getPosX(),
				ui_data[(*it)]->getPosY()),
				ci::vec2(
					ui_data[(*it)]->getPosX() + ui_data[(*it)]->getSizeX(),
					ui_data[(*it)]->getPosY() - ui_data[(*it)]->getSizeY()));

			ci::gl::draw(textures[(*it)], drawRect);
			ci::gl::color(1, 1, 1, 1);
			textures[(*it)]->unbind();

			if (ui_data[(*it)]->getUIType() == UITYPE::GaugeUI) {
				ci::gl::color(ui_data[(*it)]->getColorR(), ui_data[(*it)]->getColorG(), ui_data[(*it)]->getColorB(), ui_data[(*it)]->getColorA());
				gauge_texture[(*it)]->bind();

				ci::Rectf drawGauge(ci::vec2(
					ui_data[(*it)]->gaugeGetPosX(),
					ui_data[(*it)]->gaugeGetPosY()),
					ci::vec2(
						ui_data[(*it)]->gaugeGetPosX() + ui_data[(*it)]->gaugeGetSizeX(),
						ui_data[(*it)]->gaugeGetPosY() + ui_data[(*it)]->gaugeGetSizeY()));

				ci::gl::draw(gauge_texture[(*it)], drawGauge);
				gauge_texture[(*it)]->unbind();
				ci::gl::color(1, 1, 1, 1);
			}
		}
		else if (ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
			ci::gl::pushModelView();
			ci::gl::translate(ci::vec2(ui_data[(*it)]->getPosX() + (ui_data[(*it)]->getSizeX() / 2), ui_data[(*it)]->getPosY() + (ui_data[(*it)]->getSizeY() / 2)));
			ci::gl::scale(ci::vec2(scroll_scales[(*it)], scroll_scales[(*it)]));
			ci::gl::translate(ci::vec2(-(ui_data[(*it)]->getSizeX() / 2), -(ui_data[(*it)]->getSizeY() / 2)));
			ci::gl::color(ui_data[(*it)]->getColorR(), ui_data[(*it)]->getColorG(), ui_data[(*it)]->getColorB(), ui_data[(*it)]->getColorA());
			textures[(*it)]->bind();
			ci::Rectf drawRect(ci::vec2(
				0,
				0),
				ci::vec2(
					ui_data[(*it)]->getSizeX(),
					ui_data[(*it)]->getSizeY()));

			ci::gl::draw(textures[(*it)], drawRect);
			ci::gl::color(1, 1, 1, 1);
			textures[(*it)]->unbind();

			ci::gl::translate(ui_data[(*it)]->fontGetPosX(), ui_data[(*it)]->fontGetPosY());
			ci::gl::translate(ci::vec2(-50, -50));
			ci::gl::scale(ci::vec2(ui_data[(*it)]->fontGetScale(), ui_data[(*it)]->fontGetScale()));
			ci::gl::color(
				ui_data[(*it)]->fontGetColorR(),
				ui_data[(*it)]->fontGetColorG(),
				ui_data[(*it)]->fontGetColorB(),
				ui_data[(*it)]->fontGetColorA());
			ci::gl::draw(ci::gl::Texture2d::create(font[(*it)].render(true)));

			ci::gl::color(ci::Color::white());
			ci::gl::popModelView();


		}
		else if (ui_data[(*it)]->getUIType() == UITYPE::FontUI ||
			ui_data[(*it)]->getUIType() == UITYPE::IncrementTimeUI ||
			ui_data[(*it)]->getUIType() == UITYPE::DecrementTimeUI) {

			ci::gl::pushModelView();
			ci::gl::translate(ui_data[(*it)]->getPosX(), ui_data[(*it)]->getPosY());
			ci::gl::translate(ci::vec2(-50, 50));
			ci::gl::scale(ci::vec2(ui_data[(*it)]->fontGetScale(), -ui_data[(*it)]->fontGetScale()));
			ci::gl::color(
				ui_data[(*it)]->fontGetColorR(),
				ui_data[(*it)]->fontGetColorG(),
				ui_data[(*it)]->fontGetColorB(),
				ui_data[(*it)]->fontGetColorA());
			ci::gl::draw(font_texture[(*it)]);

			ci::gl::color(ci::Color::white());

			ci::gl::popModelView();
		}
	}
}


