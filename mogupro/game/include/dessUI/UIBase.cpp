#include "UIBase.h"
void fncPushBack(std::vector<std::function<float(float, float, float)>> fnc) {
	fnc.push_back(EasingFunction::BackIn);
	fnc.push_back(EasingFunction::BackInOut);
	fnc.push_back(EasingFunction::BackOut);
	fnc.push_back(EasingFunction::BounceIn);
	fnc.push_back(EasingFunction::BounceInOut);
	fnc.push_back(EasingFunction::BounceOut);
	fnc.push_back(EasingFunction::CircIn);
	fnc.push_back(EasingFunction::CircInOut);
	fnc.push_back(EasingFunction::CircOut);
	fnc.push_back(EasingFunction::CubicIn);
	fnc.push_back(EasingFunction::CubicInOut);
	fnc.push_back(EasingFunction::CubicOut);
	fnc.push_back(EasingFunction::ElasticIn);
	fnc.push_back(EasingFunction::ElasticInOut);
	fnc.push_back(EasingFunction::ElasticOut);
	fnc.push_back(EasingFunction::ExpoIn);
	fnc.push_back(EasingFunction::ExpoInOut);
	fnc.push_back(EasingFunction::ExpoOut);
	fnc.push_back(EasingFunction::Linear);
	fnc.push_back(EasingFunction::QuadIn);
	fnc.push_back(EasingFunction::QuadInOut);
	fnc.push_back(EasingFunction::QuadOut);
	fnc.push_back(EasingFunction::QuartIn);
	fnc.push_back(EasingFunction::QuartInOut);
	fnc.push_back(EasingFunction::QuadOut);
	fnc.push_back(EasingFunction::QuintIn);
	fnc.push_back(EasingFunction::QuintInOut);
	fnc.push_back(EasingFunction::QuintOut);
	fnc.push_back(EasingFunction::SineIn);
	fnc.push_back(EasingFunction::SineInOut);
	fnc.push_back(EasingFunction::SineOut);
}






UIBase::UIBase(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color)
	: pos(_pos), size(_size), color(_color) {
	fncPushBack(easing_function);
	is_active = false;
	start = 0;
	end = false;
	end_count = 0;

}

void UIBase::update() {
	if (is_active) {
		EaseInUpdate();
		EaseOutUpdate();
		EaseUpdate();
	}
}

void UIBase::EaseInApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time)
{

	ease_in_buf.push_back(EasingBuf(start, end, ease, time));
}

void UIBase::EaseOutApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time)
{
	ease_end_buf.push_back(EasingBuf(start, end, ease, time));
}

void UIBase::EaseUpdateApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time)
{
	ease_update_buf.push_back(EasingBuf(start, end, ease, time));
}

void UIBase::EaseUpdate()
{
	if (c_Easing::isEnd(pos.x)&&
		c_Easing::isEnd(pos.y) && 
		c_Easing::isEnd(size.x) && 
		c_Easing::isEnd(size.y) && 
		c_Easing::isEnd(color.x) && 
		c_Easing::isEnd(color.y) && 
		c_Easing::isEnd(color.z) && 
		c_Easing::isEnd(color.w)) {
		for (int i = 0; i != ease_update_buf.size(); i++) {
			if ("Pos" == ease_update_buf[i].start) {
				c_Easing::apply(pos.x,
					ease_update_buf[i].end[0], ease_update_buf[i].ease, ease_update_buf[i].time);
				c_Easing::apply(pos.y,
					ease_update_buf[i].end[1], ease_update_buf[i].ease, ease_update_buf[i].time);
			}

			if ("Size" == ease_update_buf[i].start) {
				c_Easing::apply(size.x,
					ease_update_buf[i].end[0], ease_update_buf[i].ease, ease_update_buf[i].time);
				c_Easing::apply(size.y,
					ease_update_buf[i].end[1], ease_update_buf[i].ease, ease_update_buf[i].time);
			}

			if ("Color" == ease_update_buf[i].start) {
				c_Easing::apply(color.x,
					ease_update_buf[i].end[0], ease_update_buf[i].ease, ease_update_buf[i].time);
				c_Easing::apply(color.y,
					ease_update_buf[i].end[1], ease_update_buf[i].ease, ease_update_buf[i].time);
				c_Easing::apply(color.z,
					ease_update_buf[i].end[2], ease_update_buf[i].ease, ease_update_buf[i].time);
				c_Easing::apply(color.w,
					ease_update_buf[i].end[3], ease_update_buf[i].ease, ease_update_buf[i].time);
			}
		}
	}
}

