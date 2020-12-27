#include "ckpch.h"
#include "Profiler.h"

namespace Cookie
{
	Vector<ProfileResult> Profiler::m_Results = Vector<ProfileResult>(1);
	uint32_t Profiler::m_Count = 0;
}