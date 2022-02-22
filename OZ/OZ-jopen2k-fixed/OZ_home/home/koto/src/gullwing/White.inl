// White.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif
#include "PlayerSee.hxx"
#include "PlayerSenseBody.hxx"

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif

/////////////////////////////////////////////////////////////////////////////
// WhiteObject

inline int WhiteObject::time() const
{
	return m_time;
}
inline const Vector& WhiteObject::position() const
{
	return m_position;
}
inline const Vector& WhiteObject::velocity() const
{
	return m_velocity;
}
inline Vector WhiteObject::safeVelocity() const
{
	ASSERT(m_safeVelocity.valid());
	return m_safeVelocity;
}
inline const Vector& WhiteObject::acceleration() const
{
	return m_acceleration;
}
inline Vector WhiteObject::safeAcceleration() const
{
	if(m_acceleration.valid())
		return m_acceleration;
	return Vector(0, 0);
}

inline void WhiteObject::initialize(int time, const Vector& position,
									const Vector& velocity, const Vector& acceleration)
{
	m_time = time;
	m_position = position;
	m_velocity = velocity;
	m_safeVelocity = !velocity.valid() ? Vector(0, 0) : velocity;
	m_acceleration = acceleration;
}
inline void WhiteObject::initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	m_position = Vector::invalidValue();
	m_velocity = Vector::invalidValue();
	m_safeVelocity = Vector(0, 0);
	m_acceleration = Vector::invalidValue();
	
	update(self, time, o);
}
inline void WhiteObject::update(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	m_time = time;
	
	// 位置を計算する。
	Vector toObject;
	if(o.distance().valid()) {
		toObject = Vector(o.distance(), Angle(o.direction() + self.sightDirection()));
		m_position = self.position() + toObject;
	}
	
	// 速度を計算する。
	if(o.distanceChange().valid()) {
#if 1
		ASSERT(o.distance().valid());
		double r = o.distance();
		if(r != 0.0) {
			Vector e = toObject / r;
			double x = o.distanceChange() * e.x() - o.directionChange() * (PI/180) * r * e.y();
			double y = o.distanceChange() * e.y() + o.directionChange() * (PI/180) * r * e.x();
			m_safeVelocity = m_velocity = Vector(x, y) + self.safeVelocity();
		}
#else
		Vector distant = Vector(o.distanceChange(), self.sightDirection());
		double radians = o.directionChange() * (PI / 180);
		Vector lateral = Vector(o.distance() * radians, self.sightDirection() + Angle(90));
		m_safeVelocity = m_velocity = distant + lateral + self.safeVelocity();
#endif
	} else {
		m_velocity = Vector::invalidValue();
	}
}
inline void WhiteObject::step()
{
	if(m_acceleration.valid()) {
		m_safeVelocity += m_acceleration;
		if(m_safeVelocity.norm() > maxSpeed())
			m_safeVelocity.normalize(maxSpeed());
	}
	if(m_position.valid()) {
		m_position += m_safeVelocity;
	}
	m_safeVelocity *= decay();
	
	if(m_velocity.valid()) {
		m_velocity = m_safeVelocity;
	}
	m_acceleration = Vector(0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// WhiteBall

inline void WhiteBall::initialize(int time, const Vector& position, const Vector& velocity,
								  const Vector& acceleration)
{
	WhiteObject::initialize(time, position, velocity, acceleration);
}
inline void WhiteBall::initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	WhiteObject::initialize(self, time, o);
}
inline void WhiteBall::update(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	WhiteObject::update(self, time, o);
}
inline void WhiteBall::step()
{
	WhiteObject::step();
}

inline void WhiteBall::kicked(const WhiteSelf& self, double power, Angle direction)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
	ASSERT(server().minmoment() <= direction.degrees() && direction.degrees() <= server().maxmoment());
	Vector selfToBall = this->position() - self.position();
	if(selfToBall.norm() > server().kickable_area())
		return;		// 空振り
	
	power *= server().kick_power_rate();
	double radians = abs(selfToBall.angle() - self.bodyDirection()).radians();
	double gap = selfToBall.norm() - server().ball_size() - server().player_size();
	power *= (1 - 0.25*radians/PI - 0.25*gap/server().kickable_margin()) ;
	m_acceleration = Vector(power, direction + self.bodyDirection());
}

