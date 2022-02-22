// Window.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(WINDOW_HXX__INCLUDED) // {
#define WINDOW_HXX__INCLUDED

#include "basic.hxx"
#include "Mutex.hxx"
#include "PlayerClient.hxx"
#include "Screen.hxx"

/////////////////////////////////////////////////////////////////////////////
// Window

/** システムの状態を表示するウィンドウ */
class Window
{
private:
	PlayerClient* m_clients[MAX_PLAYER*2];
	double m_distanceClientToBall[MAX_PLAYER*2];
	//typedef std::list<PlayerClient*> Clients;
	//Clients m_clients;
	PlayerClient* volatile m_observedClient;
	int m_observedIndex;
	bool m_destructing;
	
protected:
	Mutex m_mutex;
	
public:
	Window();
	virtual ~Window();
	
	void add(PlayerClient* client, int side = 0);
	void remove(PlayerClient* client);
	void set(int index);
	void update(const PlayerClient* client, double distance);
	
	virtual void go() = 0;
	virtual void updateScreen() = 0;
	
	void paint(Screen& screen);
	enum Key {
		KEY_LEFT,
			KEY_RIGHT,
			KEY_UP,
			KEY_DOWN,
			KEY_SPACE,
	};
	void keyPushed(Key key);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Window.inl"
#endif

#endif // } !defined(WINDOW_HXX__INCLUDED)
