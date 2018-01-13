#include "UIManager.h"


std::shared_ptr<UIBase> selectUIType(const int& type) {
	switch (type)
	{
	case 0:
		return std::make_shared<CollisionUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 1:
		return std::make_shared<FontUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 2:
		return std::make_shared<GaugeUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 3:
		return std::make_shared<IncrementTimeUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 4:
		return std::make_shared<DecrementTimeUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 5:
		return std::make_shared<AnimationUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	case 6:
		return std::make_shared<ScrollUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
		break;
	}
	return std::make_shared<CollisionUI>(des::Vec2f(0, 0), des::Vec2f(0, 0), des::Vec4d(1, 1, 1, 1));
}

std::function<float(float, float, float)> selectEasing(int state) {
	switch (state)
	{
	case 0:
		return EasingFunction::Linear;
		break;
	case 1:
		return EasingFunction::BackIn;
		break;
	case 2:
		return EasingFunction::BackInOut;
		break;
	case 3:
		return EasingFunction::BackOut;
		break;
	case 4:
		return EasingFunction::BounceIn;
		break;
	case 5:
		return EasingFunction::BounceInOut;
		break;
	case 6:
		return EasingFunction::BounceOut;
		break;
	case 7:
		return EasingFunction::CircIn;
		break;
	case 8:
		return EasingFunction::CircInOut;
		break;
	case 9:
		return EasingFunction::CircOut;
		break;
	case 10:
		return EasingFunction::CubicIn;
		break;
	case 11:
		return EasingFunction::CubicInOut;
		break;
	case 12:
		return EasingFunction::CubicOut;
		break;
	case 13:
		return EasingFunction::ElasticIn;
		break;
	case 14:
		return EasingFunction::ElasticInOut;
		break;
	case 15:
		return EasingFunction::ElasticOut;
		break;
	case 16:
		return EasingFunction::ExpoIn;
		break;
	case 17:
		return EasingFunction::ExpoInOut;
		break;
	case 18:
		return EasingFunction::ExpoOut;
		break;
	case 19:
		return EasingFunction::QuadIn;
		break;
	case 20:
		return EasingFunction::QuadInOut;
		break;
	case 21:
		return EasingFunction::QuadOut;
		break;
	case 22:
		return EasingFunction::QuartIn;
		break;
	case 23:
		return EasingFunction::QuartInOut;
		break;
	case 24:
		return EasingFunction::QuartOut;
		break;
	case 25:
		return EasingFunction::QuintIn;
		break;
	case 26:
		return EasingFunction::QuintInOut;
		break;
	case 27:
		return EasingFunction::QuintOut;
		break;
	case 28:
		return EasingFunction::SineIn;
		break;
	case 29:
		return EasingFunction::SineInOut;
		break;
	case 30:
		return EasingFunction::SineOut;
		break;
	}
	return EasingFunction::SineOut;
}

void loadFileUIType(std::unordered_map<std::string, int>& ui_type, const Json::Value& root_type) {
	int i = 0;
	for (auto it = root_type["UIType"].begin(); it != root_type["UIType"].end(); it++) {
		ui_type[it->asString()] = i;
		i++;
	}
}
std::string getSceneStr(const dess::SceneName& scene) {
	switch (scene) {
	case dess::SceneName::TITLE:
		return "Title";
	case dess::SceneName::GAMEMAIN:
		return "GameMain";
	case dess::SceneName::SELECT:
		return "Select";
	case dess::SceneName::RESULT:
		return "Result";
	case dess::SceneName::TUTORIAL:
		return "Tutorial";
	}
	return nullptr;
}

void sceneSelect(const dess::SceneName& scene, std::vector<std::string>& ui_objects, const Json::Value& root_type) {

	if (scene == dess::SceneName::TITLE) {
		for (auto it = root_type["Title"].begin(); it != root_type["Title"].end(); it++) {
			ui_objects.push_back(it->asString());
		}
		return;
	}

	if (scene == dess::SceneName::GAMEMAIN) {
		for (auto it = root_type["GameMain"].begin(); it != root_type["GameMain"].end(); it++) {
			ui_objects.push_back(it->asString());
		}
		return;
	}

	if (scene == dess::SceneName::SELECT) {
		for (auto it = root_type["Select"].begin(); it != root_type["Select"].end(); it++) {
			ui_objects.push_back(it->asString());
		}
		return;
	}

	if (scene == dess::SceneName::RESULT) {
		for (auto it = root_type["Result"].begin(); it != root_type["Result"].end(); it++) {
			ui_objects.push_back(it->asString());
		}
		return;
	}
	if (scene == dess::SceneName::TUTORIAL) {
		for (auto it = root_type["Tutorial"].begin(); it != root_type["Tutorial"].end(); it++) {
			ui_objects.push_back(it->asString());
		}
		return;
	}
}