/////////////////////////////////////////////////////////////////////////////
// WhitePlayer

inline const Angle& WhitePlayer::bodyDirection() const
{
	return m_bodyDirection;
}
inline const Angle& WhitePlayer::neckAngle() const
{
	return m_neckAngle;
}
inline Angle WhitePlayer::sightDirection() const
{
	return m_bodyDirection + m_neckAngle;
}
inline Side WhitePlayer::team() const
{
	return m_team;
}
inline Integer WhitePlayer::uniformNumber() const
{
	return m_uniformNumber;
}

inline void WhitePlayer::initialize(Side team, const Integer& uniformNumber,
									int time, const Vector& position, const Vector& velocity,
									const Vector& acceleration, Angle bodyDirection, Angle neckAngle)
{
	WhiteObject::initialize(time, position, velocity, acceleration);
	m_bodyDirection = bodyDirection;
	m_neckAngle = neckAngle;
	m_team = team;
	m_uniformNumber = uniformNumber;
}
inline void WhitePlayer::initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	WhiteObject::initialize(self, time, o);
	
	m_bodyDirection = Angle::invalidValue();
	m_neckAngle = Angle::invalidValue();
	// 体の向きを計算する
	if(o.bodyDirection().valid()) {
		m_bodyDirection = o.bodyDirection() + self.sightDirection();
		m_neckAngle = (o.faceDirection() + self.sightDirection()) - m_bodyDirection;
	}
	
	m_team = o.team();
	m_uniformNumber = o.uniformNumber();
}
inline void WhitePlayer::update(const WhiteSelf& self, int time, const PlayerSeeObject& o)
{
	WhiteObject::update(self, time, o);
	
	// 体の向きを計算する
	if(o.bodyDirection().valid()) {
		m_bodyDirection = o.bodyDirection() + self.sightDirection();
		m_neckAngle = (o.faceDirection() + self.sightDirection()) - m_bodyDirection;
	}

	m_team = o.team();
	m_uniformNumber = o.uniformNumber();
}
inline void WhitePlayer::step()
{
	WhiteObject::step();
}

/////////////////////////////////////////////////////////////////////////////
// WhiteSelf

inline bool WhiteSelf::isGoalie() const
{
	return m_goalie;
}
inline ViewQuality WhiteSelf::viewQuality() const
{
	return m_viewQuality;
}
inline ViewWidth WhiteSelf::viewWidth() const
{
	return m_viewWidth;
}
inline double WhiteSelf::viewWidthDegrees() const
{
	return viewWidthToDegrees(m_viewWidth);
}
inline double WhiteSelf::stamina() const
{
	return m_stamina;
}
inline double WhiteSelf::effort() const
{
	return m_effort;
}
inline double WhiteSelf::timeLastCatch() const
{
	return m_timeLastCatch;
}

inline void WhiteSelf::initialize(int uniformNumber, bool goalie, int time,
								  const Vector& position, const Vector& velocity,
								  const Vector& acceleration, Angle bodyDirection, Angle neckAngle)
{
	WhitePlayer::initialize(SIDE_LEFT, uniformNumber, time, position, velocity, acceleration, bodyDirection, neckAngle);
	m_goalie = goalie;
	m_viewQuality = VIEW_QUALITY_HIGH;
	m_viewWidth = VIEW_WIDTH_NORMAL;
	m_stamina = server().stamina_max();
	m_effort = 1.0;
	m_turnAngle = Angle(0);
	m_neckTurnAngle = Angle(0);
	m_timeLastCatch = -1;
}
inline void WhiteSelf::step()
{
	m_bodyDirection += m_turnAngle;
	m_turnAngle = Angle(0);
	m_neckAngle += m_neckTurnAngle;
	m_neckTurnAngle = Angle(0);
	WhitePlayer::step();
}

