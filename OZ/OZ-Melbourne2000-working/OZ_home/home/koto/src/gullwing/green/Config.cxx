// Config.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "../common.hxx"
#include "Config.hxx"

#ifndef NDEBUG
#  include "Config.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Config

void Config::initialize(const std::map<std::string, std::string>& parameters)
{
	double initial = server().maxpower() * server().kick_power_rate();
	initial = min(initial * 2, server().ball_speed_max());
	double ratio = mobilityRatioUntilSlow(initial);
	m_reachableByKick = ratio * initial;

	m_reachableByKick *= 2;	// 仮
}
