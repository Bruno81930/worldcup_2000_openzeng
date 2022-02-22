// PlayerJunction.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif
#include "SayBuffer.hxx"

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerJunction::Command etc...

inline PlayerJunction::Command::Command()
{
}

inline PlayerJunction::Dash::Dash(double power)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
	m_power = power;
}
inline void PlayerJunction::Dash::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(dash %f)", m_power);
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Dash::cast(PlayerObserver& o) const
{
	o.dashed(m_power);
}

inline PlayerJunction::Turn::Turn(double moment)
{
	ASSERT(server().minmoment() <= moment && moment <= server().maxmoment());
	m_moment = moment;
}
inline void PlayerJunction::Turn::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(turn %f)", m_moment);
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Turn::cast(PlayerObserver& o) const
{
	o.turned(m_moment);
}

inline PlayerJunction::Kick::Kick(double power, const Angle& direction)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
	ASSERT(server().minmoment() <= direction.degrees() && direction.degrees() <= server().maxmoment());
	m_power = power;
	m_direction = direction;
}
inline void PlayerJunction::Kick::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(kick %f %f)", m_power, m_direction.degrees());
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Kick::cast(PlayerObserver& o) const
{
	o.kicked(m_power, m_direction);
}

inline PlayerJunction::Catch::Catch(const Angle& direction)
{
	ASSERT(server().minmoment() <= direction.degrees() && direction.degrees() <= server().maxmoment());
	m_direction = direction;
}
inline void PlayerJunction::Catch::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(catch %f)", m_direction.degrees());
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Catch::cast(PlayerObserver& o) const
{
	o.ballCaught(m_direction);
}

inline PlayerJunction::Move::Move(const Vector& position)
{
	m_position = position;
}
inline void PlayerJunction::Move::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(move %f %f)", m_position.x(), m_position.y());
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Move::cast(PlayerObserver& o) const
{
	o.moved(m_position);
}

inline void PlayerJunction::Rest::format(char* buffer, int size) const
{
#ifndef NDEBUG
	ASSERT(size >= 1);
	buffer[0] = '\0';
#else
	strcpy(buffer, "(dash -0)");
	size;
#endif
}
inline void PlayerJunction::Rest::cast(PlayerObserver& /*o*/) const
{
}


inline PlayerJunction::TurnNeck::TurnNeck(const Angle& angle)
{
	ASSERT(server().minneckmoment() <= angle.degrees() && angle.degrees() <= server().maxneckmoment());
	m_angle = angle;
}
inline void PlayerJunction::TurnNeck::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(turn_neck %f)", m_angle.degrees());
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::TurnNeck::cast(PlayerObserver& o) const
{
	o.neckTurned(m_angle);
}

inline PlayerJunction::Say::~Say()
{
	delete[] m_message;
}
inline PlayerJunction::Say::Say(const char* message)
{
	m_message = new char[strlen(message) + 1];
	strcpy(m_message, message);
}
inline PlayerJunction::Say::Say(const SayBuffer& buffer, int key, int time)
{
	const int size = server().say_msg_size() + 1;
	m_message = new char[size];
	buffer.toArray(m_message, size, time, key);
}

inline void PlayerJunction::Say::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(say %s)", m_message);
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::Say::cast(PlayerObserver& o) const
{
	o.said(m_message);
}

inline PlayerJunction::ChangeView::ChangeView(ViewWidth width, ViewQuality quality)
{
	m_width = width;
	m_quality = quality;
}
inline void PlayerJunction::ChangeView::format(char* buffer, int size) const
{
	int n = sprintf(buffer, "(change_view %s %s)",
		viewWidthToString(m_width), viewQualityToString(m_quality));
	n, size;	ASSERT(n < size);
}
inline void PlayerJunction::ChangeView::cast(PlayerObserver& o) const
{
	o.viewChanged(m_width, m_quality);
}

/////////////////////////////////////////////////////////////////////////////
// PlayerJunction

