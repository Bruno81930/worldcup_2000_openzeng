// Gullwing.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "../common.hxx"
#include "Gullwing.hxx"

#ifndef NDEBUG
#  include "Gullwing.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Gullwing

const char* getDefaultTeamName()
{
	return "Gullwing1_7";
}

PlayerClient* newPlayerClient(const std::map<std::string, std::string>& parameters)
{
	std::map<std::string, std::string>::const_iterator it = parameters.find("key");
	int key = (it == parameters.end()) ? 0 : atoi(it->second.c_str());
	return new Gullwing(key, parameters);
}

Gullwing::~Gullwing()
{
}

Gullwing::Gullwing(int key, const std::map<std::string, std::string>& parameters) : Monobrain<Green>(key)
{
	addObserver(this);

	m_config.initialize(parameters);
	m_mode = MODE_NORMAL;
	m_timePlayModeChanged = 0;
	m_timeCaught = 0;
	m_freeCount = 0;

	m_enemyOffside = 0;
	m_toBall = Vector(0, 0);
	m_distanceToBall = 0.0;
	m_directionToBall = Angle(0);
	m_toNextBall = Vector(0, 0);
	m_distanceToNextBall = 0.0;
	m_directionToNextBall = Angle(0);

	m_moveTarget = Vector::invalidValue();
	m_moveTimeLimit = 0;

	

	ASSERT(sizeof(m_minOthersMaster) == sizeof(m_maxOthersMaster));
	for(int i=0; i<COUNT_OF(m_minOthersMaster); i++) {
		for(int j=0; j<COUNT_OF(m_minOthersMaster[i]); j++) {
			for(int k=0; k<COUNT_OF(m_minOthersMaster[i][j]); k++) {
				m_minOthersMaster[i][j][k] = DBL_MAX;
				m_maxOthersMaster[i][j][k] = 0;
			}
		}
	}
}

void Gullwing::paintClient(Screen& screen)
{
	screen.paintField();
	m_world.lock();
	{
		Screen& s = screen;
		
		// $BNL;R2=%a%C%7%e$rI=<((B
		/*{
			int i;
			s.color(s.colorDark());
			for(i=0; i<360; i++) {
				double x = self().position().x();
				double y = self().position().y();
				if((i % 5) != 0) {
					x += server().PITCH_WIDTH()/2 * cos(Angle(i));
					y += server().PITCH_WIDTH()/2 * sin(Angle(i));
				} else {
					x += server().CENTER_CIRCLE_R() * cos(Angle(i));
					y += server().CENTER_CIRCLE_R() * sin(Angle(i));
				}
				s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(Angle(i)), y + 2*server().PITCH_LENGTH() * sin(Angle(i)));
			}
			for(i=(int)log(server().player_size()); i<50; i++) {
				double x = self().position().x();
				double y = self().position().y();
				double a = i * server().quantize_step();
				a = exp(a);
				a = floor(a*10 + 0.5) / 10;
				s.circle(x, y, a);
			}
		}*/
		
		// $B>pJs$rI=<((B
		char buffer[1024];
		int len = sprintf(buffer,
			"#%d stamina:%f effort:%f  [%s]  neckAngle:%f  time:%d (world/self/ball:/%d/%d/%d) timeLimit:%d",
			self().uniformNumber(), self().stamina(), self().effort(),
			playModeHelper.toString(world().playMode()), (double)self().neckAngle().degrees(),
			(int)world().refereeTimer(), (int)world().time(), (int)self().time(), (int)ball().time(), (int)m_moveTimeLimit);
		ASSERT(len < 1023);
		if(ball().velocity().valid()) {
			len += sprintf(buffer+len,
				"  ball_speed:%f",
				ball().velocity().norm());
			ASSERT(len < 1023);
		}
		s.write(buffer);
		
		// $B%S%e!<%3!<%s$J$I$rI=<((B
		s.color(s.colorGreen());
		s.circle(self().position().x(), self().position().y(), server().player_size());
		double x = self().position().x();
		double y = self().position().y();
		s.line(x, y, x + server().CENTER_CIRCLE_R() * cos(self().bodyDirection().radians()),
			y + server().CENTER_CIRCLE_R() * sin(self().bodyDirection().radians()));
		double widthRadians = self().viewWidthDegrees() * (PI/180.0);
		double startRadians = self().sightDirection().radians() - widthRadians/2;
		double endRadians = self().sightDirection().radians() + widthRadians/2;
		s.arc(x, y, server().visible_distance(), endRadians, startRadians);
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(startRadians), y + 2*server().PITCH_LENGTH() * sin(startRadians));
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(endRadians), y + 2*server().PITCH_LENGTH() * sin(endRadians));
		s.arc(x, y, server().UNUM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().UNUM_TOOFAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_TOOFAR_LENGTH(), startRadians, endRadians);
		s.circle(x, y, reachableByDash());
		s.color(s.colorYellow());
		s.circle(x, y, server().player_size() + server().kickable_margin());
		s.color(s.colorRed());
		s.circle(x, y, server().audio_cut_dist());

		// $B%U%i%0$rI=<((B
		{
			s.color(s.colorFlag());
			World::Vectors::const_iterator it = world().flags().begin();
			for(; it != world().flags().end(); it++) {
				const double d = 0.8;
				const Vector& v = *it;
				s.circle(v.x(), v.y(), d);
			}
		}
		
		// $B%W%l%$%d!<$rI=<((B
		{
			Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				int color = s.colorYellow();
				if(p.team().valid()) {
					switch(p.team()) {
					case SIDE_LEFT:
						color = s.colorGreen();
						break;
					case SIDE_RIGHT:
						color = s.colorRed();
						break;
					default:
						ASSERT(false);
					}
				}
				s.color(color);
				if(p.uniformNumber().valid()) {
					s.circle(p.position().x(), p.position().y(), server().player_size());
				} else {
					s.circleFill(p.position().x(), p.position().y(), server().player_size());
				}
				if(p.velocity().valid()) {
					Vector v0 = p.position();
					Vector v1 = p.velocity() * config().mobilityRatioUntilSlow(p.velocity().norm());
					v1 += v0;
					s.line(v0.x(), v0.y(), v1.x(), v1.y());
				}
			}
		}

		// $B%\!<%k$rI=<((B
		{
			const double d = 0.5;
			int diff = world().time() - ball().time();
			if(diff <= 0)
				s.color(s.colorGreen());
			else if(diff < 3)
				s.color(s.colorYellow());
			else
				s.color(s.colorRed());
			const Vector& v = ball().position();
			s.circle(v.x(), v.y(), server().ball_size());
			s.line(v.x() - d, v.y(), v.x() + d, v.y());
			s.line(v.x(), v.y() - d, v.x(), v.y() + d);
			if(ball().velocity().valid()) {
				Vector v0 = ball().position();
				Vector v1 = ball().velocity() * config().mobilityRatioUntilSlow(ball().velocity().norm());
				v1 += v0;
				s.line(v0.x(), v0.y(), v1.x(), v1.y());
			}
		}

#if LEARNING
		if(world().m_inspiration) {
			for(int i=0; i<23; i++) {
				const CoachSeeObject* object;
				if(i < 11) {
					s.color(s.colorGreen());
					object = &world().m_inspirationSee.player(SIDE_LEFT, i);
				} else if(i < 22) {
					s.color(s.colorRed());
					object = &world().m_inspirationSee.player(SIDE_RIGHT, i-11);
				} else {
					s.color(s.colorYellow());
					object = &world().m_inspirationSee.ball();
				}
				if(object->valid()) {
					const double d = server().player_size() * 2;
					const Vector& v = object->position();
					s.circle(v.x(), v.y(), d);
				}
			}
		}
