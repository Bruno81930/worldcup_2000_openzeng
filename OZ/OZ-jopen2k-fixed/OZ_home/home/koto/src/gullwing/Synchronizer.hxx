// Synchronizer.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SYNCHRONIZER_HXX__INCLUDED) // {
#define SYNCHRONIZER_HXX__INCLUDED

#include "PlayerObserver.hxx"
#include "PlayerJunction.hxx"
#include "SayBuffer.hxx"

/////////////////////////////////////////////////////////////////////////////
// Synchronizer

/**
* $B%W%l%$%d!<%/%i%$%"%s%H$N%5!<%P$H$NF14|$r<h$C$?DL?.$r4IM}$9$k!#%3%s%7%e!<%^$K(B
* $B$D$$$F!"Aw?.8e(B Sense Body $B>pJs$N<u?.$rBT$D0J30$O(B PlayerJunction $B$HF1MM$G$"$k!#(B
*/
class Synchronizer
{
private:
	Synchronizer(const Synchronizer&);
	Synchronizer& operator= (const Synchronizer&);
	bool operator== (const Synchronizer&) const;
	
private:
	PlayerJunction m_junction;
	SayBuffer m_sayBuffer;
	
protected:
	void waitSenseBodyAndEatAll();
	void waitSenseBodyAndSee();
	void preConsume();
	void postConsume();
	
public:
	virtual ~Synchronizer();
	Synchronizer(int key);
	
	int countReceived() const;
	
	void addObserver(PlayerObserver* observer);
	void removeObserver(PlayerObserver* observer);
	
	bool connect(const char* hostName, int port, const char* teamName);
	bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber);
	
	void dash(double power);
	void turn(double moment);
	void kick(double power, const Angle& direction);
	void catchBall(const Angle& direction);
	void move(const Vector& position);
	void rest();
	
	void turnNeck(const Angle& angle);
	template<class T> void say(T i)
	{
		m_sayBuffer.push(i);
	}
	template<class T> void say(T i, int bits)
	{
		m_sayBuffer.push(i, bits);
	}
	void changeView(ViewWidth width, ViewQuality quality);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Synchronizer.inl"
#endif

#endif // } !defined(SYNCHRONIZER_HXX__INCLUDED)