inline PlayerJunction::PlayerJunction(int key)
{
	m_key = key;
	m_teamName = 0;
	m_connected = false;
	m_playMode = PLAYMODE_BEFORE_KICK_OFF;
	m_countReceived = 0;
	m_refereeTimer = -1;
	m_say = 0;
	m_broadcasting = false;
	
#ifndef NDEBUG
	addObserver(&m_dummyObserver);
#endif
}

inline int PlayerJunction::countReceived() const
{
	return m_countReceived;
}

inline void PlayerJunction::addObserver(PlayerObserver* observer)
{
	ASSERT(!m_broadcasting);
	m_observersMutex.lock();
	m_observers.push_back(observer);
	if(m_connected) {
		bool goalie = (m_uniformNumber == 0);
		observer->initialize(m_playMode, m_uniformNumber, goalie);
	}
	m_observersMutex.unlock();
}
inline void PlayerJunction::removeObserver(PlayerObserver* observer)
{
	ASSERT(!m_broadcasting);
	m_observersMutex.lock();
	m_observers.remove(observer);
	m_observersMutex.unlock();
}

inline bool PlayerJunction::wait(long timeOut)
{
	ASSERT(timeOut >= -1);
	if(m_connection.wait(0))
		return true;
	if(timeOut == 0)
		return false;
	
	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->intoWaiting();
	m_broadcasting = false;
	m_observersMutex.unlock();
	
#ifdef NDEBUG
	int max = server().send_step() * 2 * 10;	// high quality wide １０回分の時間
#else
	int max = server().sense_body_step() * 10;	// sense_body 10回分の時間
#endif
	if(timeOut == -1 || timeOut > max)
		timeOut = max;
	bool exist = m_connection.wait(timeOut);
	
	m_observersMutex.lock();
	m_broadcasting = true;
	it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->outOfWaiting();
	m_broadcasting = false;
	m_observersMutex.unlock();
	
	if(exist)
		return true;
	
	if(timeOut < max)
		return false;
	
	report("PlayerJunction time-out\n");

	m_observersMutex.lock();
	m_broadcasting = true;
	it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->connectionBroken();
	m_broadcasting = false;
	m_observersMutex.unlock();
	
#if NEVER_THROW
	exit(1);
#else
	throw ConnectionException("time-out");
#endif
	return false;
}
inline InformationType PlayerJunction::eat()
{
	wait();
	
	const int size = BUFFER_SIZE;
	char buffer[size];
	if(m_connection.receive(buffer, size, 0)) {
		m_countReceived++;
		return onMessage(buffer);
	}
	
	ASSERT(false);
	return INFORMATION_ERROR;
}