#endif
#if WITH_WINDOW
		// $B;W9M$rI=<((B
		if(m_traces.size() >= 2) {
			s.color(s.colorThinking());
			Vectors::const_iterator it = m_traces.begin();
			Vector pre = *it++;
			for(; it != m_traces.end(); it++) {
				const Vector& v = *it;
				s.line(pre.x(), pre.y(), v.x(), v.y());
				pre = v;
			}
		}
#endif
	}
	m_world.unlock();
}

double Gullwing::powerLimit(double stamina, bool forward, double dash) const	// walk: 0.0=$B2sI|M%@h(B 1.0=$B%P%i%s%9(B 2.0=$BB.EYM%@h(B
{
	ASSERT(0.0 <= dash && dash <= 2.0);
	double threshold = max(server().recover_dec_thr(), server().effort_dec_thr());
	double minimum = server().stamina_inc_max() - (server().stamina_max() - stamina);
	double middle;
	double maximum = stamina - server().stamina_max() * threshold - server().stamina_inc_max();
	
	double tmp = stamina - server().stamina_max() * threshold;
	if(tmp < 0) {
		middle = 0;
	} else {
		middle = server().stamina_inc_max() * (tmp / server().stamina_max() * (1.0 - threshold));
	}

	switch(world().playMode()) {
	case PLAYMODE_BEFORE_KICK_OFF:
	case PLAYMODE_GOAL_L:
	case PLAYMODE_GOAL_R:
	case PLAYMODE_TIME_OVER:
		minimum = 0;
		middle = 0;
		maximum = 0;
		break;

	case PLAYMODE_PLAY_ON:
	case PLAYMODE_DROP_BALL:
	case PLAYMODE_GOALIE_CATCH_L:
		break;

	case PLAYMODE_OFFSIDE_R:
	case PLAYMODE_KICK_OFF_L:
	case PLAYMODE_KICK_IN_L:
	case PLAYMODE_FREE_KICK_L:
	case PLAYMODE_CORNER_KICK_L:
	case PLAYMODE_GOAL_KICK_L:
		maximum = middle;
		break;

	case PLAYMODE_OFFSIDE_L:
	case PLAYMODE_KICK_OFF_R:
	case PLAYMODE_KICK_IN_R:
	case PLAYMODE_FREE_KICK_R:
	case PLAYMODE_CORNER_KICK_R:
	case PLAYMODE_GOAL_KICK_R:
	case PLAYMODE_GOALIE_CATCH_R:
		maximum = middle;
		break;

	default:
		ASSERT(false);
	}
	
	double power;
	if(dash <= 1.0) {
		power = (minimum * (1.0 - dash)) + (middle * dash);
	} else {
		dash -= 1.0;
		power = (middle * (1.0 - dash)) + (maximum * dash);
	}

	if(forward)
		return suppress(0.0, power, server().maxpower());
	else
		return suppress(server().minpower(), -power, 0.0);
}

double Gullwing::cyclesDashableWithFullPower() const
{
	double threshold = max(server().recover_dec_thr(), server().effort_dec_thr());
	double power = self().stamina() - server().stamina_max() * threshold - server().stamina_inc_max();
	if(power < 0)
		return 0;
	ASSERT(server().maxpower() > server().stamina_inc_max());
	double cycle = power / (server().maxpower() - server().stamina_inc_max());
	return cycle;
}
double Gullwing::reachableByDash() const
{
	double cycle = cyclesDashableWithFullPower();
	return cycle * self().effort() * server().maxpower() * server().dash_power_rate();
}
double Gullwing::getPositionValue(Vector position) const
{
	double mate = DBL_MAX;
	double x0 = 0;	// -server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()*1.5;
	if(m_mode == MODE_SETPLAYR)
		x0 = -server().PITCH_LENGTH()/2;
	if(m_mode != MODE_SETPLAYL)
		x0 = min(x0, ball().position().x() - server().CENTER_CIRCLE_R());
	double x1 = lineOffside() - 2 * server().maxpower() * server().dash_power_rate();
	if(x1 < -server().player_size())
		x1 = -server().player_size();
	double y0 = -server().PITCH_WIDTH()/2;
	double y1 = -y0;
	if(position.x() < x0)
		return position.x() - x0;
	if(position.x() > x1)
		return x1 - position.x();
	/*if(position.y() < y0)
		return position.y() - y0;
	if(position.y() > y1)
		return y1 - position.y();*/

	/*mate = min(mate, (position.x() - x0) * 2);
	mate = min(mate, (x1 - position.x()) * 2);*/
	mate = min(mate, (position.y() - y0) * 2);
	mate = min(mate, (y1 - position.y()) * 2);

	double enemy = DBL_MAX;
	Players::const_iterator it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		if(p.team() == SIDE_LEFT) {
			mate = min(mate, (position - p.position()).norm());
			//mate = min(mate, abs(position.x() - p.position().x()));
		} else {
			enemy = min(enemy, (position - p.position()).norm());
			//enemy = min(enemy, abs((position.y() - p.position().y())));
		}
	}
	return mate /*+ enemy/2*/;
	if(position.x() > 0)
		return mate + enemy/2;
	else
		return mate - enemy/2;
}

void Gullwing::initializeWorkArea()
{
	ASSERT(sizeof(m_minOthers) == sizeof(m_minOthersMaster));
	ASSERT(sizeof(m_maxOthers) == sizeof(m_maxOthersMaster));
	memcpy(m_minOthers, m_minOthersMaster, sizeof(m_minOthers));
	memcpy(m_maxOthers, m_maxOthersMaster, sizeof(m_maxOthers));
	
	double reachableByKick = config().reachableByKick();
	double reachableByDash = min(this->reachableByDash() / 2, reachableByKick * 0.8);
	Players::const_iterator it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		Vector toPlayer = p.position() - self().position();
		double distance = toPlayer.norm();
		if(distance > 0) {
			int side = (p.team() == SIDE_LEFT) ? LEFT : RIGHT;
			Distance zone;
			if(distance < reachableByDash)
				zone = NEAR;
			else if(distance < reachableByKick)
				zone = FAR;
			else
				zone = TOOFAR;
			int dir = (int)(toPlayer.angle().degrees() * ((double)sizeWorkArea/360.0));
			if(dir < 0)
				dir += sizeWorkArea;
			dir %= sizeWorkArea;
			for(;;) {	// $B#22s%k!<%W(B
				m_minOthers[side][zone][dir] = min(m_minOthers[side][zone][dir], distance);
				m_maxOthers[side][zone][dir] = max(m_maxOthers[side][zone][dir], distance);
				if(dir == sizeWorkArea)
					break;
				dir = sizeWorkArea;
			}
		}
	}

	double goalie = 0;
	m_lineOffside = 0;
	it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		if(p.team() == SIDE_RIGHT) {
			double x = p.position().x();
			if(goalie <= x) {
				m_lineOffside = goalie;
				goalie = x;
			} else if(m_lineOffside <= x) {
				m_lineOffside = x;
			}
		}
	}
}

