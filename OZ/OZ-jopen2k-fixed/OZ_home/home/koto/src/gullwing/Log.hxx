// Log.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(LOG_HXX__INCLUDED) // {
#define LOG_HXX__INCLUDED

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// Log

// 未完
class Log
{
private:
	Log(const Log&);
	Log& operator= (const Log&);
	bool operator== (const Log&) const;
	
protected:
	enum { REC_VERSION_2 = 2 };
	typedef struct {
		short	enable ;
		short	side ;
		short	unum ;
		short	angle ;
		short	x ;
		short	y ;
	} pos_t ;
	typedef struct {
		char	name[16] ;
		short	score ;
	} team_t ;
	typedef struct {
		char	pmode ;
		team_t	team[2] ;
		pos_t	pos[MAX_PLAYER * 2 + 1] ;
		short	time ;
	} showinfo_t ;
	typedef enum {
		PM_Null,
		PM_BeforeKickOff,
		PM_TimeOver,
		PM_PlayOn,
		PM_KickOff_Left,
		PM_KickOff_Right,
		PM_KickIn_Left,
		PM_KickIn_Right,
		PM_FreeKick_Left,
		PM_FreeKick_Right,
		PM_CornerKick_Left,
		PM_CornerKick_Right,
		PM_GoalKick_Left,
		PM_GoalKick_Right,
		PM_AfterGoal_Left,
		PM_AfterGoal_Right,
		PM_Drop_Ball,
		PM_OffSide_Left,
		PM_OffSide_Right,
		PM_MAX 
	} PlayMode ;
	
	FILE* m_fp;
	showinfo_t m_showinfo;
	
public:
	virtual ~Log();
	Log();
	bool open(char const* logName);
	void reset();
	void set(int time);
	void set(int index, Side side, Integer uniformNumber, Vector position, Angle angle, bool kick=false);
	void write();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Log.inl"
#endif

#endif // } !defined(LOG_HXX__INCLUDED)
