
#include "CountFrame.h"

#include <algorithm>

namespace User
{
    CountFrame::CountFrame( int maxFrame )
        : frame( 0 )
        , maxFrame( maxFrame )
    { }
    void CountFrame::Update( )
    {
        frame = std::min( frame + 1, maxFrame );
    }
    bool CountFrame::IsMax( )
    {
        return frame == maxFrame;
    }
    float CountFrame::NormalizedRectSizeFrame( )
    {
        return static_cast<float>( frame ) / maxFrame;
    }
    int CountFrame::GetNumFrame( )
    {
        return frame;
    }
    int CountFrame::GetMaxFrame( )
    {
        return maxFrame;
    }
}