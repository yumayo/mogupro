#pragma once
#include <Utility/MessageBox.h>
namespace Utility
{
template<class Ty>
class cSingletonAble
{
public:
    virtual ~cSingletonAble( )
    {

    }
    static Ty* getInstance( )
    {
        if ( !mInstance )
        {
            mInstance = new Ty( );
        }
        return mInstance;
    }
    static void removeInstance( )
    {
        delete mInstance;
        mInstance = nullptr;
    }
private:
    static Ty* mInstance;
};
template<class Ty>
Ty* cSingletonAble<Ty>::mInstance = nullptr;
}