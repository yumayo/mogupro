#include"AnimationUI.h"

void AnimationUI::EaseOutUpdate()
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
			if ("Pos" == ease_update_buf[i].start) {
				if (c_Easing::isEnd(pos.x) &&
					c_Easing::isEnd(pos.y)) {
					is_active = false;
					start = 0;
					end_count = 0;
					end = false;
				}
			}

			if ("Size" == ease_update_buf[i].start) {
				if (c_Easing::isEnd(size.x) &&
					c_Easing::isEnd(size.y)) {
					is_active = false;
					start = 0;
					end_count = 0;
					end = false;
				}

			}

			if ("Color" == ease_update_buf[i].start) {
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
		if (ease_end_buf.size() == 0
			&&animation_end) {
			is_active = false;
			start = 0;
			end_count = 0;
			end = false;
		}
	}

}