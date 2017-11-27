#pragma once
#include <functional>
#include <memory>
namespace Utility
{
using ScheduleHandle = std::shared_ptr<int>;
ScheduleHandle schedule( float updateSeconds, std::function<void( )> method, int callNum = std::numeric_limits<int>::max( ) );
}