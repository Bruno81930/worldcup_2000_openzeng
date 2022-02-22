// PlayerJunction.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "PlayerJunction.hxx"

#ifndef NDEBUG
#  include "PlayerJunction.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerJunction

PlayerJunction::~PlayerJunction()
{
	delete[] m_teamName;
	Commands::iterator it = m_commands.begin();
	for(; it != m_commands.end(); it++) {
		delete *it;
	}
	m_commands.clear();
	delete m_say;
}

void PlayerJunction::initialize(int uniformNumber, const char* playModeString)
{
	int playMode = playModeHelper.toPlayMode(playModeString, m_teamSide);
	ASSERT(0 <= playMode && playMode <= PLAYMODE_MAZ);
	m_playMode = (PlayMode)playMode;
	m_uniformNumber = uniformNumber;
	bool goalie = (uniformNumber == 0);
	
	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->initialize(m_playMode, m_uniformNumber, goalie);
	m_broadcasting = false;
	m_observersMutex.unlock();
}

bool PlayerJunction::connect(const char* hostName, int port, const char* teamName)
{
	ASSERT(!m_connected);
	delete[] m_teamName;
	m_teamName = new char[strlen(teamName) + 1];
	strcpy(m_teamName, teamName);
	
	int progress = 0;
	for(; progress<2; progress++) {
		// 最初に goalie で試して、２回めは goalie なしで試す。
		// 成否が確定した次点で随時 return する。
#if !NEVER_THROW
		try {
#endif
			const int size = BUFFER_SIZE;
			char buffer[size];
			m_connection.initialize(hostName, port);
			int n;
			if(progress == 0)
				n = sprintf(buffer, "(init %s (version 5.17) (goalie))", teamName);
			else
				n = sprintf(buffer, "(init %s (version 5.17))", teamName);
			ASSERT(n < size);
			report("send: '%s'\n", buffer);
			m_connection.send(buffer);
			
			if(!m_connection.receive(buffer, size, server().sense_body_step() * 50)) {
				report("receive: time-out\n");
				return false;
			}
			report("receive: '%s'\n", buffer);
			static const char noMoreGoalie[] = "(error no_more_team_or_player_or_goalie)";
			if(strncmp(buffer, noMoreGoalie, sizeof(noMoreGoalie)-1) == 0)
				continue;
			else if(strncmp(buffer, "(init"/*)*/, 5) != 0)
				return false;
			
			char side[11];
			int uniformNumber;
			char playModeString[81];
			n = sscanf(buffer, "(init %10s %d %80[^"/*(*/")])", side, &uniformNumber, playModeString);
			if(n == EOF || n < 3)
				return false;
			uniformNumber--;
			
			if(strcmp(side, server().LEFT_STR()) == 0) {
				m_teamSide = SIDE_LEFT;
			} else if(strcmp(side, server().RIGHT_STR()) == 0) {
				m_teamSide = SIDE_RIGHT;
			} else {
				ASSERT(false);
			}
			
			ASSERT((progress == 0) == (uniformNumber == 0));	// キーパーは背番号１
			initialize(uniformNumber, playModeString);
			m_connected = true;
			return true;
#if !NEVER_THROW
		} catch(ConnectionException) {
			return false;
		}
#endif
	}
	return false;
}
bool PlayerJunction::reconnect(const char* hostName, int port, const char* teamName, int uniformNumber)
{
	ASSERT(!m_connected);
	delete[] m_teamName;
	m_teamName = new char[strlen(teamName) + 1];
	strcpy(m_teamName, teamName);
	
#if !NEVER_THROW
	try {
#endif
		const int size = BUFFER_SIZE;
		char buffer[size];
		m_connection.initialize(hostName, port);
		int n = sprintf(buffer, "(reconnect %s %d)", m_teamName, uniformNumber + 1);
		ASSERT(n < size);
		report("send: '%s'\n", buffer);
		m_connection.send(buffer);
		
		if(!m_connection.receive(buffer, size, server().recv_step() * 50))
			return false;
		report("receive: '%s'\n", buffer);
		if(strncmp(buffer, "(reconnect"/*)*/, 10) != 0)
			return false;
		
		char side[11];
		char playModeString[81];
		n = sscanf(buffer, "(reconnect %10s %80[^"/*(*/")])", side, playModeString);
		if(n == EOF || n < 2)
			return false;
		
		if(strcmp(side, server().LEFT_STR()) == 0) {
			m_teamSide = SIDE_LEFT;
		} else if(strcmp(side, server().RIGHT_STR()) == 0) {
			m_teamSide = SIDE_RIGHT;
		} else {
			ASSERT(false);
		}
		
		initialize(uniformNumber, playModeString);
#if !NEVER_THROW
	} catch(ConnectionException) {
	}
#endif
	return m_connected;
}
