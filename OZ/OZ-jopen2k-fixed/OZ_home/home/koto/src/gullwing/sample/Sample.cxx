// Sample.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "../common.hxx"
#include "Sample.hxx"

#ifndef NDEBUG
#  include "Sample.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Sample

const char* getDefaultTeamName()
{
	return "Sample";
}

PlayerClient* newPlayerClient(const std::map<std::string, std::string>& parameters)
{
	return new Sample();
}

Sample::~Sample()
{
}

void Sample::go()
{
	// $BA0Ds(B:$B%W%l%$%b!<%I$,JQ$o$i$J$$!#(B
	class PlayModeConstant : SamplePremise
	{
		PlayMode m_playMode;
	public:
		PlayModeConstant(Sample* brain) : SamplePremise(brain)
		{
			m_playMode = m_brain->world().playMode();
		}
		bool checkPremise() {
			PlayMode playMode = m_brain->world().playMode();
			bool result = (m_playMode == playMode);
			m_playMode = playMode;
			return result;
		}
	} anonym(this);
	
	Monobrain<White>::go();
};

void Sample::act()
{
    // $B%"%k%4%j%:%`K\BN!#(Bfor(;;) { act(); } $B$5$l$F$$$k!#(B
	
	switch(world().playMode()) {
	case PLAYMODE_BEFORE_KICK_OFF:
	case PLAYMODE_GOAL_L:
	case PLAYMODE_GOAL_R:
		movable();
		break;
		
	case PLAYMODE_PLAY_ON:
	case PLAYMODE_DROP_BALL:
	case PLAYMODE_OFFSIDE_L:
	case PLAYMODE_OFFSIDE_R:
		normal();
		break;
		
	case PLAYMODE_TIME_OVER:
		return;
		
	case PLAYMODE_KICK_OFF_L:
	case PLAYMODE_KICK_IN_L:
	case PLAYMODE_FREE_KICK_L:
	case PLAYMODE_CORNER_KICK_L:
	case PLAYMODE_GOAL_KICK_L:
	case PLAYMODE_GOALIE_CATCH_L:
		normal();
		break;
		
	case PLAYMODE_KICK_OFF_R:
	case PLAYMODE_KICK_IN_R:
	case PLAYMODE_FREE_KICK_R:
	case PLAYMODE_CORNER_KICK_R:
	case PLAYMODE_GOAL_KICK_R:
	case PLAYMODE_GOALIE_CATCH_R:
		normal();
		break;
		
	default:
		ASSERT(false);
	}
}

void Sample::movable()
{
	// $B%-%C%/%*%U$NA0$K<B9T$5$l$k%3!<%I$r$3$3$K5-=R$9$k!#(B
	
	// $B%i%s%@%`$J0LCV$X(B move $B$9$k!#(B
	double x = -m_random.next() * server().PITCH_LENGTH()/2;
	double y = m_random.next() * server().PITCH_WIDTH() - server().PITCH_WIDTH()/2;
	move(Vector(x, y));
	
	// move $B$7$?$i$"$?$j$r8+2s$9!#(B
	for(;;) {
		// $B;k3P$NI}$h$j>/$7>.$5$$3QEY$@$12sE>$9$k!#(B
		turn(Angle(self().viewWidthDegrees() * 0.9), true);
	}
	
	// $BCm!'0l8+L58B%k!<%W$@$,!"(BPremise $B$,Jx$l$k$H!"(Bact $B$N@hF,$+$i$d$jD>$9!#(B
}

void Sample::normal()
{
	// $BDL>o$N%W%l%$$N%3!<%I$r$3$3$K5-=R$9$k!#(B
	
	// $B%\!<%k$r8+<:$C$?$i(B, $B2sE>$9$k!#(B
	if(world().time() - ball().time() > 10) {
		// $B;k3P$NI}$h$j>/$7>.$5$$3QEY$@$12sE>$9$k!#(B
		turn(Angle(self().viewWidthDegrees() * 0.9), true);
		// act $B$N;O$a$+$i=hM}$r$d$jD>$9!#(B
		return;
	}
	
	// $B<+J,$+$i(B, $B%\!<%k$X$N%Y%/%H%k(B
	Vector toBall = ball().position() - self().position();
	// $B<+J,$NBN$N8~$-$rCf?4$K$7$?(B, $B%\!<%k$N0LCV$N3QEY(B
	Angle directionToBall = toBall.angle() - self().bodyDirection();
	
	// $B%\!<%k$,6a$/$K$"$l$P(B, $B%4!<%k$X8~$1$F=3$k!#(B
	if(toBall.norm() <= server().kickable_area()) {
		// $B<+J,$+$i(B, $B%4!<%k$X$N%Y%/%H%k(B
		Vector toGoal = Vector(server().PITCH_LENGTH()/2, 0) - self().position();
		// $B<+J,$NBN$N8~$-$rCf?4$K$7$?(B, $B%4!<%k$N0LCV$N3QEY(B
		Angle direction = toGoal.angle() - self().bodyDirection();
		// $B%4!<%k$X8~$1$F(B, $B;W$$$C$-$j=3$k!#(B
		kick(server().maxpower(), direction);
		// act $B$N;O$a$+$i=hM}$r$d$jD>$9!#(B
		return;
	}
	
	// $B%\!<%k$NJ}$r8~$/(B
	if(abs(directionToBall.degrees()) >= 15) {
		// $B%\!<%k$NJ}$r8~$/(B
		turn(directionToBall);
		// act $B$N;O$a$+$i=hM}$r$d$jD>$9!#(B
		return;
	}
	
	// $BB>$K6a$$L#J}$,$$$l$P(B, $B>y$k(B
	// $B%o!<%k%I%b%G%k$K5-21$5$l$F$$$kA4$F$N%W%l%$%d!<$KBP$7$F(B, $B0J2<$r$7$i$Y$k!#(B
	Players::const_iterator it = others().begin();
	for(; it != others().end(); ++it) {
		const Player& player = *it;
		
		// $B%A!<%`$,J,$+$C$F$$$F(B, $BL#J}%A!<%`$G(B,
		if(player.team() == SIDE_LEFT) {
			Vector himToBall = ball().position() - player.position();
			// $B<+J,$h$j%\!<%k$K6a$1$l$P(B
			if(himToBall.norm() < toBall.norm() * 0.9) {
				// act $B$N;O$a$+$i=hM}$r$d$jD>$9!#(B
				return;
			}
		}
	}
	
	// $B%\!<%k$X8~$+$C$FAv$k!#(B
	dash(server().maxpower());
	
	// act $B$N;O$a$+$i=hM}$r$d$jD>$9!#(B
	return;
}