inline void WhiteSelf::update(const PlayerSenseBody& sbi)
{
	m_viewQuality = sbi.viewQuality();
	m_viewWidth = sbi.viewWidth();
	m_stamina = sbi.stamina();
	m_effort = sbi.effort();
	double speed = sbi.speed();
	if(!m_velocity.valid()) {
		m_velocity = Vector(speed, bodyDirection());
	} else {
		double oldSpeed = m_velocity.norm();
		if(abs(oldSpeed - speed) > 0.005) {
			if(speed <= oldSpeed * 0.2)
				m_velocity.normalize(-speed);	// おそらく何かに衝突した
			else if(oldSpeed == 0)
				m_velocity = Vector(speed, bodyDirection());
			else
				m_velocity.normalize(speed);
		}
	}
	m_neckAngle = sbi.neckAngle();
}
inline void WhiteSelf::updateSightDirection(Angle direction)
{
	m_bodyDirection = direction - m_neckAngle;
}
inline void WhiteSelf::updatePosition(Vector position)
{
	m_position = position;
}
inline void WhiteSelf::updateVelocity(Vector velocity)
{
	m_velocity = velocity;
}
inline void WhiteSelf::updateTime(int time)
{
	m_time = time;
}

inline void WhiteSelf::dashed(double power)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
	double power_need = (power > 0) ? power : (power * 2);
	if(power_need > stamina())
		power_need = stamina();
	m_stamina -= power_need;
	power = (power > 0) ? power_need : (power_need / -2);
	power *= effort() * server().dash_power_rate();
	m_acceleration = Vector(power, m_bodyDirection);
}
inline void WhiteSelf::turned(double moment)
{
	ASSERT(server().minmoment() <= moment && moment <= server().maxmoment());
	double degrees = moment / (1.0 + server().inertia_moment() * safeVelocity().norm());
	//ASSERT(m_turnAngle == Angle(0));
	m_turnAngle += Angle(degrees);
}
inline void WhiteSelf::neckTurned(Angle angle)
{
	ASSERT(server().minneckmoment() <= angle.degrees() && angle.degrees() <= server().maxneckmoment());
	m_neckTurnAngle += angle;
/*#ifndef NDEBUG
	double degrees = (m_neckAngle + m_neckTurnAngle).degrees();
	ASSERT(server().minneckang() <= degrees && degrees <= server().maxneckang());
#endif*/
}
inline void WhiteSelf::ballCaught(int time, Angle /*direction*/)
{
	m_timeLastCatch = time;
}
inline void WhiteSelf::moved(Vector position)
{
	m_position = position;
}
inline void WhiteSelf::viewChanged(ViewWidth width, ViewQuality quality)
{
	m_viewWidth = width;
	m_viewQuality = quality;
}

/////////////////////////////////////////////////////////////////////////////
// White

inline White::White()
{
}

/*
inline White::White()
{
}

  inline void White::lock()
  {
  m_mutex.lock();
  }
  inline void White::unlock()
  {
  m_mutex.unlock();
  }
  
	inline PlayerObserver& White::asObserver()
	{
	return *this;
	}
	
	  inline White::iterator White::begin()
	  {
	  return end() - m_countOfPlayers;
	  }
	  inline White::const_iterator White::begin() const
	  {
	  return end() - m_countOfPlayers;
	  }
	  inline White::iterator White::end()
	  {
	  return m_players + COUNT_OF(m_players);
	  }
	  inline White::const_iterator White::end() const
	  {
	  return m_players + COUNT_OF(m_players);
	  }
	  
		inline int White::time() const
		{
		return m_time;
		}
		inline PlayMode White::playMode() const
		{
		return m_playMode;
		}
		inline const WhiteSelf& White::self() const
		{
		return m_self;
		}
		inline const WhiteBall& White::ball() const
		{
		return m_ball;
		}
		inline const Vector* White::flags() const
		{
		return m_flags;
		}
		
		  inline void White::step()
		  {
		  m_self.step();
		  m_ball.step();
		  iterator it = begin();
		  for(; it != end(); it++)
		  (*it).step();
}*/

/////////////////////////////////////////////////////////////////////////////
#undef inline
