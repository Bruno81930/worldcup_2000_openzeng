// PlayerObserver.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYEROBSERVER_HXX__INCLUDED) // {
#define PLAYEROBSERVER_HXX__INCLUDED

#include "basic.hxx"
class PlayerSenseBody;
class PlayerSee;
class PlayerHear;

/////////////////////////////////////////////////////////////////////////////
// PlayerObserver

/**
* $B%W%l%$%d!<%/%i%$%"%s%H$H$NDL?.$r4F;k$9$k%*%V%8%'%/%H!#Nc30$r(B throw $B$7$F$O$J$i$J$$!#(B
*/
class PlayerObserver
{
public:
	virtual ~PlayerObserver();
	PlayerObserver();
	
	virtual void intoWaiting();
	virtual void outOfWaiting();
	
	virtual void initialize(PlayMode playMode, int uniformNumber, bool goalie);
	virtual void connectionBroken();
	
	virtual void received(const PlayerSenseBody& sbi);
	virtual void received(const PlayerSee& si);
	virtual void received(const PlayerHear& hi, int key);
	
	virtual void turned(double moment);
	virtual void dashed(double power);
	virtual void kicked(double power, Angle direction);
	virtual void ballCaught(Angle direction);
	virtual void moved(Vector position);

	virtual void neckTurned(Angle angle);
	virtual void said(const char* m_message);
	virtual void viewChanged(ViewWidth width, ViewQuality quality);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerObserver.inl"
#endif

#endif // } !defined(PLAYEROBSERVER_HXX__INCLUDED)
