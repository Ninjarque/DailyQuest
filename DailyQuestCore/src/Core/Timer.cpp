#include "Timer.h"

std::unordered_map<int, std::chrono::steady_clock::time_point> Timer::times;

void Timer::start(int id)
{
	auto s = std::chrono::steady_clock::now();
	times.insert({ id, s });
}

float Timer::end(int id, TIME_TYPE timeType)
{
	float dt = 0.0f;
	if (!times.count(id))
		return dt;
	auto s = times[id];
	times.erase(id);
	auto e = std::chrono::steady_clock::now();
	switch (timeType)
	{
	case TIME_TYPE::MILLISECONDES:
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		break;
	case TIME_TYPE::SECONDES:
		dt = std::chrono::duration_cast<std::chrono::seconds>(e - s).count();
		break;
	case TIME_TYPE::MINUTES:
		dt = std::chrono::duration_cast<std::chrono::minutes>(e - s).count();
		break;
	case TIME_TYPE::HOURS:
		dt = std::chrono::duration_cast<std::chrono::hours>(e - s).count();
		break;
	}
	return dt;
}