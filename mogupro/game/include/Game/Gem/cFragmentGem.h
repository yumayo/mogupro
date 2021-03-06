#pragma once
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Game/Gem/GemData.h>
#include <Node/node.h>
#include <Node/action.hpp>
#include <Collision/cRigidBody.h>
#include <Collision/cAABBCollider.h>
#include <Game/Light/cPointLightParam.h>
#include <Resource\/cFbxManager.h>

namespace Game
{
	namespace Gem
	{

		using namespace ci::gl;
		using namespace ci;
		class cFragmentGem
		{
		public:

			cFragmentGem(int id, vec3 position, vec3 scale, ColorA color, GemType type, vec3 direction);
			~cFragmentGem();
			void      setup();
			void      draw();
			void      update();
			void      lateUpdate(const float& delta_time);


			int       getId()		  { return mId; }
			vec3      getPos()	      { return mPosition; }
			vec3      getScale()      { return mScale; }
			//vec3      getRotate()     { return mRotate;  }
			ColorA    getColorA()     { return mColorA; }
			GemType   getType()       { return mType; }
			bool      isActive()      { return mIsActive; }
			bool      getVisible()    { return mVisible; }
			float     getSinRotate()  { return mSinRotate; }
			vec3      getPutPos()     { return mPutPos; }
			Collision::cAABBCollider getAabb() { return mAabb; }

			void      setPos(vec3 position)             { mPosition = position; }
			void      setScale(vec3 scale)              { mScale = scale; }
			//void      setRotate(vec3 rotate)            { mRotate = rotate; }
			void      setColor(ColorA colorA)           { mColorA = colorA; }
			void      setIsActive(bool isActive = true) { mIsActive = isActive; }
			void      setVisible(bool visible);
			void      setIsRigid(bool isRigid);
			void      setType(GemType type)             { mType = type; }
			void      setPutPos(vec3 putPos)            { mPutPos = putPos; }
			void      setSinRotate(float sinRotate)     { mSinRotate = sinRotate; }


			hardptr<Node::node> node;
			hardptr<Node::node> popNode;

		private:

			int         mId;
			vec3        mPosition;
			vec3        mScale;
			vec3        mRotate;
			float       mRotateSpeed;
			//float       mAngle;
			vec3        mSpeed;
			ColorA      mColorA;
			bool        mIsActive;
			GemType     mType;
			float       mSinRotate;
			vec3        mPutPos;
			bool        mIsRigid;
			bool        mVisible;
			vec3        mDirection;
			Collision::cRigidBody mRb;
			Collision::cAABBCollider mAabb;
			
		};
	}
}
