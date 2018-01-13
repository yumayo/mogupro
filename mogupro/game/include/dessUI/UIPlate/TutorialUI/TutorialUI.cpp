#include "TutorialUI.h"
#include <Utility/cInput.h>
#include <Game/cPlayerManager.h>
#include <Game/Field/FieldData.h>
#include <CameraManager/cCameraManager.h>
void TutorialUI::setup(const dess::SceneName & name)
{
	now_scene = TUTORIAL::MOVE;
	buf_scene = static_cast<int>(TUTORIAL::NONE);
	UIPlate::setup(name);
	textInit();
}
//プレイヤー初期化
void TutorialUI::playerInit()
{
	Game::cPlayerManager::getInstance()->getActivePlayer()->settingPosition(ci::vec3(Game::Field::WORLD_SIZE.x / 2, Game::Field::WORLD_SIZE.y + 0.5f, 17.0F));
	Game::cPlayerManager::getInstance()->getActivePlayer()->move(ci::vec3(0, 0, 0.1f));
	CAMERA->setCameraAngle(cinder::vec2(0, 0));
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
	ui_data["掘る説明２"]->fontSetText(u8"「LB」ボタンを長押しすると\n地面を掘ることができます。");
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
void TutorialUI::tutorialMoveSetup(const float & delta_time)
{
	Game::cPlayerManager::getInstance()->getActivePlayer()->settingPosition(ci::vec3(Game::Field::WORLD_SIZE.x / 2, Game::Field::WORLD_SIZE.y + 0.5f, 17.0F));
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

}
void TutorialUI::tutorialMoveUpdate(const float& delta_time)
{

	if (!turo_flags["操作説明"] && !ui_data["説明板"]->getActive()) {
		turo_flags["操作説明"] = true;
		ui_data["説明板初期"]->setActive(true);
		ui_data["説明板初期"]->setActive(false);
		ui_data["移動説明初期"]->setActive(true);
		ui_data["移動説明初期"]->setActive(false);
	}
	if (turo_flags["操作説明"] && !turo_flags["移動"] && !ui_data["説明板初期"]->getActive()) {

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

	}
	if (ENV->pullKey(ci::app::MouseEvent::RIGHT_DOWN)) {
		turo_flags["移動"] = true;

		ui_data["移動板"]->setActive(false);
		ui_data["移動説明"]->setActive(false);
		ui_data["移動説明２"]->setActive(false);

		ui_data["コントローラ左"]->setActive(false);
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

	}

	if (ui_data["移動板"]->getActive() == false && turo_flags["移動"] == true
		&& turo_flags["移動終了"] == false) {
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
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["移動終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::EYE;
	}
}
void TutorialUI::tutorialEyeSetup(const float & delta_time)
{
	playerInit();
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

	}


	if (ENV->pullKey(ci::app::MouseEvent::RIGHT_DOWN)) {
		turo_flags["視点"] = true;

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
			ui_data["FadePlateIn"]->setActive(false);
			for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
				ui_data[(*it)]->setActive(false);
			}
			turo_flags["視点終了"] = true;
		}
	}
	if (!ui_data["FadePlateIn"]->getActive()) {
		now_scene = TUTORIAL::DRILL;
	}


}

void TutorialUI::tutorialDrillSetup(const float & delta_time) {
	playerInit();
	ui_data["説明板"]->setActive(true);
	ui_data["説明板"]->setActive(false);
	ui_data["掘る説明"]->setActive(true);
	ui_data["掘る説明"]->setActive(false);
	ui_data["FadePlateIn"]->setActive(true);

	//進行フラグ初期化
	turo_flags["掘る"] = false;
	turo_flags["掘る終了"] = false;

}
void TutorialUI::tutorialDrillUpdate(const float & delta_time){

	if (!turo_flags["掘る"] && !ui_data["説明板"]->getActive()) {

		ui_data["移動板"]->setActive(true);
		ui_data["掘る説明２"]->setActive(true);

		ui_data["コントローラLR"]->setActive(true);
		ui_data["コントローラL"]->setActive(true);

		ui_data["掘ってる画像"]->setActive(true);

	}


	if (ENV->pullKey(ci::app::MouseEvent::RIGHT_DOWN)) {
		turo_flags["視点"] = true;

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
}
void TutorialUI::update(const float& delta_time)
{
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


