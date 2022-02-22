// Config.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CONFIG_HXX__INCLUDED) // {
#define CONFIG_HXX__INCLUDED

#include "../Log.hxx"

/////////////////////////////////////////////////////////////////////////////
// Config

class Config
{
private:
	Config(const Config&);
	Config& operator= (const Config&);
	bool operator== (const Config&) const;

protected:
	FILE* m_logFile;
	Log m_log;
	double m_reachableByKick;

public:
	bool openLogFile(const char* logName, const char* textLogName);
	FILE* logFile();
	Log& log();
	static double cyclesUntilSlow(double speed);
	static double mobilityRatioUntilSlow(double speed);
	double reachableByKick() const;		// $B$*$*$6$C$Q$K!"=3$j$,FO$/HO0O!#%Q%9@h$N8uJd$r%j%9%H%"%C%W$9$k$N$K;H$&!#(B
	//double powerKickTo(double length) const;

	~Config();
	Config();

	void initialize(const std::map<std::string, std::string>& parameters);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Config.inl"
#endif

#endif // } !defined(CONFIG_HXX__INCLUDED)