void Gullwing::received(const PlayerHear& hi, int key)
{
	SayBuffer b;
	if(hi.senderType() == SENDER_DIRECTION && b.fromArray(hi.message(), hi.refereeTimer(), key)) {
		short type, cx, cy, tx, ty, count;
		b.get(type);
		b.get(cx);
		b.get(cy);
		b.get(tx);
		b.get(ty);
		b.get(count);
		
		Vector catcher = Vector((double)cx / SHRT_MAX * server().PITCH_LENGTH(),
								(double)cy / SHRT_MAX * server().PITCH_WIDTH());
		double norm = (catcher - self().position()).norm();
		Players::const_iterator it = others().begin();
		for(; it != others().end(); it++) {
			const Player& p = *it;
			if((catcher - p.position()).norm() < norm && p.team() == SIDE_LEFT)
				return;
		}
		
		m_moveTarget = Vector((double)tx / SHRT_MAX * server().PITCH_LENGTH(),
								(double)ty / SHRT_MAX * server().PITCH_WIDTH());
		m_moveTimeLimit = world().time() + min((int)count, 20);
		
#if WITH_WINDOW
		m_traces.clear();
		m_traces.push_back(self().position());
		m_traces.push_back(m_moveTarget);
#endif
	} else if(hi.senderType() == SENDER_REFEREE) {
		if(hi.refereeMessage() == REFREEMESSAGE_GOALIE_CATCH_BALL_L) {
			m_timeCaught = world().time();
		}
	}
}

void Gullwing::steped()
{
	double last = 0;
	m_enemyOffside = 0;
	Players::const_iterator it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		if(p.team() == SIDE_RIGHT) {
			if(p.position().x() > last) {
				m_enemyOffside = last;
				last = p.position().x();
			} else if(p.position().x() > m_enemyOffside) {
				m_enemyOffside = p.position().x();
			}
		}
	}

	last = 0;
	m_ourOffside = 0;
	it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		if(p.team() == SIDE_LEFT) {
			if(p.position().x() < last) {
				m_ourOffside = last;
				last = p.position().x();
			} else if(p.position().x() < m_ourOffside) {
				m_ourOffside = p.position().x();
			}
		}
	}

	m_toBall = ball().position() - self().position();
	m_distanceToBall = m_toBall.norm();
	m_directionToBall = (m_distanceToBall == 0) ? Angle(0) : m_toBall.angle();
	
	m_toNextBall = (ball().position() + ball().safeVelocity()) - self().position();
	m_distanceToNextBall = m_toNextBall.norm();
	m_directionToNextBall = (m_distanceToNextBall == 0) ? Angle(0) : m_toNextBall.angle();
}

void Gullwing::go()
{
#if 0
	char logName[256];
	char textName[256];
	time_t t;
	time(&t);
#ifdef _MSC_VER
	long s = ::timeGetTime();
#else
	long s = ::getpid();
#endif
#if LEARNING
	sprintf(logName, "_log_%d_%ld_%d.playerlog.log", (int)t, (long)s, (int)self().uniformNumber());
	sprintf(textName, "_log_%d_%ld_%d.playerlog.cvs", (int)t, (long)s, (int)self().uniformNumber());
	if(!m_config.openLogFile(logName, textName)) {
		report("error: can't open log file '%s'\n", logName);
		return;
	}
#endif
#endif

	m_world.setConfig(&m_config);

	// $BA0Ds(B:$B%W%l%$%b!<%I$,JQ$o$i$J$$!#(B
	class PlayModeConstant : GullwingPremise
	{
		PlayMode m_playMode;
	public:
		PlayModeConstant(Gullwing* brain) : GullwingPremise(brain)
		{
			m_playMode = m_brain->world().playMode();
		}
		bool checkPremise() {
			PlayMode playMode = m_brain->world().playMode();
			bool result = (m_playMode == playMode);
			m_playMode = playMode;
			if(!result)
				m_brain->m_timePlayModeChanged = m_brain->world().time();
			return result;
		}
	} anonym(this);
	
	Monobrain<Green>::go();
};

void Gullwing::act()
{
	switch(world().playMode()) {
	case PLAYMODE_BEFORE_KICK_OFF:
	case PLAYMODE_GOAL_L:
	case PLAYMODE_GOAL_R:
		m_mode = MODE_NORMAL;
		movable();
		break;
		
	case PLAYMODE_PLAY_ON:
	case PLAYMODE_DROP_BALL:
	case PLAYMODE_OFFSIDE_L:
	case PLAYMODE_OFFSIDE_R:
		m_mode = MODE_NORMAL;
		normal();
		break;
		
	case PLAYMODE_TIME_OVER:
		return;
		
	case PLAYMODE_KICK_OFF_L:
	case PLAYMODE_KICK_IN_L:
	case PLAYMODE_FREE_KICK_L:
	case PLAYMODE_CORNER_KICK_L:
	case PLAYMODE_GOAL_KICK_L:
		m_mode = MODE_SETPLAYL;
		normal();
		break;
		
	case PLAYMODE_GOALIE_CATCH_L:
		m_mode = MODE_SETPLAYL;
		goalieCatching();
		return;

	case PLAYMODE_KICK_OFF_R:
	case PLAYMODE_KICK_IN_R:
	case PLAYMODE_FREE_KICK_R:
	case PLAYMODE_CORNER_KICK_R:
	case PLAYMODE_GOAL_KICK_R:
	case PLAYMODE_GOALIE_CATCH_R:
		m_mode = MODE_SETPLAYR;
		normal();
		break;
		
	default:
		ASSERT(false);
	}
}

void Gullwing::movable()
{
	double x = -m_random.next() * (server().PITCH_LENGTH()/2 - server().PENALTY_AREA_LENGTH()*1.5);
	double y = m_random.next() * server().PITCH_WIDTH() - server().PITCH_WIDTH()/2;
	
	double x0 = -server().PITCH_LENGTH()/2 + server().GOAL_AREA_LENGTH()/2;
	double x1 = -server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()*1.5;
	double x3 = -server().PITCH_LENGTH()*2/40;
	double x2 = (x1 + x3) / 2;
	double x4 = -server().PITCH_LENGTH()*1/40;
	double y0 = 0;
	double y1 = server().PITCH_WIDTH()*1/8;
	double y2 = server().PITCH_WIDTH()*2/8;
	double y3 = server().PITCH_WIDTH()*3/8;
	switch(self().uniformNumber()) {
	case 0:
		x = x0;
		y = 0;
		break;
	
	case 1://2:
		x = x1;
		y = 0;
		break;
	case 2://4:
		x = x1;
		y = y2;
		break;
	case 3://6:
		x = x1;
		y = -y2;
		break;
	
	case 4://8:
		x = x2;
		y = y1;
		break;
	case 5://7:
		x = x2;
		y = -y1;
		break;
	case 6://10:
		x = x2;
		y = y3;
		break;
	case 7://9:
		x = x2;
		y = -y3;
		break;
	
	case 8://5:
		x = x3;
		y = y2;
		break;
	case 9://3:
		x = x3;
		y = -y2;
		break;
	case 10://1:
		x = x4;
		y = y0;
		break;
	}
	changeView(VIEW_WIDTH_NORMAL, VIEW_QUALITY_HIGH);
	turnNeckTo(Angle(0));
	move(Vector(x, y));
	
	for(;;) {
		seeAround();
	}
}

