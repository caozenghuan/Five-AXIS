#pragma once
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;
class CTimer
{
public:
	CTimer() :m_begin(std::chrono::high_resolution_clock::now()){}
	void reset(){ m_begin = std::chrono::high_resolution_clock::now(); }
	//默认输出ms
	template <typename Duration = milliseconds>
	int64_t elapsed() const
	{
		return duration_cast <Duration> (std::chrono::high_resolution_clock::now() - m_begin).count();

	}
	//输出ns
	int64_t elapsed_micro() const
	{
		return elapsed <std::chrono::microseconds>();

	}
	//输出ns
	int64_t elapsed_nano() const
	{
		return elapsed <std::chrono::nanoseconds>();

	}
	//输出s
	int64_t elapsed_seconds() const
	{
		return elapsed <std::chrono::seconds>();

	}
	//输出s
	int64_t elapsed_minutes() const
	{
		return elapsed <std::chrono::minutes>();

	}
	//输出s
	int64_t elapsed_hours() const
	{
		return elapsed <std::chrono::hours>();

	}

protected:
private:std::chrono::time_point <std::chrono::high_resolution_clock> m_begin;
};

