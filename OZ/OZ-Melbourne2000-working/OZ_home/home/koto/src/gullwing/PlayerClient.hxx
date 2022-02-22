// PlayerClient.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERCLIENT_HXX__INCLUDED) // {
#define PLAYERCLIENT_HXX__INCLUDED

class Window;
class Screen;

/////////////////////////////////////////////////////////////////////////////
// PlayerClient

/**
* $B%W%l%$%d!<%/%i%$%"%s%H(B
*/
class PlayerClient
{
private:
	
protected:
	Window* m_window;
	
public:
	virtual ~PlayerClient();
	PlayerClient();
	
	/** $B%/%i%$%"%s%H$N>uBV$rI=<($9$k%&%#%s%I%&$r@_Dj$9$k!#(B*/
	virtual void setWindow(Window* window);
	/** $B%/%i%$%"%s%H$N>uBV$rI=<($9$k%&%#%s%I%&$r2r=|$9$k!#(B*/
	virtual void removeWindow(Window* window);
	/** $B%/%i%$%"%s%H$N>uBV$r(B screen $B$XI=<($9$k!#(B*/
	virtual void paintClient(Screen& screen) = 0;
	
	/**
	* $B%5!<%P$X(B init $B%3%^%s%I$G@\B3$9$k!#:G=i$K%4!<%k%-!<%Q!<$H$7$F@\B3$r;n$_$F!"(B
    * $B<:GT$7$?$iIaDL$N%W%l%$%d!<$H$7$F$NEPO?$r;n$_$k!#(B
	*/
	virtual bool connect(const char* hostName, int port, const char* teamName) = 0;
	/**
	* $B%5!<%P$X(B reconnect $B%3%^%s%I$G@\B3$9$k!#(B
	*/
	virtual bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber) = 0;
	/**
	* $B%/%i%$%"%s%H$r<B9T$9$k!#(B
	*/
	virtual void go() = 0;
};

/**
* $B%G%U%)%k%H$N%A!<%`L>$rJV$9!#(B
*/
const char* getDefaultTeamName();

/**
* $B?7$7$$(B PlayerClient $B$r(B new $B$7$FJV$9!#(B
*/
PlayerClient* newPlayerClient(const std::map<std::string, std::string>& parameters);

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerClient.inl"
#endif

#endif // } !defined(PLAYERCLIENT_HXX__INCLUDED)