void selectUILoad(const dess::SceneName& scene, Json::Value& data) {
	
	if (scene == dess::SceneName::TITLE) {
		data = data["Title"];
		return;
	}
	if (scene == dess::SceneName::SELECT) {
		data = data["Select"];
		return;
	}
	if (scene == dess::SceneName::GAMEMAIN) {
		data = data["GameMain"];
		return;
	}
	if (scene == dess::SceneName::RESULT) {
		data = data["Result"];
		return;
	}
	if (scene == dess::SceneName::TUTORIAL) {
		data = data["Tutorial"];
		return;
	}
}

void UIManager::setEasing(const Json::Value& root, const std::string& it, const int& type) {

	for (auto ary = root.begin(); ary != root.end(); ary++) {
		float change_buf[4];
		int i = 0;
		for (auto end = (*ary)["Change"].begin(); end != (*ary)["Change"].end(); end++) {
			change_buf[i] = end->asFloat();
			i++;
		}
		switch (type)
		{
		case 1:
			ui_data[it]->EaseInApply(
				(*ary)["UIState"].asString(),
				change_buf,
				selectEasing(easing_type[(*ary)["EasingType"].asString()]),
				(*ary)["Frame"].asFloat());
			break;
		case 2:
			ui_data[it]->EaseOutApply(
				(*ary)["UIState"].asString(),
				change_buf,
				selectEasing(easing_type[(*ary)["EasingType"].asString()]),
				(*ary)["Frame"].asFloat());
			break;
		case 3:
			ui_data[it]->EaseUpdateApply(
				(*ary)["UIState"].asString(),
				change_buf,
				selectEasing(easing_type[(*ary)["EasingType"].asString()]),
				(*ary)["Frame"].asFloat());
			break;
		case 4:
			ui_data[it]->EaseCollisionApply(
				(*ary)["UIState"].asString(),
				change_buf,
				selectEasing(easing_type[(*ary)["EasingType"].asString()]),
				(*ary)["Frame"].asFloat());
			break;
		}

	}
}
void UIManager::loadEasingType(const Json::Value& root_type) {
	int i = 0;
	for (auto it = root_type["EasingType"].begin(); it != root_type["EasingType"].end(); it++) {
		easing_type[it->asString()] = i;
		i++;
	}
}

std::string loadString(std::string const& relative_path) {
	return std::string(static_cast<char*>(ci::app::loadAsset(relative_path)->getBuffer()->getData()));
}

