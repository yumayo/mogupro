#pragma once
#include "../UIs/CollisionUI/CollisionUI.h"
#include "../UIs/FontUI/FontUI.h"
#include "../UIs/GaugeUI/GaugeUI.h"
#include "../UIs/TextureUI/TextureUI.h"
#include "../UIs/IncrementTimeUI/IncrementTimeUI.h"
#include "../UIs/DecrementTimeUI/DecrementTimeUI.h"
#include "../UIs/AnimationUI/AnimationUI.h"
#include "../UIs/ScrollUI/ScrollUI.h"
#include <Resource/TextureManager.h>
#include <memory>
#include "cinder/app/App.h"
#include "jsoncpp/json.h"
namespace dess {
	enum class SceneName {
		TITLE,
		SELECT,
		GAMEMAIN,
		RESULT,
		TUTORIAL
	};
}
class UIManager
{
protected:
	std::unordered_map<std::string, int> easing_type;
	std::vector<std::string> ui_objects;
	std::vector<std::string> ui_scrolls_key;
	std::map<std::string,float> scroll_scales;
public:
	UIManager() {};

	std::unordered_map<std::string, std::shared_ptr<UIBase>> ui_data;
	void setEasing(const Json::Value& root,const std::string& it,const int& type);
	void JsonInit(const dess::SceneName& scene);
	void loadEasingType(const Json::Value& root_type);
};