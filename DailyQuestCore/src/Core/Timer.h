#pragma once

#include <chrono>
#include <ctime>

#include <map>
#include <unordered_map>

enum TIME_TYPE
{
	MILLISECONDES,
	SECONDES,
	MINUTES,
	HOURS,
};

class Timer
{
public:
	static void start(int id);
	static float end(int id, TIME_TYPE timeType);
private:
	static std::unordered_map<int, std::chrono::steady_clock::time_point> times;
};