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
* プレイヤークライアントのサーバとの同期を取った通信を管理する。コンシューマに
* ついて、送信後 Sense Body 情報の受信を待つ以外は PlayerJunction と同様である。
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
