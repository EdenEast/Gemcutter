// Copyright (c) 2017 Emilian Cioca
#include "Jewel3D/Precompiled.h"
#include "Timer.h"
#include "Logging.h"

#include <Windows.h>

namespace
{
	__int64 GetTimerResolution()
	{
		LARGE_INTEGER temp;
		if (QueryPerformanceFrequency(&temp))
		{
			return temp.QuadPart;
		}

		Jwl::Error("Timer: High performance timer is not supported by your CPU.");
		return 0;
	}

	static const __int64 ticksPerSecond = GetTimerResolution();
	static const __int64 ticksPerMS = ticksPerSecond / 1000;

	//- Casted ahead of time for a performance boost in GetElapsedMS() and GetElapsedSeconds().
	static const double d_ticksPerSecond = static_cast<double>(ticksPerSecond);
	static const double d_ticksPerMS = static_cast<double>(ticksPerMS);
}

namespace Jwl
{
	Timer::Timer()
	{
		Reset();
	}

	bool Timer::IsSupported()
	{
		return ticksPerSecond > 0;
	}

	__int64 Timer::GetTicksPerMS()
	{
		return ticksPerMS;
	}

	__int64 Timer::GetTicksPerSecond()
	{
		return ticksPerSecond;
	}

	__int64 Timer::GetCurrentTick()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		return currentTime.QuadPart;
	}

	void Timer::Reset()
	{
		startTime = GetCurrentTick();
	}

	double Timer::GetElapsedMS() const
	{
		return (GetCurrentTick() - startTime) / d_ticksPerMS;
	}

	double Timer::GetElapsedSeconds() const
	{
		return (GetCurrentTick() - startTime) / d_ticksPerSecond;
	}

	bool Timer::IsElapsedMS(double ms) const
	{
		return ms >= GetElapsedMS();
	}

	bool Timer::IsElapsedSeconds(double seconds) const
	{
		return seconds >= GetElapsedSeconds();
	}

	void Timer::SubtractTimeMS(double ms)
	{
		// Moving the start time forward effectively removes time.
		startTime += static_cast<__int64>(ms) / ticksPerMS;
	}

	void Timer::SubtractTimeSeconds(double seconds)
	{
		// Moving the start time forward effectively removes time.
		startTime += static_cast<__int64>(seconds) / ticksPerSecond;
	}

	void Timer::AddTimeMS(double ms)
	{
		// Moving the start time back effective adds more time.
		startTime -= static_cast<__int64>(ms) / ticksPerMS;
	}

	void Timer::AddTimeSeconds(double seconds)
	{
		// Moving the start time back effective adds more time.
		startTime -= static_cast<__int64>(seconds) / ticksPerSecond;
	}
}
