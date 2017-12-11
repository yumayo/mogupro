#pragma once
#include <random>
namespace Utility
{
    class RandomInt
    {
    public:
        RandomInt() :
            mt(rd())
        {
        }
        RandomInt(const int& min, const int& max) :
            mt(rd()),
            rand(min, max)
        {
            // maxよりminが大きいとエラー
        }

        void setRange(const int& min, const int& max)
        {
            rand = std::uniform_int_distribution<>(min, max);
        }

        int operator()()
        {
            return rand(mt);
        }
    private:
        std::random_device rd;
        std::mt19937 mt;
        std::uniform_int_distribution<> rand;
    };

    class RandomFloat
    {
    public:
        RandomFloat() :
            mt(rd())
        {
        }
        RandomFloat(const float& min, const float& max) :
            mt(rd()),
            rand(min, max)
        {
            // maxよりminが大きいとエラー
        }

        void setRange(const float& min, const float& max)
        {
            rand = std::uniform_real_distribution<float>(min, max);
        }

        float operator()()
        {
            return rand(mt);
        }


    private:
        std::random_device rd;
        std::mt19937 mt;
        std::uniform_real_distribution<float> rand;
    };

}