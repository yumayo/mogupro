#pragma once
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Utility/cUserPointer.hpp>
#include <Game/Light/cPointLightParam.h>
#include <Utility/cSingletonAble.h>

namespace Particle
{
enum class ParticleType
{
    NONE,
    SCATTER,    // 重力無しの飛び散り
    SPLASH,     // 重力ありの飛び散り
    GLITTER,    // 動きなしキラキラ
};

enum class ParticleTextureType
{
    NONE,
    SAND,
    SPARK,
};

class cParticle
{
public:

    cParticle() {}
    cParticle( const ci::vec3& vec, const ci::vec3& position = ci::vec3() );

    void update();

public:
    ci::vec3 mPosition;
    ci::vec3 mVec;
};

class cParticleHolder
{
public:

    cParticleHolder( const ci::vec3& position,
                     const ParticleType& type,
                     const ParticleTextureType& texture_type,
                     const float& time );
    ~cParticleHolder();

    void update( const float& delta_time );
    void draw( const glm::quat& rotation );

    bool IsActive();

private:

    void sortByCamera();
    void create();

private:

    ci::vec3 mPosition;
    Utility::softptr<Game::Light::cPointLightParam> mHandle;
    std::vector<std::shared_ptr<cParticle>> mParticles;
    std::string mTextureName;
    float mTime;

};

class cParticleManager : public Utility::cSingletonAble<cParticleManager>
{
public:

    cParticleManager();
    ~cParticleManager();

    void update( const float& delta_time );
    void draw();

    // パーティクルを生成する
    // position     : 生成位置 
    // type         : 飛ばす種類
    // texture_type : 画像の種類
    void create( const ci::vec3& position,
                 const ParticleType& type,
                 const ParticleTextureType& texture_type,
                 const float& time );

private:

    void builbordUpdate();

private:

    std::vector<std::shared_ptr<cParticleHolder>> mParticleHolders;
    glm::quat mBuilbordRotate;

};
}
