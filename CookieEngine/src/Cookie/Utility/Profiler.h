#pragma once

#include "Cookie/Common/Vector.h"
#include "Cookie/Utility/Timer.h"

#define PROFILE_SCOPE(name) Cookie::ProfileTimer timer##__LINE__(name, [&](Cookie::ProfileResult result) { Cookie::Profiler::PushResult(result); })

namespace Cookie
{

	struct ProfileResult
	{
		const char* Name;
		double Duration;
	};

	template<typename Fn>
	struct ProfileTimer
	{

		const char* Name;
		bool bStopped;
		double StartTime;
		Fn Func;

		ProfileTimer(const char* name, Fn&& func)
			: Name(name), bStopped(false), Func(func)
		{
			StartTime = GetCurrentTime();
		}

		~ProfileTimer()
		{
			if (!bStopped)
				StopTimer();
		}
		
		void StopTimer()
		{
			double endTime = GetCurrentTime();
			bStopped = true;
			double duration = (endTime - StartTime) * 1000.0;
			Func({ Name, duration });
		}

	};

	class Profiler
	{
	public:
		static Vector<ProfileResult> m_Results;
		static uint32_t m_Count;

		static void PushResult(ProfileResult result)
		{
			Profiler::m_Results.Insert(result, m_Count++);
		}
	};

}