void Gullwing::goalieCatching()
{
	if(self().isGoalie()) {
		// move
		double plus = 0;
		Players::const_iterator it = others().begin();
		for(; it != others().end(); it++) {
			const Player& p = *it;
			if(p.team() == SIDE_RIGHT && p.position().x() < 0) {
				if(p.position().y() > 0)
					plus += p.position().x() * p.position().x();
				else
					plus -= p.position().x() * p.position().x();
			}
		}
		if(plus >= 0)
			move(Vector(-server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()*2/3,
			-server().PITCH_WIDTH()/4));
		else
			move(Vector(-server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()*2/3,
			server().PITCH_WIDTH()/4));
		
		// $B%o!<%k%I%b%G%k99?7(B
		seeAround();
		findBall();
		
		// $BL#J}$,0\F0$9$k$N$rBT$D!#(B
		int limit = world().time() + 200;
		for(; world().time() < limit; seeAround()) {
			Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				if((self().position() - p.position()).norm() < 3 * server().kickable_area())
					break;
			}
			if(it != others().end())
				continue;
			
			// $B%o!<%/%(%j%"$r:n$k(B
			PlayerStates states;
			const PlayerStates& constStates = states;
			{
				PlayerState ps;
				ps.player = &self();
				ps.position = self().position();
				ps.time = world().time();
				states.push_back(ps);
				
				Players::const_iterator it = others().begin();
				for(; it != others().end(); it++) {
					const Player& p = *it;
					ps.player = &p;
					ps.position = p.position();
					ps.time = world().time();
					states.push_back(ps);
				}
			}
			
			const double initialSpeed = server().kickable_area() * server().ball_decay() + server().maxpower() * server().kick_power_rate() * 0.75;
			
			// $B%Q%9$r9M$($k(B
			{
				typedef std::set<Passwork, std::greater<Passwork> > Passworks;
				Passworks passworks;
				
				const int step = 5;
				int base = (int)(step * m_random.next());
				for(int dir=base; dir<360+base; dir+=step) {
					Vector ballPosition = ball().position();
					Vector ballVelocity = Vector(initialSpeed, Angle(dir));
					int time = world().time();
					double nearestEnemy = DBL_MAX;
					int result = simulate(constStates, ballPosition, ballVelocity, time, nearestEnemy);
					const WhitePlayer* catcher = (result >= 0) ? constStates[result].player : 0;
					if(result == -3 || (catcher && catcher->team() == SIDE_LEFT
						&& ballPosition.x() >= -server().PITCH_LENGTH()/2 + server().GOAL_AREA_LENGTH() * 2)) {
						Passwork p;
						p.startDirection = Angle(dir);
						p.startSpeed = initialSpeed;
						p.playerStates = constStates;
						if(catcher) {
							p.playerStates[result].position = ballPosition;
							p.playerStates[result].time = time;
						}
						p.ballPosition = ballPosition;
						p.time = time;
						p.closed = false;
						p.shoot = catcher ? false : true;
						p.catcher = result;
						p.firstCatch = ballPosition;
						p.enemy = nearestEnemy;
						p.time = time;
#if WITH_WINDOW
						p.traces.push_back(ball().position());
						p.traces.push_back(p.ballPosition);
#endif
						passworks.insert(p);
					}
				}
				if(!passworks.empty()) {
					const Passwork& p = *passworks.begin();
#if WITH_WINDOW
					m_traces = p.traces;
#endif
					
					double last = 0;
					double offside = 0;
					Players::const_iterator it = others().begin();
					for(; it != others().end(); it++) {
						const Player& p = *it;
						if(p.team() == SIDE_RIGHT) {
							double x = p.position().x();
							if(last < x) {
								offside = last;
								last = x;
							} else if(offside < x) {
								offside = x;
							}
						}
					}
					
					if(!p.shoot) {
						Vector catcher = p.playerStates[p.catcher].player->position();
						if(catcher.x() < offside) {
							say(SAY_MOVE);
							short tmp = (short)(catcher.x() / server().PITCH_LENGTH() * SHRT_MAX);
							say(tmp);
							tmp = (short)(catcher.y() / server().PITCH_WIDTH() * SHRT_MAX);
							say(tmp);
							tmp = (short)(p.firstCatch.x() / server().PITCH_LENGTH() * SHRT_MAX);
							say(tmp);
							tmp = (short)(p.firstCatch.y() / server().PITCH_WIDTH() * SHRT_MAX);
							say(tmp);
							tmp = (short)6;
							say(tmp);
						}
					}


					// $BA0Ds(B:$B%\!<%k$,%3%s%H%m!<%k2DG=$JHO0O$+$i$O$:$l$J$$!#(B
					class BallControllable : GullwingPremise
					{
					public:
						BallControllable(Gullwing* brain) : GullwingPremise(brain)
						{
						}
						bool checkPremise() {
							return m_brain->toBall().norm() <= server().kickable_area();
						}
					} anonym(this);
#if NEVER_THROW
#  error
#endif
					// $B%\!<%k$r>/$7=3$k!#(B
					try {
						Vector target = toBall();
						if(target == Vector(0, 0)) {
							target = Vector(server().player_size() + server().kickable_margin()/3, Angle(0));
						} else {
							double tmp1 = server().player_size() + server().kickable_margin()*1/3;
							double tmp2 = server().player_size() + server().kickable_margin()*2/3;
							if(target.norm() < tmp1)
								target.normalize(tmp1);
							else if(target.norm() > tmp2)
								target.normalize(tmp2);
						}
						kick(target - toBall() - ball().safeVelocity());
					} catch(PremiseException) {
					}
					if(world().playMode() == PLAYMODE_GOALIE_CATCH_L)
						return;
					kickTwiceTo(p.startSpeed, p.startDirection);
					return;
				}
			}
		}
	} else {
		free();
	}
}

void Gullwing::normal()
{
	// $B%\!<%k$r8+<:$C$?$i!"C5$9!#(B
	if(world().time() - ball().time() > 6) {
		findBall();
		return;
	}
	
	initializeWorkArea();
	
	// $B%-!<%Q!<$J$i!"%-%c%C%A$r9M$($k!#(B
	if(self().isGoalie() && world().playMode() != PLAYMODE_GOALIE_CATCH_L && isInMyPenaltyArea(self().position())) {
		if(distanceToBall() <= server().catchable_area_l() * 0.8
			&& world().time() >= self().timeLastCatch() + server().catch_ban_cycle()
			/*&& world().time() > m_timePlayModeChanged + 10*/) {
			changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
			turnNeckTo(directionToBall() - self().bodyDirection());
			catchIn(directionToBall());
			return;
		}
	}

	if(world().playMode() == PLAYMODE_GOALIE_CATCH_L) {
		if(!self().isGoalie())
			free();			// $B$H$j$"$($:!"$R$^!#(B
		else
			ballNear();		// $B%\!<%k$,6a$/$K$"$k!#(B
	} else if(distanceToBall() <= server().kickable_area() * 0.95)
		ballNear();
	else
		ballFar();			// $B%\!<%k$,1s$/$K$"$k!#(B
	return;
}

