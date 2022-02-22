// Synchronizer.inl
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
// Synchronizer

inline void Synchronizer::waitSenseBodyAndEatAll()
{
	ASSERT(server().simulator_step() == server().sense_body_step());
	while(m_junction.eat() != INFORMATION_SENSE_BODY)
		;
	while(m_junction.wait(0))
		m_junction.eat();
}
inline void Synchronizer::waitSenseBodyAndSee()
{
	while(m_junction.eat() != INFORMATION_SEE)
		;
	while(m_junction.eat() != INFORMATION_SENSE_BODY)
		;
}

inline void Synchronizer::preConsume()
{
	if(m_sayBuffer.getBitsSize() > 0)
		m_junction.say(m_sayBuffer);
}
inline void Synchronizer::postConsume()
{
	m_sayBuffer.clear();
	waitSenseBodyAndEatAll();
}

inline Synchronizer::Synchronizer(int key) : m_junction(key)
{
}

inline int Synchronizer::countReceived() const
{
	return m_junction.countReceived();
}

inline void Synchronizer::addObserver(PlayerObserver* observer)
{
	m_junction.addObserver(observer);
}
inline void Synchronizer::removeObserver(PlayerObserver* observer)
{
	m_junction.removeObserver(observer);
}

inline bool Synchronizer::connect(const char* hostName, int port, const char* teamName)
{
	bool result = m_junction.connect(hostName, port, teamName);
#if MULTI_THREAD
	/*if(result)
		waitSenseBodyAndEatAll();*/
#endif
	return result;
}
inline bool Synchronizer::reconnect(const char* hostName, int port, const char* teamName, int uniformNumber)
{
	bool result = m_junction.reconnect(hostName, port, teamName, uniformNumber);
#if MULTI_THREAD
	/*if(result)
		waitSenseBodyAndEatAll();*/
#endif
	return result;
}

inline void Synchronizer::dash(double power)
{
	preConsume();
	m_junction.dash(power);
	postConsume();
}
inline void Synchronizer::turn(double moment)
{
	preConsume();
	m_junction.turn(moment);
	postConsume();
}
inline void Synchronizer::kick(double power, const Angle& direction)
{
	preConsume();
	m_junction.kick(power, direction);
	postConsume();
}
inline void Synchronizer::catchBall(const Angle& direction)
{
	preConsume();
	m_junction.catchBall(direction);
	postConsume();
}
inline void Synchronizer::move(const Vector& position)
{
	preConsume();
	m_junction.move(position);
	postConsume();
}
inline void Synchronizer::rest()
{
	preConsume();
	m_junction.rest();
	postConsume();
}

inline void Synchronizer::turnNeck(const Angle& angle)
{
	m_junction.turnNeck(angle);
}
inline void Synchronizer::changeView(ViewWidth width, ViewQuality quality)
{
	m_junction.changeView(width, quality);
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
