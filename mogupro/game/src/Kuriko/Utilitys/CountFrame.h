# pragma once

namespace User
{
    class CountFrame
    {
        int maxFrame;
        int frame;
    public:
        CountFrame( int maxFrame );
        void Update( );
        bool IsMax( );
        float NormalizedRectSizeFrame( );
        int GetNumFrame( );
        int GetMaxFrame( );
    };
}