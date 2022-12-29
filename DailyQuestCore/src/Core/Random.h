#pragma once

#include <random>

class Random
{
public:
	Random(unsigned int seed) 
	{
		m_randomEngine.seed(seed);
	}
	float GetFloat() 
	{
		return (float)m_distribution(m_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	static void Init()
	{
		s_randomEngine.seed(std::random_device()());
	}
	static float Float()
	{
		return (float)s_distribution(s_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_randomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;

	std::mt19937 m_randomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> m_distribution;
};