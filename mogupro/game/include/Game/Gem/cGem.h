#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

namespace Game
{
	namespace Gem
	{
		enum GemType
		{
			Dia,
			Gold,
			Silver,
			Iron
		};

		class cGem
		{
		public:
			cGem() {};
			cGem(vec3 postion, vec3 size, Color color, GemType type) : mPosition(postion), mSize(size), mColor(color), mType(type) {};
			~cGem() {};
			void SetUp(vec3 postion_, vec3 size_, Color color_, GemType type);
			void Draw();
			void Update();
		private:
			vec3 mPosition;
			vec3 mSize;
			vec3 mRotation;
			Color mColor;
			GemType mType;
			int mMoney;
		};
	}
}