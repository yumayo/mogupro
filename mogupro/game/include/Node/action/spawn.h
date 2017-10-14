#pragma once
#include <Node/Action/timeline.h>
namespace Node
{
namespace Action
{
// 複数のアクションを並列に実行します。
// final class
class spawn : public timeline
{
public:
    template<class Head, class... Tail>
    CREATE_H( spawn, Head const& head, Tail const& ...tail );
    bool init( );
    template <class Head, class... Tail>
    bool init( Head const& head, Tail const& ...tail );
public:
    void setup( ) final override;
    bool is_done( )  final override;
    float update( float delta )  final override;
    void restart( )  final override;
};

template<class Head, class... Tail>
inline CREATE_CPP( spawn, Head const& head, Tail const& ...tail )
{
    CREATE( spawn, head, tail... );
}

template<class Head, class... Tail>
inline bool spawn::init( Head const& head, Tail const& ...tail )
{
    _actions.emplace_back( head );
    return init( tail... );
}
}
}