//jsonデータのファイル読み込み
void UIManager::JsonInit(const dess::SceneName& scene)
{

	Json::Value root_type;
	Json::Reader reader;
	std::unordered_map<std::string, int> ui_type;
	if (reader.parse(loadString("UI/UIType.json") , root_type)) {
		//UIで使うイージングの読み込み
		loadEasingType(root_type);

		//UIのオブジェクトをUIType.jsonを参照してファイル読み込み
		//そのシーンのuiを読み込む
		sceneSelect(scene, ui_objects, root_type);
		//UIの属性をUIType.jsonを参照してファイル読み込み
		
		loadFileUIType(ui_type, root_type);
	}

	Json::Value root;
	if (reader.parse(loadString("UI/UI.json"), root)) {
		//UIType.jsonのオブジェクトネームとUI.jsonのUIタイプに応じて
		//uiにinsert
		selectUILoad(scene, root);
		for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {

			ui_data[*it] = selectUIType(ui_type[root[(*it)]["UIType"].asString()]);

			if (ui_data[(*it)]->getUIType() == UITYPE::NormalUI ||
				ui_data[(*it)]->getUIType() == UITYPE::CollisionUI ||
				ui_data[(*it)]->getUIType() == UITYPE::GaugeUI ||
				ui_data[(*it)]->getUIType() == UITYPE::AnimationUI ||
				ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
				//テクスチャのパスをセット
				ui_data[(*it)]->setTexturePath(root[(*it)]["TexturePath"].asString());
			}
			else {
				//ui_data[(*it)]->fontSetPath(root[(*it)]["FontPath"].asString());
			}

			

			//初期Posをjsonからセット
			ui_data[(*it)]->setPos(root[(*it)]["Pos"][0].asFloat(),root[(*it)]["Pos"][1].asFloat());
			ui_data[(*it)]->setColor(root[(*it)]["Color"][0].asFloat(), root[(*it)]["Color"][1].asFloat(), root[(*it)]["Color"][2].asFloat(), root[(*it)]["Color"][3].asFloat());
			if (ui_data[(*it)]->getUIType() == UITYPE::NormalUI ||
				ui_data[(*it)]->getUIType() == UITYPE::CollisionUI ||
				ui_data[(*it)]->getUIType() == UITYPE::GaugeUI ||
				ui_data[(*it)]->getUIType() == UITYPE::AnimationUI ||
				ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
				//初期Sizeをjsonからセット
				ui_data[(*it)]->setSize(root[(*it)]["Size"][0].asFloat(),
					root[(*it)]["Size"][1].asFloat());
			}
			else {
				ui_data[(*it)]->fontSetSize(root[(*it)]["Size"].asFloat());
			}


			//開始イージングをjsonからセット
			setEasing(root[(*it)]["EaseIn"], *it,1);

			//終了イージングをjsonからセット
			setEasing(root[(*it)]["EaseOut"], *it,2);

			//常時イージングをjsonからセット
			setEasing(root[(*it)]["EaseUpdate"], *it,3);

			//CollisionUIの場合のjsonのセット
			if (root[(*it)]["UIType"].asString() == "CollisionUI" ||
				root[(*it)]["UIType"].asString() == "ScrollUI") {
				setEasing(root[(*it)]["EaseCollision"], *it,4);
			}

			//GaugeUIの場合のjsonのセット
			if (root[(*it)]["UIType"].asString() == "GaugeUI") {
				//Gaugeテクスチャのパスをセット
				ui_data[(*it)]->gaugeSetTexturePath(root[(*it)]["GaugeTexturePath"].asString());

				//Gauge初期Posをjsonからセット
				ui_data[(*it)]->gaugeSetPos(root[(*it)]["GaugePos"][0].asFloat(),
					root[(*it)]["GaugePos"][1].asFloat());
				

				//Gauge初期Sizeをjsonからセット
				ui_data[(*it)]->gaugeSetSize(root[(*it)]["GaugeSize"][0].asFloat(),
					root[(*it)]["GaugeSize"][1].asFloat());

			}

			//FontUIの場合のTextをセット
			if (ui_data[(*it)]->getUIType() == UITYPE::FontUI||
				ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
				ui_data[(*it)]->fontSetScale(root[(*it)]["FontScale"].asFloat());
				ui_data[(*it)]->fontSetColor(
					root[(*it)]["FontColor"][0].asFloat(),
					root[(*it)]["FontColor"][1].asFloat(),
					root[(*it)]["FontColor"][2].asFloat(),
					root[(*it)]["FontColor"][3].asFloat());
				ui_data[(*it)]->fontSetText(root[(*it)]["Text"].asString());
			}

			//DecrementTimeUIの場合の時間をセット
			if (ui_data[(*it)]->getUIType() == UITYPE::DecrementTimeUI) {
				ui_data[(*it)]->timeSetFlame(root[(*it)]["Flame"].asInt());
				ui_data[(*it)]->timeSetSeconds(root[(*it)]["Seconds"].asInt());
				ui_data[(*it)]->timeSetMinutes(root[(*it)]["Minutes"].asInt());
			}

			//AnimationUIの時のJsonのセット
			if (ui_data[(*it)]->getUIType() == UITYPE::AnimationUI) {

				ui_data[(*it)]->animationSetSeats(
					root[(*it)]["Seets"][0].asInt(),
					root[(*it)]["Seets"][1].asInt());

				ui_data[(*it)]->animationSetCutSize(
					root[(*it)]["CutSize"][0].asInt(),
					root[(*it)]["CutSize"][1].asInt());

				ui_data[(*it)]->animationSetBetween(root[(*it)]["Between"].asFloat());

				ui_data[(*it)]->animationSetLooping(root[(*it)]["Looping"].asBool());
			}

			if (ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
				ui_data[(*it)]->fontSetPos(root[(*it)]["FontPos"][0].asFloat(),
					root[(*it)]["FontPos"][1].asFloat());
				ui_scrolls_key.push_back((*it));
				scroll_scales[(*it)] = 1;
			}
		}
	}
}


