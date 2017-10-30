#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Node\node.h"
#include "Node/action.hpp"

namespace Game
{
	namespace Gem
	{
		enum GemType
		{
			Dia,
			Gold,
			Iron,
			Coal,
		};

		class cGem
		{
		public:
			// id          id
			// positin     位置
			// scale       大きさ
			// color       カラー（ここは本来Texture）
			// type        Gemの種類(これ入れたらtexture引数にいらないかも)
			cGem(int id,ci::vec3 postion, ci::vec3 scale, ci::Color color, GemType type) : mId(id),mPosition(postion), mScale(scale), mColor(color), mType(type) {};
			~cGem() {};

			void setUp(ci::vec3 postion, ci::vec3 scale, ci::Color color, GemType type);
			void draw();
			void drawFbo();
			void update();

			int getId() { return mId; }
			ci::vec3 getPos() { return mPosition; }
			ci::vec3 getPutPos() { return mPutPosition; }
			ci::vec3 getScale() { return mScale; }
			GemType getType() { return mType; }
			ci::Color getColor() { return mColor; }
			float getSinRotate() { return mSinrotate; }
			bool getIsDrillhit() { return misdrillhit; }
			void setPos(ci::vec3 pos) { mPosition = pos; }
			void setPutPos(ci::vec3 pos) { mPutPosition = pos; }
			void setSinRotate(float rotate) { mSinrotate = rotate; }
			void setIsDrillhit(bool ishit) { misdrillhit = ishit; }
			void setIndices(int offset) { for (int i = 0; i < 8; i++) indices.push_back(i + offset); }
			hardptr<Node::node> root;
			
		private:

			int mId;
			ci::vec3 mPosition;
			ci::vec3 mPutPosition;
			ci::vec3 mScale;
			ci::vec3 mRotation;
			ci::Color mColor;
			GemType mType;
			std::vector<int> indices;
			int mMoney;
			float mSinrotate;
			bool misdrillhit = false;
		};
	}
}