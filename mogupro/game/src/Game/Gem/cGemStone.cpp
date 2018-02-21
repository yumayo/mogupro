#include <Game/Gem/cGemStone.h>
#include <Utility/cTimeMeasurement.h>
#include <Game/cLightManager.h>
#include <Game/cShaderManager.h>

namespace Game
{
	namespace Gem
	{

		cGemStone::cGemStone(int id, ci::vec3 postion, ci::vec3 scale, ci::ColorA color, GemType type)
			: mId(id), mPosition(postion), mScale(scale), mColor(color), mType(type), mIsActive(true), mAabb(postion, scale)
		{
			setColorAs();
			setNomals();
			mPointLightHandle = cLightManager::getInstance( )->addPointLight( mPosition, ci::vec3( mColor.r, mColor.g, mColor.b ), 2.0F );

			// /////////���C�g�̃f�[�^���l�ߍ��ށB
			pointLightIds.emplace_back(mPointLightHandle->getId());
		};
		cGemStone::~cGemStone()
		{
			mAabb.removeWorld();
		};


		void cGemStone::setUp()
		{
			mAabb.addWorld();
		}


		void cGemStone::draw()
		{
			if (!mIsActive) return;

			// ////////////�܂Ƃ߂Ă�����ID���g���ă��C�g�𔽉f������B
			cShaderManager::getInstance()->uniformUpdate(pointLightIds, lineLightIds, spotLightIds);

			ci::gl::pushModelMatrix();
			ci::gl::color(ci::Color(mColor));
			ci::gl::translate(mPosition);
			ci::gl::scale(ci::vec3(2));
			Resource::cFbxManager::getInstance()->draw("Gemstone3");
			ci::gl::color(ci::ColorA(1, 1, 1, 1));
			ci::gl::popModelMatrix();
		}


		void cGemStone::drawFbo()
		{
		}


		void cGemStone::update()
		{
		}

		void cGemStone::setIndices(int offset)
		{
			for (int i = 0; i < 36; i++)
			{
				indices.push_back(BOXINDICES[i] + offset);
			}
		}

		void  cGemStone::setIsActive(bool isActive)
		{
			mIsActive = isActive;
			if ( mIsActive == false )
			{
				mPointLightHandle->reAttachRadius( 0.0F );
			}
		}

		//---------------------------//
		//        ��΂�����         //
		//---------------------------//
		//���_���ꂩ���ɏW�߂ĕ�΂������Ȃ�����
		void cGemStone::deleteGem()
		{
			for (size_t i = 0; i < indices.size(); i++)
			{
				indices[i] = 0;
			}
			setIsActive( false );
		}
	}
}