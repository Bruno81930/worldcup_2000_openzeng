// Gullwing.inl
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
// Gullwing

inline GullwingPremise::GullwingPremise(Gullwing* brain) : Premise(brain)
{
	m_brain = brain;
}

inline const Config& Gullwing::config()
{
	return m_config;
}

inline double Gullwing::enemyOffside() const
{
	return m_enemyOffside;
}
inline double Gullwing::ourOffside() const
{
	return m_ourOffside;
}
inline const Vector& Gullwing::toBall() const
{
	return m_toBall;
}
inline double Gullwing::distanceToBall() const
{
	return m_distanceToBall;
}
inline const Angle& Gullwing::directionToBall() const
{
	return m_directionToBall;
}
inline const Vector& Gullwing::toNextBall() const
{
	return m_toNextBall;
}
inline double Gullwing::distanceToNextBall() const
{
	return m_distanceToNextBall;
}
inline const Angle& Gullwing::directionToNextBall() const
{
	return m_directionToNextBall;
}

inline void Gullwing::passTo(const Vector& target)
{
	ASSERT(target.valid());
	Vector ballToTarget = target - ball().position();
	if(ballToTarget == Vector(0, 0))
		return;
	double length = ballToTarget.norm();
	double speed = server().kickable_area();	// $BE~C#;~$NB.EY$G=i4|2=(B
	while(length >= 0) {
		length -= speed;
		speed /= server().ball_decay();
	}
	// $B$3$N;~E@$G!"(Bspeed $B$O=3$C$?D>8e$K$=$&$"$k$Y$-B.$5(B
	Vector acc = ballToTarget;
	acc.normalize(speed);		// $B=3$C$?D>8e$K$=$&$"$k$Y$-B.EY(B
	acc -= ball().safeVelocity();	// $B=3$j$K$h$C$FM?$($k$Y$-2CB.EY(B
	
	// $B2CB.EY(B acc $B$r%\!<%k$XM?$($k$h$&$K=3$k(B
	kick(acc);

	// TODO:$B<+J,$NNO$r$3$($k>l9g!"(BkickTo(const Angle& direction) $B$r8F$S=P$9!#(B
}
inline void Gullwing::kickTo(const Angle& direction)
{
	ASSERT(direction.valid());
	kick(server().maxpower(),
		direction - self().bodyDirection());

	// TODO:$B8=:_$NB.EY$b9MN8$N$3$H!#(B
}
inline void Gullwing::catchIn(const Angle& direction)
{
	catchBall(direction - self().bodyDirection());
}

inline bool Gullwing::isInPitch(const Vector& v)
{
	return abs(v.x()) < server().PITCH_LENGTH()/2 && abs(v.y()) < server().PITCH_WIDTH()/2;
}
inline bool Gullwing::isInMyPenaltyArea(const Vector& v)
{
	double x = v.x() + server().PITCH_LENGTH()/2;
	double y = v.y();
	return 0 < x && x < server().PENALTY_AREA_LENGTH()
		&& abs(y) < server().PENALTY_AREA_WIDTH()/2;
}
inline bool Gullwing::isInMyGoalArea(const Vector& v)
{
	double x = v.x() + server().PITCH_LENGTH()/2;
	double y = v.y();
	return 0 < x && x < server().GOAL_AREA_LENGTH()
		&& abs(y) < server().GOAL_AREA_WIDTH()/2;
}
inline bool Gullwing::isInMyHalfGoalArea(const Vector& v)
{
	double x = v.x() + server().PITCH_LENGTH()/2;
	double y = v.y();
	return 0 < x && x < server().GOAL_AREA_LENGTH()/2
		&& abs(y) < server().GOAL_AREA_WIDTH()/4;
}

inline bool Gullwing::isInEnemyPenaltyArea(const Vector& v)
{
	return isInMyPenaltyArea(Vector(-v.x(), v.y()));
}

inline double Gullwing::minOthers(Side side, Distance distance, int direction) const
{
	ASSERT(side == SIDE_LEFT || side == SIDE_RIGHT);
	ASSERT(0 <= direction && direction < sizeWorkArea);
	if(side == SIDE_LEFT) {
		return m_minOthers[LEFT][distance][direction];
	} else {
		return m_minOthers[RIGHT][distance][direction];
	}
}
inline double Gullwing::maxOthers(Side side, Distance distance, int direction) const
{
	ASSERT(side == SIDE_LEFT || side == SIDE_RIGHT);
	ASSERT(0 <= direction && direction < sizeWorkArea);
	if(side == SIDE_LEFT) {
		return m_maxOthers[LEFT][distance][direction];
	} else {
		return m_maxOthers[RIGHT][distance][direction];
	}
}
inline double Gullwing::minOthers(Side side, Distance distance) const
{
	ASSERT(side == SIDE_LEFT || side == SIDE_RIGHT);
	if(side == SIDE_LEFT) {
		return m_minOthers[LEFT][distance][sizeWorkArea];
	} else {
		return m_minOthers[RIGHT][distance][sizeWorkArea];
	}
}
inline double Gullwing::maxOthers(Side side, Distance distance) const
{
	ASSERT(side == SIDE_LEFT || side == SIDE_RIGHT);
	if(side == SIDE_LEFT) {
		return m_maxOthers[LEFT][distance][sizeWorkArea];
	} else {
		return m_maxOthers[RIGHT][distance][sizeWorkArea];
	}
}
inline double Gullwing::lineOffside() const
{
	return m_lineOffside;
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