void UIBase::EaseInUpdate()
{
	start++;
	if (start == 1) {
		for (int i = 0; i != ease_in_buf.size(); i++) {

			if ("Pos" == ease_in_buf[i].start) {
				c_Easing::apply(pos.x,
					ease_in_buf[i].end[0], ease_in_buf[i].ease, ease_in_buf[i].time);
				c_Easing::apply(pos.y,
					ease_in_buf[i].end[1], ease_in_buf[i].ease, ease_in_buf[i].time);
			}

			if ("Size" == ease_in_buf[i].start) {
				c_Easing::apply(size.x,
					ease_in_buf[i].end[0], ease_in_buf[i].ease, ease_in_buf[i].time);
				c_Easing::apply(size.y,
					ease_in_buf[i].end[1], ease_in_buf[i].ease, ease_in_buf[i].time);
			}

			if ("Color" == ease_in_buf[i].start) {
				c_Easing::apply(color.x,
					ease_in_buf[i].end[0], ease_in_buf[i].ease, ease_in_buf[i].time);
				c_Easing::apply(color.y,
					ease_in_buf[i].end[1], ease_in_buf[i].ease, ease_in_buf[i].time);
				c_Easing::apply(color.z,
					ease_in_buf[i].end[2], ease_in_buf[i].ease, ease_in_buf[i].time);
				c_Easing::apply(color.w,
					ease_in_buf[i].end[3], ease_in_buf[i].ease, ease_in_buf[i].time);
			}


		}
	}
}

void UIBase::EaseOutUpdate()
{
	if (end) {
		end_count++;
		if (end_count == 1) {
			for (int i = 0; i != ease_in_buf.size(); i++) {

				if ("Pos" == ease_in_buf[i].start) {
					c_Easing::clear(pos.x);
					c_Easing::clear(pos.y);
				}

				if ("Size" == ease_in_buf[i].start) {
					c_Easing::clear(size.x);
					c_Easing::clear(size.y);
				}

				if ("Color" == ease_in_buf[i].start) {
					c_Easing::clear(color.x);
					c_Easing::clear(color.y);
					c_Easing::clear(color.z);
					c_Easing::clear(color.w);
				}

			}
			for (int i = 0; i != ease_update_buf.size(); i++) {
				if ("Pos" == ease_update_buf[i].start) {
					c_Easing::clear(pos.x);
					c_Easing::clear(pos.y);
				}

				if ("Size" == ease_update_buf[i].start) {
					c_Easing::clear(size.x);
					c_Easing::clear(size.y);
				}

				if ("Color" == ease_update_buf[i].start) {
					c_Easing::clear(color.x);
					c_Easing::clear(color.y);
					c_Easing::clear(color.z);
					c_Easing::clear(color.w);
				}
			}
			for (int i = 0; i != ease_end_buf.size(); i++) {

				if ("Pos" == ease_end_buf[i].start) {
					c_Easing::apply(pos.x,
						ease_end_buf[i].end[0], ease_end_buf[i].ease, ease_end_buf[i].time);
					c_Easing::apply(pos.y,
						ease_end_buf[i].end[1], ease_end_buf[i].ease, ease_end_buf[i].time);
				}

				if ("Size" == ease_end_buf[i].start) {
					c_Easing::apply(size.x,
						ease_end_buf[i].end[0], ease_end_buf[i].ease, ease_end_buf[i].time);
					c_Easing::apply(size.y,
						ease_end_buf[i].end[1], ease_end_buf[i].ease, ease_end_buf[i].time);
				}

				if ("Color" == ease_end_buf[i].start) {
					c_Easing::apply(color.x,
						ease_end_buf[i].end[0], ease_end_buf[i].ease, ease_end_buf[i].time);
					c_Easing::apply(color.y,
						ease_end_buf[i].end[1], ease_end_buf[i].ease, ease_end_buf[i].time);
					c_Easing::apply(color.z,
						ease_end_buf[i].end[2], ease_end_buf[i].ease, ease_end_buf[i].time);
					c_Easing::apply(color.w,
						ease_end_buf[i].end[3], ease_end_buf[i].ease, ease_end_buf[i].time);
				}

			}

		}
		for (int i = 0; i != ease_end_buf.size(); i++) {
			if ("Pos" == ease_end_buf[i].start) {
				if (c_Easing::isEnd(pos.x)&&
					c_Easing::isEnd(pos.y)) {
					is_active = false;
					start = 0;
					end_count = 0;
					end = false;
				}
			}

			if ("Size" == ease_end_buf[i].start) {
				if (c_Easing::isEnd(size.x) &&
					c_Easing::isEnd(size.y)) {
					is_active = false;
					start = 0;
					end_count = 0;
					end = false;
				}
				
			}

			if ("Color" == ease_end_buf[i].start) {
				if (c_Easing::isEnd(color.x) &&
					c_Easing::isEnd(color.y) &&
					c_Easing::isEnd(color.z) &&
					c_Easing::isEnd(color.w)) {
					is_active = false;
					start = 0;
					end_count = 0;
					end = false;
				}
				
			}
			
		}
		if (ease_end_buf.size() == 0) {
			is_active = false;
			start = 0;
			end_count = 0;
			end = false;
		}
	}

}
