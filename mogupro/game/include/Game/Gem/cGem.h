#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Node\node.h"
#include "Node/action.hpp"
#include "../cPlayerManager.h"
#include "Utility\cTimeMeasurement.h"

namespace Game
{
	namespace Gem
	{
		const float visibleRange = 20;

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
			cGem(int id,ci::vec3 postion, ci::vec3 scale, ci::ColorA color, GemType type, float delay)
				: mId(id),mPosition(postion), mScale(scale), mColor(color), mType(type), mDelay(delay) {};
			~cGem() {};

			void setUp(ci::vec3 postion, ci::vec3 scale, ci::ColorA color, GemType type, float delay);
			void draw();
			void drawFbo();
			void update();

			int getId() { return mId; }
			ci::vec3 getPos() { return mPosition; }
			ci::vec3 getCenterPos() { return mPosition + mScale / 2.0f; }
			ci::vec3 getPutPos() { return mPutPosition; }
			ci::vec3 getScale() { return mScale; }
			GemType getType() { return mType; }
			ci::ColorA getColor() { return mColor; }
			float getSinRotate() { return mSinrotate; }
			bool getIsDrillhit() { return misdrillhit; }
			std::vector<uint32_t> getIndices() { return indices; }
			std::vector<ci::vec3> getNomals() { return nomals; }
			std::vector<ci::ColorA> getColorAs() { return colorAs; }
			void setPos(ci::vec3 pos) { mPosition = pos; }
			void setPutPos(ci::vec3 pos) { mPutPosition = pos; }
			void setSinRotate(float rotate) { mSinrotate = rotate; }
			void setIsDrillhit(bool ishit) { misdrillhit = ishit; }
			void setIndices(int offset);
			void setNomals() { for (int i = 0; i < 24; i++) { nomals.push_back(BOXNOMAL[i]); } };
			void setColorAs() { for (int i = 0; i < 24; i++) { colorAs.push_back(mColor); } }
			hardptr<Node::node> root;
			void cGem::deleteGem();
			

		private:

			

			int mId;
			ci::vec3 mPosition;
			ci::vec3 mPutPosition;
			ci::vec3 mScale;
			ci::vec3 mRotation;
			ci::Color mColor;
			float mDelay;
			GemType mType;
			std::vector<uint32_t> indices;
			std::vector<ci::vec3> nomals;
			std::vector<ci::ColorA> colorAs;
			float mSinrotate;
			bool misdrillhit = false;
			
	

			uint32_t BOXINDICES[36] = 
			{
				0, 1, 2, 0, 2, 3,
				4, 5, 6, 4, 6, 7,
				8, 9,10, 8, 10,11,
				12,13,14,12,14,15,
				16,17,18,16,18,19,
				20,21,22,20,22,23
			};

			ci::vec3 BOXNOMAL[24] =
			{ ci::vec3(1,0,0),ci::vec3(1,0,0),ci::vec3(1,0,0),ci::vec3(1,0,0),
				ci::vec3(0,1,0),ci::vec3(0,1,0),ci::vec3(0,1,0),ci::vec3(0,1,0),
				ci::vec3(0,0,1),ci::vec3(0,0,1),ci::vec3(0,0,1),ci::vec3(0,0,1),
				ci::vec3(-1,0,0),ci::vec3(-1,0,0),ci::vec3(-1,0,0),ci::vec3(-1,0,0),
				ci::vec3(0,-1,0),ci::vec3(0,-1,0),ci::vec3(0,-1,0),ci::vec3(0,-1,0),
				ci::vec3(0,0,-1),	ci::vec3(0,0,-1),ci::vec3(0,0,-1),ci::vec3(0,0,-1)
			};

		};
	}
}