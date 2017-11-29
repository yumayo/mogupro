#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "../../Collision/cRigidBody.h"
#include "GemData.h"

namespace Game
{
	namespace Gem
	{

		using namespace ci::gl;
		using namespace ci;
		class cFragmentGem
		{
		public:
			cFragmentGem(int id,vec3 position, vec3 scale, ColorA color, GemType type) : mId(id), mPosition(position), mScale(scale), mColorA(color), mType(type) {};
			~cFragmentGem() {};
			void draw();
			void update();

			int getId() { return mId; }
			vec3 getPos() { return mPosition; }
			vec3 getScale() { return mScale; }
			ColorA getColorA() { return mColorA; }
			GemType getType() { return mType; }
			bool isActive() { return mIsActive; }
			void setPos(vec3 position) { mPosition = position; }
			void setScale(vec3 scale) { mScale = scale; }
			void setColor(ColorA colorA) { mColorA = colorA; }
			void setIsActive(bool isActive = true) { mIsActive = isActive; }
			void setType(GemType type) { mType = type; }

		private:

			int mId;
			vec3 mPosition;
			vec3 mScale;
			ColorA mColorA;
			bool mIsActive;
			GemType mType;
		};
	}
}