// $B%7%_%e%l!<%7%g%s$r9T$&!#%\!<%k$rJaB*$7$?%W%l%$%d!<$NE:$(;z!"(B-1($B$@$l$bJaB*$7$J$$(B)$B!"(B
// -2($B%\!<%k$,30$K=P$?(B)$B!"$b$7$/$O(B -3($B%7%e!<%H$,7h$^$C$?(B)$B$rJV$9!#(B
int Gullwing::simulate(const PlayerStates& players, Vector& ballPosition, Vector& ballVelocity, int& time, double& nearestEnemy)
{
	const double speed = server().maxpower() * server().dash_power_rate() * (1 + server().player_decay() + server().player_decay() * server().player_decay());
	//double speed = 0;
	//double run = 0;		// $B%W%l%$%d!<$NAv9T5wN%(B
	int catcher = -1;	// $B%\!<%k$r:G=i$KJdB-$9$k%W%l%$%d!<(B
	int count = 0;
	for(; count<20; count++, time++) {	// 20 $B%5%$%/%kDI$$$+$1$FDI$$$D$+$J$$$J$i!"$"$-$i$a$k!#(B
		//speed += server().maxpower() * server().dash_power_rate();
		//run += speed;
		//speed *= server().player_decay();
		Vector preposition = ballPosition;
		ballPosition += ballVelocity;
		ballVelocity *= server().ball_decay();
		
		if(!isInPitch(ballPosition)) {
			catcher = -2;
			if(preposition.x() < server().PITCH_LENGTH()/2 && ballPosition.x() > server().PITCH_LENGTH()/2) {
				double ratio = (server().PITCH_LENGTH()/2 - preposition.x()) / (ballPosition.x() - preposition.x());
				double y = ratio * preposition.y() + (1-ratio) * ballPosition.y();
				if(abs(y) < server().goal_width()/2 * 0.8) {
					// $B%7%e!<%H@.N)(B
					catcher = -3;
				}
			}
			break;
		}

		//double misprision = ballPosition.x() / (server().PITCH_LENGTH()/2);
		//double misprision = ballPosition.x() / (server().PITCH_LENGTH()/2);
		/*if(isInEnemyPenaltyArea(ballPosition))
			misprision = +1.0;	// $B2a>.I>2A(B
		if(self().isGoalie() && world().playMode() == PLAYMODE_GOALIE_CATCH_L) {
			int time = world().time() - m_timeCaught;
			if(time <= 0)
				time = 0;
			misprision /= time * 0.3 + 1;
		}*/
		//double misprision = -0.1;
		nearestEnemy = DBL_MAX;
		double approach = DBL_MAX;
		PlayerStates::const_iterator it = players.begin();
		for(int i=0; it != players.end(); it++, i++) {
			const PlayerState& p = *it;
			double length = (ballPosition - p.position).norm();
			double run = speed * (time - p.time);
			if(p.player->team() != SIDE_LEFT) {
				double tmp = run * 1.02; // * (1.0 - misprision/5)
				tmp += /*(0.5 - misprision/2) * 2 */ server().dash_power_rate() * server().maxpower();
				tmp = length - tmp;
				nearestEnemy = min(nearestEnemy, tmp);
				if(ballVelocity.norm() < 1.5 * server().kickable_area() /* (1.5 - misprision/2)*/) {
					if(tmp < 0 && tmp < approach) {
						approach = tmp;
						catcher = i;
					}
				}
			} else {
				if(ballVelocity.norm() < 1.0 * server().kickable_area()) {
					double tmp = length - run/* * 0.7*/;
					if(tmp < 0 && tmp < approach) {
						approach = tmp;
						catcher = i;
					}
				}
			}
		}
		if(catcher >= 0)
			break;
	}
	return catcher;
}


void Gullwing::ballNear()
{
	// $BA0Ds(B:$B%\!<%k$,%3%s%H%m!<%k2DG=$JHO0O$+$i$O$:$l$J$$!#(B
	class BallControllable : GullwingPremise
	{
	public:
		BallControllable(Gullwing* brain) : GullwingPremise(brain)
		{
		}
		bool checkPremise() {
			return m_brain->toBall().norm() <= server().kickable_area();
		}
	} anonym(this);

	if(self().isGoalie() && world().playMode() == PLAYMODE_GOALIE_CATCH_L && world().time() - m_timeCaught < 10) {
		// move $B$9$k!#(B
		/*double plus = 0;
		Players::const_iterator it = others().begin();
		for(; it != others().end(); it++) {
			const Player& p = *it;
			if(p.team() == SIDE_RIGHT && p.position().x() < 0) {
				if(p.position().y() > 0)
					plus += -p.position().x();
				else
					plus -= -p.position().x();
			}
		}
		if(plus >= 0)
			move(Vector(-server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()/2,
						-server().PITCH_WIDTH()/4));
		else
			move(Vector(-server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()/2,
						server().PITCH_WIDTH()/4));*/
		move(Vector(-server().PITCH_LENGTH()/2 + server().PENALTY_AREA_LENGTH()/2,
					0));
		//seeAround();
		// $B2<$XB3$/(B
	}

	// $B%o!<%/%(%j%"$r:n$k(B
	PlayerStates states;
	const PlayerStates& constStates = states;
	{
		PlayerState ps;
		ps.player = &self();
		ps.position = self().position();
		ps.time = world().time();
		states.push_back(ps);

		Players::const_iterator it = others().begin();
		for(; it != others().end(); it++) {
			const Player& p = *it;
			ps.player = &p;
			ps.position = p.position();
			ps.time = world().time();
			states.push_back(ps);
		}
	}

	const double initialSpeed = server().kickable_area() * server().ball_decay() + server().maxpower() * server().kick_power_rate() * 0.75;

	// $B%7%e!<%H$r9M$($k(B
	{
		Vector goal = Vector(server().PITCH_LENGTH()/2, 0);
		Vector ballToGoal = goal - ball().position();
		if(isInEnemyPenaltyArea(ball().position()) || ballToGoal.norm() < server().PENALTY_AREA_LENGTH() * 1.5) {
			Vector delta1 = Vector(server().PITCH_LENGTH()/2, -server().goal_width()/2 * 0.8) - ball().position();
			if(delta1 == Vector(0, 0))
				delta1 = Vector(0, -0.0001);
			Vector delta2 = Vector(server().PITCH_LENGTH()/2, +server().goal_width()/2 * 0.8) - ball().position();
			if(delta1 == Vector(0, 0))
				delta1 = Vector(0, 0.0001);

			int dir1 = (int)delta1.angle().degrees();
			int dir2 = (int)delta2.angle().degrees();
			while(dir1 < 0)
				dir1 += 360;
			dir1 %= 360;
			while(dir2 >= 360)
				dir2 -= 360;
			while(dir2 < dir1)
				dir2 += 360;
			if(15 < dir2 - dir1 && dir2 - dir1 < 180) {
				int bestDirection = INT_MIN;
				double bestValue = DBL_MAX/2;
				int i;
				for(i = dir1; i <= dir2; i+=1) {
					Vector ballPosition = ball().position();
					Vector ballVelocity = Vector(initialSpeed, Angle(i));
					int time = world().time();
					double nearestEnemy = DBL_MAX;
					int catcher = simulate(constStates, ballPosition, ballVelocity, time, nearestEnemy);
					double value = DBL_MAX;
					if(catcher == -3)
						value = -nearestEnemy;
					else if(catcher >= 0)
						value = 10 * server().PITCH_LENGTH() - ballPosition.x();	// $B%4!<%k$K6a$$$b$N$rM%@h(B
					if(value < bestValue) {
						bestValue = value;
						bestDirection = i;
					}
				}

				if(bestDirection != INT_MIN) {
					kickTwiceTo(initialSpeed, Angle(bestDirection));
					return;
				}
			}
		}
	}
	// $B%Q%9$r9M$($k(B
	{
		typedef std::set<Passwork, std::greater<Passwork> > Passworks;
		Passworks passworks;
		
		const int step = 15;
		int base = (int)(step * m_random.next());
		for(int dir=base; dir<360+base; dir+=step) {
			Vector ballPosition = ball().position();
			Vector ballVelocity = Vector(initialSpeed, Angle(dir));
			int time = world().time();
			double nearestEnemy = DBL_MAX;
			int result = simulate(constStates, ballPosition, ballVelocity, time, nearestEnemy);
			const WhitePlayer* catcher = (result >= 0) ? constStates[result].player : 0;
			if(result == -3 || (catcher && catcher->team() == SIDE_LEFT
				&& (ballPosition.x() >= ball().position().x()
					|| ballPosition.x() >= -server().PITCH_LENGTH()/2 + 1.3 * server().PENALTY_AREA_LENGTH()))) {
				Passwork p;
				p.startDirection = Angle(dir);
				p.startSpeed = initialSpeed;
				p.playerStates = constStates;
				if(catcher) {
					p.playerStates[result].position = ballPosition;
					p.playerStates[result].time = time;
				}
				p.ballPosition = ballPosition;
				p.time = time;
				p.closed = false;
				p.shoot = catcher ? false : true;
				p.catcher = result;
				p.firstCatch = ballPosition;
				p.enemy = nearestEnemy;
				p.time = time;
#if WITH_WINDOW
				p.traces.push_back(ball().position());
				p.traces.push_back(p.ballPosition);
#endif
				passworks.insert(p);
			}
		}

/*$B$3$N%"%k%4%j%:%`$G@hFI$_$7$F$b!"%W%l%$%d!<$N@)05HO0O$,9-$9$.$F!"@5Ev$JI>2A$K$J$i$J$$!#(B
		for(int i=0; i<20; i++) {
			Passworks::iterator it = passworks.begin();
			for(; it != passworks.end(); it++) {
				if(!it->closed)
					break;
			}
			if(it == passworks.end())
				break;

			Passwork master = *it;
			passworks.erase(it);
			const int step = 30;
			int base = (int)(step * m_random.next());
			for(int dir=base; dir<360+base; dir+=step) {
				Vector ballPosition = master.ballPosition;
				Vector ballVelocity = Vector(initialSpeed, Angle(dir));
				int time = master.time;
				double nearestEnemy = DBL_MAX;
				int result = simulate(master.playerStates, ballPosition, ballVelocity, time, nearestEnemy);
				const WhitePlayer* catcher = (result >= 0) ? constStates[result].player : 0;
				if(result == -3 || (catcher && catcher->team() == SIDE_LEFT)) {
					Passwork p = master;
					if(catcher) {
						p.playerStates[result].position = ballPosition;
						p.playerStates[result].time = time;
					}
					p.ballPosition = ballPosition;
					p.time = time;
					p.shoot = catcher ? false : true;
					p.catcher = result;
					p.enemy = nearestEnemy;
					p.time = time;
#if WITH_WINDOW
					p.traces.push_back(p.ballPosition);
#endif
					passworks.insert(p);
				}
			}
			master.closed = true;
			passworks.insert(master);
		}
*/
		
		if(!passworks.empty()) {
			const Passwork& p = *passworks.begin();
			if(self().position().x() < server().PITCH_LENGTH()/4 || p.ballPosition.x() > server().PITCH_LENGTH()/4) {
#if WITH_WINDOW
				m_traces = p.traces;
#endif
				
				double last = 0;
				double offside = 0;
				Players::const_iterator it = others().begin();
				for(; it != others().end(); it++) {
					const Player& p = *it;
					if(p.team() == SIDE_RIGHT) {
						double x = p.position().x();
						if(last < x) {
							offside = last;
							last = x;
						} else if(offside < x) {
							offside = x;
						}
					}
				}
				
				if(!p.shoot) {
					Vector catcher = p.playerStates[p.catcher].player->position();
					if(catcher.x() < offside) {
						say(SAY_MOVE);
						short tmp = (short)(catcher.x() / server().PITCH_LENGTH() * SHRT_MAX);
						say(tmp);
						tmp = (short)(catcher.y() / server().PITCH_WIDTH() * SHRT_MAX);
						say(tmp);
						tmp = (short)(p.firstCatch.x() / server().PITCH_LENGTH() * SHRT_MAX);
						say(tmp);
						tmp = (short)(p.firstCatch.y() / server().PITCH_WIDTH() * SHRT_MAX);
						say(tmp);
						tmp = (short)6;
						say(tmp);
					}
				}
				
				kickTwiceTo(p.startSpeed, p.startDirection);
				return;
			}
		}

#if WITH_WINDOW
		m_traces.clear();
#endif
		if(self().isGoalie() && world().playMode() == PLAYMODE_GOALIE_CATCH_L) {
			// $B%Q%9$G$-$J$$!#$@$l$+6a$/$K$-$F$[$7$$!#(B
			double distance = DBL_MAX;
			Vector nearest = Vector(0, 0);
			Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				if(p.team() == SIDE_LEFT) {
					Vector toPlayer = p.position() - self().position();
					double d = toPlayer.norm();
					if(d < distance) {
						distance = d;
						nearest = p.position();
					}
				}
			}
			say(SAY_MOVE);
			short tmp = (short)(nearest.x() / server().PITCH_LENGTH() * SHRT_MAX);
			say(tmp);
			tmp = (short)(nearest.y() / server().PITCH_WIDTH() * SHRT_MAX);
			say(tmp);
			tmp = (short)(self().position().x() / server().PITCH_LENGTH() * SHRT_MAX);
			say(tmp);
			tmp = (short)(self().position().y() / server().PITCH_WIDTH() * SHRT_MAX);
			say(tmp);
			tmp = (short)10;
			say(tmp);
			seeAround();
		} else {
			Vector toGoal = Vector(server().PITCH_LENGTH()/2, 0) - self().position();
			Angle directionToGoal = (toGoal == Vector(0, 0)) ? Angle(0) : toGoal.angle();

			// $B%I%j%V%k(B
			kick(Vector(1.5 * server().dash_power_rate() * server().maxpower(), directionToGoal) - ball().safeVelocity());
			//kickTwiceTo(server().PITCH_LENGTH(), directionToGoal);
		}
	}
}

