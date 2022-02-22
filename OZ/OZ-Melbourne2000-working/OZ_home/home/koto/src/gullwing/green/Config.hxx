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
	double reachableByKick() const;		// おおざっぱに、蹴りが届く範囲。パス先の候補をリストアップするのに使う。
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
