#include "UIPlate.h"


void setTexture(std::unordered_map<std::string, ci::gl::TextureRef>& mTexture, const std::vector<std::string>& ui_objects,
	std::unordered_map<std::string, std::shared_ptr<UIBase>>& ui_data, std::unordered_map<std::string, ci::TextLayout>& font,
	std::unordered_map<std::string, ci::gl::TextureRef>& gauge_texture,
	ci::Font font_data) {
	
	for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {

		if (ui_data[(*it)]->getUIType() == UITYPE::NormalUI ||
			ui_data[(*it)]->getUIType() == UITYPE::CollisionUI ||
			ui_data[(*it)]->getUIType() == UITYPE::GaugeUI ||
			ui_data[(*it)]->getUIType() == UITYPE::AnimationUI ||
			ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {
			//テクスチャのパスをセット

			auto img = ci::loadImage(ci::app::loadAsset(ui_data[(*it)]->getTexturePath()));
			mTexture[(*it)] = ci::gl::Texture::create(img);
			if (ui_data[(*it)]->getUIType() == UITYPE::GaugeUI) {
				auto img = ci::loadImage(ci::app::loadAsset(ui_data[(*it)]->gaugeGetTexturePath()));
				gauge_texture[(*it)] = ci::gl::Texture::create(img);

			}
			if (ui_data[(*it)]->getUIType() == UITYPE::ScrollUI) {

				font[(*it)].setFont(font_data);
				font[(*it)].setColor(ci::ColorA(
					ui_data[(*it)]->fontGetColorR(),
					ui_data[(*it)]->fontGetColorG(),
					ui_data[(*it)]->fontGetColorB(),
					ui_data[(*it)]->fontGetColorA()));
				font[(*it)].addLine(ui_data[(*it)]->fontGetText());
				continue;


			}

		}else if (ui_data[(*it)]->getUIType() == UITYPE::FontUI ||
			ui_data[(*it)]->getUIType() == UITYPE::IncrementTimeUI ||
			ui_data[(*it)]->getUIType() == UITYPE::DecrementTimeUI) {
			font[(*it)].setFont(font_data);
			font[(*it)].setColor(ci::ColorA(
				ui_data[(*it)]->fontGetColorR(),
				ui_data[(*it)]->fontGetColorG(),
				ui_data[(*it)]->fontGetColorB(),
				ui_data[(*it)]->fontGetColorA()));
			font[(*it)].addLine(ui_data[(*it)]->fontGetText());
		}

	}
}

void UIPlate::setup(const dess::SceneName& name)
{
	font_data = ci::Font(ci::app::loadAsset("AMEMUCHIGOTHIC-06.ttf"), 100);
	JsonInit(name);
	setTexture(textures, ui_objects, ui_data, font, gauge_texture, font_data);
}

void UIPlate::update(const float& delta_time)
{
	for (auto it = ui_objects.begin(); it != ui_objects.end(); it++) {
		ui_data[*it]->update();
	}

}

void UIPlate::draw()
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
					ui_data[(*it)]->getPosY() + ui_data[(*it)]->getSizeY()));

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
		else if(ui_data[(*it)]->getUIType() == UITYPE::ScrollUI){
			
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
	}

}