void Gullwing::ballFar()
{
	const int cycles = 20;	// $B%7%_%e%l!<%7%g%s$9$k%5%$%/%k?t(B
	// $B%\!<%k$rJa3M$G$-$kCOE@$r8+6K$a$k!#(B
	Vector target = ball().position();	// $B%?!<%2%C%HCOE@(B
	if(ball().velocity().valid()) {
		Vector ball = this->ball().position();		// $B%\!<%k$N0LCV(B
		Vector velocity = this->ball().velocity();	// $B%\!<%k$NB.EY(B
		double stamina = self().stamina();		// $B<+J,$N%9%?%_%J(B
		double speed = 0;						// $B<+J,$N%9%T!<%I(B
		double run = 0;							// $B<+J,$NAv$C$?5wN%(B
		for(int i=0; i<cycles; i++) {	// cycles $B%5%$%/%kDI$$$+$1$FDI$$$D$+$J$$$J$i!"$"$-$i$a$k!#(B
			double power = powerLimit(stamina, true, 2.0);
			stamina = stamina - power + server().stamina_inc_max();
			speed += power * self().effort() * server().dash_power_rate();
			speed = min(speed, server().player_speed_max());
			run += speed;
			ball += velocity;
			
			if(velocity.norm() < 2 * server().kickable_area()) {
				Vector toBall = ball - self().position();
				Angle angle = (toBall == Vector(0, 0)) ? Angle(0) : toBall.angle();
				if(abs(angle - self().bodyDirection()).degrees() < 10) {
					if((ball - self().position()).norm() <= run)
						break;
				} else {
					// turn $B$NJ,!"Av$l$J$$(B
					if((ball - self().position()).norm() <= run - speed)
						break;
				}
			}

			speed *= server().player_decay();
			velocity *= server().ball_decay();
		}
		target = ball;
	}
	
	Vector toTarget = target - self().position();
	
	if(self().isGoalie()) {
		// $B%-!<%Q!<$O%Z%J%k%F%#!<%(%j%"$N30$N$3$H$O!"6=L#L5$$!#(B
		if(!isInMyPenaltyArea(target)) {
			free();
			return;
		}
		// $B30$X=P$F9T$/%\!<%k$K$b!"6=L#$O$J$$!#(B
		Vector myGoal(-server().PITCH_LENGTH()/2, 0);
		Vector goalToBall = ball().position() - myGoal;
		Vector velocity = ball().safeVelocity();
		if(velocity != Vector(0, 0) && abs(goalToBall.angle() - velocity.angle()).degrees() < 60) {
			Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				if(p.team() == SIDE_RIGHT)
					if((ball().position() - p.position()).norm() < 2 * server().kickable_area())
						if((ball().position() + velocity - p.position()).norm() < 2 * server().kickable_area())
							break;
			}
			if(it == others().end()) {
				free();
				return;
			}
		}
	} else {
		// $BB>$K6a$$L#J}(B($B%-!<%Q!<$r=|$/(B)$B$,$$$k$+D4$Y$k(B
		bool mateNear = false;
		Players::const_iterator it = others().begin();
		for(; it != others().end(); ++it) {
			const Player& player = *it;
			if(player.team() == SIDE_LEFT && player.uniformNumber() != 0) {
				Vector himToTarget = target - player.position();
				if(himToTarget.norm() < toTarget.norm() * 0.9) {
					mateNear = true;
					break;
				}
			}
		}
		// $BB>$K6a$$L#J}$,$$$l$P!"<+J,$O$H$j$"$($:$R$^!#(B
		if(mateNear) {
			free();
			return;
		}
	}
	
	// $B%\!<%k$r<h$j$KAv$k(B
	if(ball().safeVelocity() != Vector(0, 0) &&
		(toTarget != Vector(0, 0) && abs(toTarget.angle() - self().bodyDirection()).degrees() <= 20)) {
		// $B$3$N3QEY$r0];}$7$?$^$^Av$C$F!"%\!<%k$r<h$l$k$+D4$Y$k!#(B
		const Vector ballInitial = ball().position();		// $B%\!<%k$N=i4|0LCV(B
		const Vector ballVelocity = ball().safeVelocity();	// $B%\!<%k$N=i4|B.EY(B
		double speed = ballVelocity.norm();					// $B%\!<%k$N%9%T!<%I(B
		double run = 0;										// $B%\!<%k$NAv$C$?5wN%(B
		Vector position = self().position();		// $B<+J,$N0LCV(B
		Vector preposition;							// $BD>A0$N<+J,$N0LCV(B
		Vector velocity = self().safeVelocity();	// $B<+J,$NB.EY(B
		double lastSign = ballVelocity.y() * (position.x() - ballInitial.x()) - ballVelocity.x() * (position.y() - ballInitial.y());
		Angle direction = self().bodyDirection();
		double stamina = self().stamina();
		int i=0;
		for(; i<cycles; i++) {
			double power = powerLimit(stamina, true, 2.0);
			stamina = stamina - power + server().stamina_inc_max();
			velocity += Vector(power * self().effort() * server().dash_power_rate(), direction);
			if(velocity.norm() > server().player_speed_max())
				velocity.normalize(server().player_speed_max());
			preposition = position;
			position += velocity;
			velocity *= server().player_decay();
			run += speed;
			speed *= server().ball_decay();
			
			double sign = ballVelocity.y() * (position.x() - ballInitial.x()) - ballVelocity.x() * (position.y() - ballInitial.y());
			if(sign * lastSign <= 0)
				break;
			lastSign = sign;
		}
		if(i < cycles) {
			double norm = (ballInitial - position).norm();
			if(norm <= run && run + speed*3 <= norm) {
				// turn $B$7$J$/$F$b$H$l$k$7!"(Bturn $B$7$F$k$H$H$l$J$=$&(B
				dashTo(position, 2.0);
				return;
			}
		}
	}
	dashTo(target, 2.0);
	return;
}

