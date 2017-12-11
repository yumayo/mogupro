#pragma once
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Game/Gem/GemData.h>
#include <Node/node.h>
#include <Node/action.hpp>
#include <Collision/cRigidBody.h>
#include <Collision/cAABBCollider.h>

namespace Game
{
	namespace Gem
	{


		class cGemStone
		{


		public:
			// id          id
			// positin     位置
			// scale       大きさ
			// color       カラー（ここは本来Texture）
			// type        Gemの種類(これ入れたらtexture引数にいらないかも)
			cGemStone(int id,ci::vec3 postion, ci::vec3 scale, ci::ColorA color, GemType type)
				: mId(id),mPosition(postion), mScale(scale), mColor(color), mType(type), mIsActive(true), mAabb(postion,scale)
			    {};
			~cGemStone();

			void         setUp();
			void         draw();
			void         drawFbo();
			void         update();

			int          getId()         { return mId; }
			ci::vec3     getPos()        { return mPosition; }
			ci::vec3     getCenterPos()  { return mPosition + mScale / 2.0f; }
			ci::vec3     getPutPos()     { return mPutPosition; }
			ci::vec3     getScale()      { return mScale; }
			GemType      getType()       { return mType; }
			ci::ColorA   getColor()      { return mColor; }
			bool         isActive()      { return mIsActive; }
			std::vector<uint32_t>      getIndices()   { return indices; }
			std::vector<ci::vec3>      getNomals()    { return nomals; }
			std::vector<ci::ColorA>    getColorAs()   { return colorAs; }
			Collision::cAABBCollider   getAabb()      { return mAabb; }

			void         setPos(ci::vec3 pos)         { mPosition = pos; }
			void         setPutPos(ci::vec3 pos)      { mPutPosition = pos; }
			void         setIndices(int offset);
			void         setIsActive(bool isActive)   { mIsActive = isActive; }
			void         setNomals()                  { for (int i = 0; i < 24; i++) { nomals.push_back(BOXNOMAL[i]); } }
			void         setColorAs()                 { for (int i = 0; i < 24; i++) { colorAs.push_back(mColor); } }
			void         deleteGem();


			hardptr<Node::node> root;
			

		private:

			
			int           mId;
			ci::vec3      mPosition;
			ci::vec3      mPutPosition;
			ci::vec3      mScale;
			ci::vec3      mRotation;
			ci::ColorA     mColor;
			bool          mIsActive;
			GemType       mType;
			std::vector<uint32_t>        indices;
			std::vector<ci::vec3>        nomals;
			std::vector<ci::ColorA>      colorAs;
			Collision::cAABBCollider     mAabb;
	

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
			{
			  ci::vec3(1,0,0),ci::vec3(1,0,0),ci::vec3(1,0,0),ci::vec3(1,0,0),
			  ci::vec3(0,1,0),ci::vec3(0,1,0),ci::vec3(0,1,0),ci::vec3(0,1,0),
			  ci::vec3(0,0,1),ci::vec3(0,0,1),ci::vec3(0,0,1),ci::vec3(0,0,1),
			  ci::vec3(-1,0,0),ci::vec3(-1,0,0),ci::vec3(-1,0,0),ci::vec3(-1,0,0),
			  ci::vec3(0,-1,0),ci::vec3(0,-1,0),ci::vec3(0,-1,0),ci::vec3(0,-1,0),
			  ci::vec3(0,0,-1),	ci::vec3(0,0,-1),ci::vec3(0,0,-1),ci::vec3(0,0,-1)
			};

		};
	}
}