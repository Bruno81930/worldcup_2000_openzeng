// Window.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Window

inline Window::Window()
{
	m_observedClient = 0;
	m_observedIndex = -1;
	m_destructing = false;
	
	for(int i=0; i<COUNT_OF(m_clients); i++)
		m_clients[i] = 0;
}

inline void Window::add(PlayerClient* client, int side)
{
	ASSERT(0 <= side && side <= 1);
	m_mutex.lock();
	int i;
	for(i=0; i<MAX_PLAYER; i++) {
		if(m_clients[i + side * MAX_PLAYER] == 0)
			break;
	}
	ASSERT(i < MAX_PLAYER);
	m_clients[i + side * MAX_PLAYER] = client;
	m_distanceClientToBall[i + side * MAX_PLAYER] = DBL_MAX;
	client->setWindow(this);
	m_mutex.unlock();
}
inline void Window::remove(PlayerClient* client)
{
	m_mutex.lock();
	for(int i=0; i<COUNT_OF(m_clients); i++) {
		if(m_clients[i] == client) {
			if(i == m_observedIndex)
				set(-1);
			client->removeWindow(this);
			m_clients[i] = 0;
			break;
		}
	}
	m_mutex.unlock();
}

inline void Window::update(const PlayerClient* client, double distance)
{
	m_mutex.lock();
	int i;
	for(i=0; i<COUNT_OF(m_clients); i++) {
		if(m_clients[i] == client)
			break;
	}
	ASSERT(i<COUNT_OF(m_clients));
	m_distanceClientToBall[i] = distance;
	m_mutex.unlock();
	
	if(client == m_observedClient)
		updateScreen();
}

inline void Window::paint(Screen& screen)
{
	if(m_observedClient)
		m_observedClient->paintClient(screen);
	else
		screen.paintField();
}

inline void Window::keyPushed(Key key)
{
	if(key == KEY_LEFT || key == KEY_RIGHT) {
		int direction = (key == KEY_LEFT) ? -1 : +1;
		int index = m_observedIndex;
		do {
			index = (index + direction + COUNT_OF(m_clients) + 1) % (COUNT_OF(m_clients) + 1);
		} while(index < COUNT_OF(m_clients) && m_clients[index] == 0);
		if(index >= COUNT_OF(m_clients))
			set(-1);
		else
			set(index);
		return;
	} else if(key == KEY_UP || key == KEY_DOWN || key == KEY_SPACE) {
		int i, j;
		int base = (m_observedIndex < MAX_PLAYER) ? 0 : MAX_PLAYER;
		
		// リストアップする。
		int table[MAX_PLAYER];
		int maz = 0;
		for(i=0; i<MAX_PLAYER; i++) {
			if(m_clients[base+i] != 0) {
				table[maz++] = base+i;
			}
		}
		if(maz == 0)
			return;
		
		// ソートする。
		for(i=0; i<maz; i++) {
			int x = table[i];
			for(j=i-1; j>=0 && m_distanceClientToBall[table[j]]>m_distanceClientToBall[x]; j--)
				table[j+1] = table[j];
			table[j+1] = x;
		}
		
		// 「次」のものを探す。
		for(i=0; i<maz; i++) {
			if(table[i] == m_observedIndex)
				break;
		}
		if(key != KEY_SPACE && i < maz) {
			int direction = (key == KEY_DOWN) ? -1 : +1;
			i = (i + direction + maz) % maz;
		} else {
			i = 0;
		}
		set(table[i]);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