void Gullwing::free()
{
	if(m_moveTarget.valid()) {
		if(m_moveTimeLimit > world().time()) {
			dashTo(m_moveTarget, 2.0);
			return;
		}
		m_moveTarget = Vector::invalidValue();
	}

	m_freeCount++;
	
	// $B$?$^$K$O!"$^$o$j$r8+$k!#(B
	if((m_freeCount % 10) == 0) {
		int c = m_freeCount / 10;
		changeView(VIEW_WIDTH_WIDE, VIEW_QUALITY_HIGH);
		turnNeckTo(Angle(0));
		Angle direction = (c&1) ? Angle(90) : Angle(-90);
		direction += directionToNextBall();
		turn(direction - self().bodyDirection());
		int last = world().time();
		while(self().time() < last && m_moveTimeLimit <= world().time())
			rest();
		return;
	}

	// $B%]%8%7%g%K%s%0(B
	if(self().isGoalie()) {
		if(!isInMyHalfGoalArea(self().position())) {
			dashTo(Vector(-server().PITCH_LENGTH()/2 + server().GOAL_AREA_LENGTH()/4, 0), 1.2);
			return;
		}
	} else {
		if(self().uniformNumber() <= 3) {
			if(world().playMode() == PLAYMODE_GOALIE_CATCH_L) {
				double tmp = -server().PITCH_LENGTH()/2 + 1.5 * server().PENALTY_AREA_LENGTH();
				if(self().position().x() < tmp) {
					dashTo(Vector(tmp + server().kickable_area(), self().position().y()), 1.0);
				} else if(self().position().x() > tmp + 3 * server().kickable_area()) {
					dashTo(Vector(tmp + 2 * server().kickable_area(), self().position().y()), 1.0);
				} else {
					double tmp1 = getPositionValue(self().position() + Vector(server().player_size(), Angle(90)));
					double tmp2 = getPositionValue(self().position() + Vector(server().player_size(), Angle(-90)));
					if(tmp1 > tmp2)
						dashTo(self().position() + Vector(9.15, Angle(90)), 1.0);
					else
						dashTo(self().position() + Vector(9.15, Angle(-90)), 1.0);
				}
				return;
			} else if(ball().position().x() < ourOffside()) {
				if(m_mode != MODE_SETPLAYL) {
					if(!isInMyGoalArea(self().position())) {
						dashTo(Vector(-server().PITCH_LENGTH()/2 + server().GOAL_AREA_LENGTH()/4, 0), 2.0);
						return;
					}
				}
			}
		} else if(self().uniformNumber() == 8) {
			double dash = enemyOffside() - self().position().x();
			if(dash < 0)
				dash = 2.0;
			else
				dash = 1.0 + suppress(0.0, dash / (server().PITCH_LENGTH()/2), 1.0);
			dashTo(Vector(enemyOffside() - server().kickable_area(), server().PITCH_WIDTH()/4), dash);
			return;
		} else if(self().uniformNumber() == 9) {
			double dash = enemyOffside() - self().position().x();
			if(dash < 0)
				dash = 2.0;
			else
				dash = 1.0 + suppress(0.0, dash / (server().PITCH_LENGTH()/2), 1.0);
			dashTo(Vector(enemyOffside() - server().kickable_area(), -server().PITCH_WIDTH()/6), dash);
			return;
		}



		double value = getPositionValue(self().position());
		int best = -1;
		for(int i=0; i<360; i+=360/24) {
			double tmp = getPositionValue(self().position() + Vector(server().player_size(), Angle(i)));
			if(tmp > value) {
				value = tmp;
				best = i;
			}
		}
		if(best != -1) {
			dashTo(self().position() + Vector(9.15, Angle(best)), 1.0);
			return;
			
			/*Angle direction = Angle(best);
			changeView(VIEW_WIDTH_NORMAL, VIEW_QUALITY_HIGH);
			turnNeckTo(directionToBall - direction);
			turnTo(direction);

			dash(powerLimit2());
			return;*/
		}
	}

	changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
	turnNeckTo(Angle(0));
	turn(directionToNextBall() - self().bodyDirection());
}

