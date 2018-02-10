#pragma once
#include "../UIPlate.h"

class TutorialUI : public UIPlate
{ 
private:
	enum TUTORIAL {
		NONE = 0,
		MOVE = 1,
		EYE = 2,
		DRILL = 3,
		GEM = 4,
		ATTACK = 5,
		DELIVERY = 6,
		ITEM = 7,
		END = 8
	};
	TUTORIAL now_scene;
	std::unordered_map<std::string, ci::gl::Texture2dRef> font_texture;
	//進行フラグ
	std::unordered_map<std::string, bool> turo_flags;
	std::unordered_map<std::string, float> tuto_counts;
	int buf_scene;
	float cannon_far = 20.0f;
	float player_far = 5.0f;
	bool tutorial_stan;
	ci::vec2 camera_angle_buf = cinder::vec2(-M_PI/2, 0);
public:
	TutorialUI(){
	}
	bool getTutorialStan() { return tutorial_stan; }
	//プレイヤー初期化
	void playerInit();
	//テキスト初期化
	void textInit();
	//次へボタン
	void enableNextButton();
	void disableNextButton();
	void tutorialNumbers(int num);
	//移動
	void tutorialMoveSetup(const float& delta_time);
	void tutorialMoveUpdate(const float& delta_time);

	//視点
	void tutorialEyeSetup(const float& delta_time);
	void tutorialEyeUpdate(const float& delta_time);

	//掘る
	void tutorialDrillSetup(const float& delta_time);
	void tutorialDrillUpdate(const float& delta_time);

	//ジェム
	void tutorialGemSetup(const float& delta_time);
	void tutorialGemUpdate(const float& delta_time);

	//攻撃
	void tutorialAttackSetup(const float& delta_time);
	void tutorialAttackUpdate(const float& delta_time);

	//納品
	void tutorialDeliverySetup(const float& delta_time);
	void tutorialDeliveryUpdate(const float& delta_time);

	//アイテム
	void tutorialItemSetup(const float& delta_time);
	void tutorialItemUpdate(const float& delta_time);

	//終了
	void tutorialEndSetup(const float& delta_time);
	void tutorialEndUpdate(const float& delta_time);

	void setup(const dess::SceneName& name) override;
	void update(const float& delta_time) override;
	void draw() override;

};
