// Green.inl
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
// Green

inline void GreenBall::setVelocity(const Vector& vel)
{
	m_velocity = vel;
	m_safeVelocity = vel;
}

inline void GreenBall::average(const GreenSelf& self, int time, const PlayerSeeObject& o)
{
	Vector position = m_position;
	Vector velocity = m_velocity;
	GreenBall::update(self, time, o);
	if(o.distance().valid() && o.distance() > server().visible_distance()) {
		/*if(position.valid() && m_position.valid())
			m_position = (position + m_position) / 2;*/
		if(velocity.valid() && m_velocity.valid())
			m_velocity = (velocity + m_velocity) / 2;
	}
}
inline void GreenBall::step()
{
	ASSERT(false);
}
inline void GreenBall::step(const GreenSelf& self)
{
	Vector position = m_position;
	WhiteBall::step();
	
	double d = (m_position - self.position()).norm();
	if(d <= size() + self.size()) {
		if(d != 0.0) {
			Vector sub = m_safeVelocity;
			if(sub != Vector(0, 0))
				sub.normalize(d);
			m_position -= sub;
		}
		m_safeVelocity *= -0.1;
		if(m_velocity.valid())
			m_velocity = m_safeVelocity;
	}
}


inline Green::Green()
{
	m_config = 0;
}
inline Config& Green::config()
{
	ASSERT(m_config);
	return *m_config;
}
inline void Green::setConfig(Config* config)
{
	m_config = config;
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