void Gullwing::dashTo(Vector target, double dash)
{
	{
		double brim = server().kickable_area() * 0.80;
		double xmax = server().PITCH_LENGTH()/2 - brim;
		double ymax = server().PITCH_WIDTH()/2 - brim;
		double x = suppress(-xmax, target.x(), xmax);
		double y = suppress(-ymax, target.y(), ymax);
		target = Vector(x, y);

		// x, y $B$,$H$b$KHO0O30$N>l9g!"(Bserver().ckick_margin() $B$r8+$?$$(B
	}

	Vector toTarget = target - self().position();
	Angle directionToTarget = toTarget.angle();
	Angle directionToNextBallTargetRelative = directionToNextBall() - directionToTarget;

	double degrees = abs(directionToNextBallTargetRelative).degrees();
	bool fore = (degrees < 90 + self().viewWidthDegrees()*0.4);	// true=$BA0J}$X%@%C%7%e(B, false=$B%P%C%/%@%C%7%e(B
	Angle directionTurnTo = (fore ? directionToTarget : (directionToTarget + Angle(180)));

	// $B>c32J*$,$"$l$P!"$h$1$k!#(B
	Vector dashTo = self().position() + Vector(powerLimit(self().stamina(), true, dash) * server().kick_power_rate(), directionToTarget);
	Players::const_iterator it = others().begin();
	for(; it != others().end(); it++) {
		const Player& p = *it;
		if((p.position() - dashTo).norm() < server().player_size() * 2)
			break;
	}
	if(it != others().end()) {
		const Player& p = *it;
		Vector toPlayer = p.position() - self().position();
		Angle angle = toPlayer.angle() - directionToTarget;
		Angle slide = directionToTarget + ((angle.degrees() < 0) ? Angle(+60) : Angle(-60));
		if(abs(slide - self().bodyDirection()).degrees() >= 10) {
			changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
			turnNeckTo(directionToNextBall() - slide);
			turnTo(slide);
		} else {
			changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
			turnNeckTo(directionToNextBall() - self().bodyDirection());
			this->dash(powerLimit(self().stamina(), true, dash));
		}
		return;
	}

	// $BBN$N8~$-$rJd@5$9$k(B
	if(abs(directionTurnTo - self().bodyDirection()).degrees() >= 10) {
		changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
		turnNeckTo(directionToNextBall() - directionTurnTo);
		turnTo(directionTurnTo);
		return;
	}
	
	// $BAv$k(B
	double length = toTarget.norm();
	length = length / (1.0 + server().player_decay());	// $B#2%5%$%/%k$GE~Ce$9$k(B
	double power = length / self().effort() / server().dash_power_rate();
	if(fore)
		power = suppress(0.0, power, powerLimit(self().stamina(), fore, dash));
	else
		power = suppress(powerLimit(self().stamina(), fore, dash), -power, 0.0);

	changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
	turnNeckTo(directionToNextBall() - self().bodyDirection());
	this->dash(power);
}

void Gullwing::kickTwiceTo(double initialSpeed, Angle directionToTarget)
{
	// $B%\!<%k$NB.EY$,M=B,CM$J$i!"%\!<%k$r;_$a$F$_$k(B&$BBN$NJ}8~$r@0$($k(B
	if(!ball().velocity().valid()) {
		changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
		turnNeckTo(directionToBall() - self().bodyDirection());
		kick(-ball().safeVelocity());

		double sign = ((directionToBall() - directionToTarget).degrees() < 0) ? -1 : +1;
		Angle angle = Angle(sign * 45) + directionToTarget;
		turnNeckTo(Angle(sign * 90));
		turn(angle - self().bodyDirection());
	} else if(abs(directionToTarget - self().bodyDirection()).degrees() >= 90) {
		// $B=3$kJ}8~$XBN$r8~$1$k(B
		
		if(toNextBall().norm() >= server().kickable_area()) {
			// $B%\!<%k$,B.$1$l$P!"$$$C$?$s;_$a$k!#(B
			changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
			turnNeckTo(directionToBall() - self().bodyDirection());
			kick(-ball().safeVelocity());
			return;
		}
		
		changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
		turnNeckTo(directionToNextBall() - directionToTarget);
		turn(directionToTarget - self().bodyDirection());
		return;
	}

	// $B$^$:!"<+J,$N8eJ}$^$G%\!<%k$r1?$V(B
	{
		double sign = ((directionToBall() - directionToTarget).degrees() < 0) ? -1 : +1;
		Angle directionToSubtarget = Angle(sign * 120) + directionToTarget;
		double distanceToSubtarget = (server().kickable_margin() / 2) + server().player_size();

		turnBallTo(Vector(distanceToSubtarget, directionToSubtarget));
	}
	
	// 2$BCJ$2$j(BGO!!
	{
		changeView(VIEW_WIDTH_NARROW, VIEW_QUALITY_HIGH);
		turnNeckTo(directionToTarget - self().bodyDirection());
		kick(Vector(server().kickable_area(), directionToTarget) - ball().safeVelocity());
		kickTo(directionToTarget);	// TODO:$BNO$rD4@a$9$k$3$H!#(B
		findBall();
	}
}

void Gullwing::turnBallTo(Vector toTarget)
{
	for(;;) {
		double distanceToTarget = toTarget.norm();
		Angle directionToTarget = (distanceToTarget == 0) ? Angle(0) : toTarget.angle();

		double degrees = (directionToTarget - directionToBall()).degrees();
		double distance = distanceToTarget;
		Vector toCurrentTarget;
		int count = (int)abs(degrees / 30) + 1;
		for(; count<10; count++) {
			Angle angle = Angle(degrees / count) + directionToBall();
			toCurrentTarget = Vector(distance, angle);
			double x1 = toBall().x();
			double y1 = toBall().y();
			double x2 = toCurrentTarget.x();
			double y2 = toCurrentTarget.y();
			double d = abs(x1 * y2 - x2 * y1) / (toCurrentTarget - toBall()).norm();
			if(d > server().player_size() + server().ball_size())
				break;
			distance = (server().kickable_margin() / 2) + server().player_size();
		}
		turnNeckTo(toCurrentTarget.angle() - self().bodyDirection());
		kick(toCurrentTarget - toBall() - ball().safeVelocity());
		if(count == 1)
			break;
	}
}

void Gullwing::findBall()
{
	// $BA0Ds(B:$B%\!<%k$,;k3&$KF~$i$J$$!#(B
	class LostBall : GullwingPremise
	{
		PlayMode m_playMode;
		int m_time;
	public:
		LostBall(Gullwing* brain, int time) : GullwingPremise(brain)
		{
			m_time = time;
		}
		bool checkPremise() {
			return m_brain->ball().time() <= m_time;
		}
	} anonym(this, ball().time());
	
#if NEVER_THROW
#  error
#endif
	try {
#if 1
		// $B$^$:%T%C%A$NCf?4$r8+$kJ}<0(B
		changeView(VIEW_WIDTH_WIDE, VIEW_QUALITY_HIGH);
		turnNeckTo(Angle(0));
		Angle direction = (self().position() == Vector(0, 0)) ? Angle(0) : self().position().angle();
		turnTo(direction + Angle(180), true);
		turnTo(direction, true);
		Angle angle = Angle(self().viewWidthDegrees() * 0.9);
		for(;;)
			turn(angle, true);
#else
		// $B%\!<%k$NM=B,COE@$N<~0O$r%9%-%c%s$7$F$$$/J}<0(B
		changeView(VIEW_WIDTH_NORMAL, VIEW_QUALITY_HIGH);
		turnNeckTo(Angle(0));
		turnTo(directionToBall(), true);
		const double width = self().viewWidthDegrees() * 0.9;
		double degrees = 0;
		while(degrees < 180) {
			degrees = (degrees > 0) ? -degrees : (-degrees + width);
			turnTo(Angle(degrees) + directionToBall(), true);
		}
		for(;;)
			turn(Angle(width), true);
#endif
	} catch(PremiseException) {
	} 
}

void Gullwing::seeAround()
{
	changeView(VIEW_WIDTH_NORMAL, VIEW_QUALITY_HIGH);
	double degrees = 0;
	while(degrees < 360) {
		double turnDegrees = self().viewWidthDegrees() * 0.9;
		turnNeckTo(Angle(0));
		turn(Angle(turnDegrees), true);
		degrees += turnDegrees;
	}
}
