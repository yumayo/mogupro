#include <Utility/Schedule.h>
#include <Utility/cScheduler.h>
namespace Utility
{
ScheduleHandle schedule( float updateSeconds, std::function<void( )> method, int callNum )
{
	return Utility::cScheduler::getInstance( )->apply( updateSeconds, method, callNum );
}
}