inline InformationType PlayerJunction::onMessage(/*non-const*/ char* message)
{
	const static char sense_body[]  = "sense_body ";
	const static char see[]         = "see ";
	const static char hear[]        = "hear ";
	const static char error[]       = "error ";
	
	InformationType result = INFORMATION_ERROR;
	
	{
		while(isspace(*message))
			message++;
		char* p = message + strlen(message) - 1;
		while(p >= message && isspace(*p))
			*p-- = '\0';
	}
	
	char* p = message;
	ASSERT(*p == '(');/*)*/
	p++;
	ASSERT(strlen(p) >= 2);
	switch(p[2]) {
	case 'n':
		result = INFORMATION_SENSE_BODY;
		ASSERT(strncmp(p, sense_body, sizeof(sense_body)-1) == 0);
		p += sizeof(sense_body)-1;
		m_senseBodyInformation.initialize(p, m_teamName, m_teamSide);
		onSenseBody(m_senseBodyInformation);
		break;
	case 'e':
		result = INFORMATION_SEE;
		ASSERT(strncmp(p, see, sizeof(see)-1) == 0);
		p += sizeof(see)-1;
		m_seeInformation.initialize(p, m_teamName, m_teamSide);
		onSee(m_seeInformation);
		break;
	case 'a':
		result = INFORMATION_HEAR;
		ASSERT(strncmp(p, hear, sizeof(hear)-1) == 0);
		p += sizeof(hear)-1;
		m_hearInformation.initialize(p, m_teamSide);
		onHear(m_hearInformation);
		break;
	case 'r':
		ASSERT(strncmp(p, error, sizeof(error)-1) == 0);
		report("%s\n", message);
		break;
	default:
		ASSERT(false);
	}
	return result;
}
inline void PlayerJunction::onSenseBody(const PlayerSenseBody& sbi)
{
	m_refereeTimer = sbi.refereeTimer();

	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->received(sbi);
	m_broadcasting = false;
	m_observersMutex.unlock();
}
inline void PlayerJunction::onSee(const PlayerSee& si)
{
	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->received(si);
	m_broadcasting = false;
	m_observersMutex.unlock();
}
inline void PlayerJunction::onHear(const PlayerHear& hi)
{
	if(hi.senderType() == SENDER_REFEREE) {
		int pm = hi.refereeMessage();
		if(0 <= pm && pm < PLAYMODE_MAZ) {
			if(playModeHelper.isPlayModeChanged(m_playMode, (PlayMode)pm))
				m_playMode = (PlayMode)pm;
		}
	}
	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		(*it)->received(hi, m_key);
	m_broadcasting = false;
	m_observersMutex.unlock();
}
inline void PlayerJunction::broadcast(const Command& command)
{
	m_observersMutex.lock();
	m_broadcasting = true;
	Observers::iterator it = m_observers.begin();
	for(; it != m_observers.end(); it++)
		command.cast(**it);
	m_broadcasting = false;
	m_observersMutex.unlock();
}

inline void PlayerJunction::send(const Consumer& command)
{
	char buffer[BUFFER_SIZE];
	char *p = buffer;
	Commands::iterator it = m_commands.begin();
	for(; it != m_commands.end(); it++) {
		(*it)->format(p, &buffer[BUFFER_SIZE] - p);
		p += strlen(p);
		//broadcast(**it);
		delete *it;
	}
	m_commands.clear();
	
	command.format(p, &buffer[BUFFER_SIZE] - p);
	p += strlen(p);
	broadcast(command);

	if(m_say) {
		m_say->format(p, &buffer[BUFFER_SIZE] - p);
		broadcast(*m_say);
		delete m_say;
		m_say = 0;
	}

	if(buffer[0] != '\0')
		m_connection.send(buffer);
}
inline void PlayerJunction::push(Nonconsumer* command)
{
	broadcast(*command);
	m_commands.push_back(command);
}

inline void PlayerJunction::dash(double power)
{
	Dash command(power);
	send(command);
}
inline void PlayerJunction::turn(double moment)
{
	Turn command(moment);
	send(command);
}
inline void PlayerJunction::kick(double power, const Angle& direction)
{
	Kick command(power, direction);
	send(command);
}
inline void PlayerJunction::catchBall(const Angle& direction)
{
	Catch command(direction);
	send(command);
}
inline void PlayerJunction::move(const Vector& position)
{
	Move command(position);
	send(command);
}
inline void PlayerJunction::rest()
{
	Rest command;
	send(command);
}

inline void PlayerJunction::turnNeck(const Angle& angle)
{
	TurnNeck* command = new TurnNeck(angle);
	push(command);
}
inline void PlayerJunction::say(const SayBuffer& buffer)
{
	ASSERT(m_say == 0);
	Say* command = new Say(buffer, m_key, m_refereeTimer);
	m_say = command;
}
inline void PlayerJunction::say(const char* message)
{
	ASSERT(message);
	ASSERT(m_say == 0);
	Say* command = new Say(message);
	m_say = command;
}
inline void PlayerJunction::changeView(ViewWidth width, ViewQuality quality)
{
	ChangeView* command = new ChangeView(width, quality);
	push(command);
}


/////////////////////////////////////////////////////////////////////////////
#undef inline
