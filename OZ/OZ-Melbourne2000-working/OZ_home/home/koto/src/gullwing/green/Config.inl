// Config.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#include "../Server.hxx"

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Config

inline Config::~Config()
{
	if(m_logFile)
		fclose(m_logFile);
}

inline Config::Config()
{
	m_logFile = 0;
	m_reachableByKick = 0;
}

inline bool Config::openLogFile(const char* logName, const char* textLogName)
{
	if(!m_log.open(logName))
		return false;

	if(m_logFile)
		fclose(m_logFile);
	m_logFile = fopen(textLogName, "wt");
	return (m_logFile != 0);
}
inline FILE* Config::logFile()
{
	return m_logFile;
}

inline Log& Config::log()
{
	return m_log;
}

inline double Config::cyclesUntilSlow(double speed)
{
	double target = server().kickable_area();
	double cycles = ::log(target/speed) / ::log(server().ball_decay());
	if(cycles < 0)
		cycles = 0;
	return cycles;
}
inline double Config::mobilityRatioUntilSlow(double speed)
{
	double cycles = cyclesUntilSlow(speed);
	double result = 0.0;
	double v = 1.0;
	for(int i=0; i<cycles; i++) {
		result += v;
		v *= server().ball_decay();
	}
	if(result == 0.0)
		return 0.00001;
	return result;
}
inline double Config::reachableByKick() const
{
	return m_reachableByKick;
}

/*inline SimpleNN<3, 3>& Config::shortMatchNN()
{
	return m_shortMatchNN;
}*/

/*inline double Config::powerKickTo(double length) const
{
	double speed = server().kickable_area();	// 到達時の速度で初期化
	ASSERT(speed > 0);
	while(length >= 0) {
		length -= speed;
		speed /= server().ball_decay();
	}
	double power = speed / server().kick_power_rate();
	return min(power, server().maxpower());
}*/


/////////////////////////////////////////////////////////////////////////////
#undef inline
