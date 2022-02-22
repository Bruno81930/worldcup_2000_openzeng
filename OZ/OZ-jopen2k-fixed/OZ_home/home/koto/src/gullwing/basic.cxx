// basic.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "basic.hxx"

#ifndef NDEBUG
#  include "basic.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayModeHelper

PlayModeHelper playModeHelper;

const char* const PlayModeHelper::m_playmodeStrings[PLAYMODE_MAZ] = {
	"kick_off_l",
		"kick_off_r",
		"kick_in_l",
		"kick_in_r",
		"free_kick_l",
		"free_kick_r",
		"corner_kick_l",
		"corner_kick_r",
		"goal_kick_l",
		"goal_kick_r",
		"goal_l",
		"goal_r",
		"offside_l",
		"offside_r",
		"goalie_catch_ball_l",
		"goalie_catch_ball_r",
		"before_kick_off",
		"time_over",
		"play_on",
		"drop_ball",
};

PlayModeHelper::~PlayModeHelper()
{
}
PlayModeHelper::PlayModeHelper()
{
}


/////////////////////////////////////////////////////////////////////////////
// RefreeMessageHelper

RefreeMessageHelper refreeMessageHelper;

const char* const RefreeMessageHelper::m_refreeMessageStrings[] = {
	"time_up_without_a_team",
		"time_up",
		"time_extended",
		"half_time",
		0,
		"faul_l",
		"faul_r",
		"goalie_catch_ball_l",
		"goalie_catch_ball_r",
		0,
};


/////////////////////////////////////////////////////////////////////////////
// FlagHelper

const Flag FlagHelper::m_flagToSymmetric[] = {
	FLAG_RB, FLAG_CB, FLAG_LB,
		GOAL_R,  FLAG_C,  GOAL_L,
		FLAG_RT, FLAG_CT, FLAG_LT,
		
		FLAG_PRB, FLAG_PLB,
		FLAG_PRC, FLAG_PLC,
		FLAG_PRT, FLAG_PLT,
		
		FLAG_GRB, FLAG_GLB,
		FLAG_GRT, FLAG_GLT,
		
		FLAG_BR50, FLAG_BR40, FLAG_BR30, FLAG_BR20, FLAG_BR10, FLAG_B0, FLAG_BL10, FLAG_BL20, FLAG_BL30, FLAG_BL40, FLAG_BL50,
		FLAG_TR50, FLAG_TR40, FLAG_TR30, FLAG_TR20, FLAG_TR10, FLAG_T0, FLAG_TL10, FLAG_TL20, FLAG_TL30, FLAG_TL40, FLAG_TL50,
		
		FLAG_RB30, FLAG_RB20, FLAG_RB10, FLAG_R0, FLAG_RT10, FLAG_RT20, FLAG_RT30,
		FLAG_LB30, FLAG_LB20, FLAG_LB10, FLAG_L0, FLAG_LT10, FLAG_LT20, FLAG_LT30,
};

const char* const FlagHelper::m_flagNames[] = {
	"l t", "c t", "r t", "l", "c", "r", "l b", "c b", "r b",
		"p l t", "p r t", "p l c", "p r c", "p l b", "p r b",
		"g l t", "g r t", "g l b", "g r b",
		"t l 50", "t l 40", "t l 30", "t l 20", "t l 10", "t 0", "t r 10", "t r 20", "t r 30", "t r 40", "t r 50",
		"b l 50", "b l 40", "b l 30", "b l 20", "b l 10", "b 0", "b r 10", "b r 20", "b r 30", "b r 40", "b r 50",
		"l t 30", "l t 20", "l t 10", "l 0", "l b 10", "l b 20", "l b 30",
		"r t 30", "r t 20", "r t 10", "r 0", "r b 10", "r b 20", "r b 30",
};

FlagHelper flagHelper;

FlagHelper::FlagHelper()
{
#ifndef NDEBUG
	for(int i=0; i<FLAG_MAZ; i++) {
		ASSERT(i == m_flagToSymmetric[m_flagToSymmetric[i]]);
	}
#endif
	m_initialized = false;
}
void FlagHelper::initialize(double goalWidth)
{
	ASSERT(goalWidth > 0);
	m_initialized = true;
	int x, y, i;
	for(y=0; y<3; y++) {
		for(x=0; x<3; x++) {
			i = FLAG_LT + y*3 + x;
			m_flagToPosition[i] = Vector((x-1) * server().PITCH_LENGTH()/2,
				(y-1) * server().PITCH_WIDTH()/2);
		}
	}
	for(y=0; y<3; y++) {
		for(x=0; x<2; x++) {
			i = FLAG_PLT + y*2 + x;
			m_flagToPosition[i] = Vector((x*2-1) * (server().PITCH_LENGTH()/2 - server().PENALTY_AREA_LENGTH()),
				(y-1) * server().PENALTY_AREA_WIDTH()/2);
		}
	}
	for(y=0; y<2; y++) {
		for(x=0; x<2; x++) {
			i = FLAG_GLT + y*2 + x;
			m_flagToPosition[i] = Vector((x*2-1) * server().PITCH_LENGTH()/2,
				(y*2-1) * goalWidth/2);
		}
	}
	for(y=0; y<2; y++) {
		for(x=0; x<11; x++) {
			i = FLAG_TL50 + y*11 + x;
			m_flagToPosition[i] = Vector((x-5) * 10.0,
				(y*2-1) * (server().PITCH_WIDTH()/2 + 5.0));
		}
	}
	for(y=0; y<7; y++) {
		for(x=0; x<2; x++) {
			i = FLAG_LT30 + y + x*7;
			m_flagToPosition[i] = Vector((x*2-1) * (server().PITCH_LENGTH()/2 + 5.0),
				(y-3) * 10.0);
		}
	}
}

static Flag nameToFlagSimple(const char* name)
{
	ASSERT(name[0] != '\0');
	int p1 = name[0];
	int p2 = (name[1] == '\0') ? 0 : name[2];
	int p3 = (p2 == 0 || name[3] == '\0') ? 0 : name[4];
	switch(p1) {
	case 'c':
		switch(p2) {
		case 't':		return FLAG_CT;
		case 0:			return FLAG_C;
		case 'b':		return FLAG_CB;
		}
		break;
	case 'p':
		switch(p2) {
		case 'l':
			switch(p3) {
			case 't':	return FLAG_PLT;
			case 'c':	return FLAG_PLC;
			case 'b':	return FLAG_PLB;
			}
			break;
		case 'r':
			switch(p3) {
			case 't':	return FLAG_PRT;
			case 'c':	return FLAG_PRC;
			case 'b':	return FLAG_PRB;
			}
			break;
		}
		break;
	case 'g':
		switch(p2) {
		case 'r':
			switch(p3) {
			case 't':	return FLAG_GRT;
			case 'b':	return FLAG_GRB;
			}
			break;
		case 'l':
			switch(p3) {
			case 't':	return FLAG_GLT;
			case 'b':	return FLAG_GLB;
			}
			break;
		}
		break;
	case 'l':
		switch(p2) {
		case 0:			return GOAL_L;
		case 't':
			switch(p3) {
			case 0:		return FLAG_LT;
			case '3':	return FLAG_LT30;
			case '2':	return FLAG_LT20;
			case '1':	return FLAG_LT10;
			}
			break;
		case '0':		return FLAG_L0;
		case 'b':
			switch(p3) {
			case '1':	return FLAG_LB10;
			case '2':	return FLAG_LB20;
			case '3':	return FLAG_LB30;
			case 0:		return FLAG_LB;
			}
			break;
		}
		break;
	case 'r':
		switch(p2) {
		case 0:			return GOAL_R;
		case 't':
			switch(p3) {
			case 0:		return FLAG_RT;
			case '3':	return FLAG_RT30;
			case '2':	return FLAG_RT20;
			case '1':	return FLAG_RT10;
			}
			break;
		case '0':		return FLAG_R0;
		case 'b':
			switch(p3) {
			case '1':	return FLAG_RB10;
			case '2':	return FLAG_RB20;
			case '3':	return FLAG_RB30;
			case 0:		return FLAG_RB;
			}
			break;
		}
		break;
	case 't':
		switch(p2) {
		case 'l':
			switch(p3) {
			case '5':	return FLAG_TL50;
			case '4':	return FLAG_TL40;
			case '3':	return FLAG_TL30;
			case '2':	return FLAG_TL20;
			case '1':	return FLAG_TL10;
			}
			break;
		case '0':		return FLAG_T0;
		case 'r':
			switch(p3) {
			case '1':	return FLAG_TR10;
			case '2':	return FLAG_TR20;
			case '3':	return FLAG_TR30;
			case '4':	return FLAG_TR40;
			case '5':	return FLAG_TR50;
			}
			break;
		}
		break;
	case 'b':
		switch(p2) {
		case 'l':
			switch(p3) {
			case '5':	return FLAG_BL50;
			case '4':	return FLAG_BL40;
			case '3':	return FLAG_BL30;
			case '2':	return FLAG_BL20;
			case '1':	return FLAG_BL10;
			}
			break;
		case '0':		return FLAG_B0;
		case 'r':
			switch(p3) {
			case '1':	return FLAG_BR10;
			case '2':	return FLAG_BR20;
			case '3':	return FLAG_BR30;
			case '4':	return FLAG_BR40;
			case '5':	return FLAG_BR50;
			}
			break;
		}
		break;
	}
	ASSERT(false);
	return FLAG_C;
}

Flag FlagHelper::nameToFlag(const char* name, Side teamSide)
{
	ASSERT(name);
	ASSERT(teamSide == SIDE_LEFT || teamSide == SIDE_RIGHT);
#if 1
	Flag flag = nameToFlagSimple(name);
	ASSERT(0 <= flag && flag < FLAG_MAZ);
	ASSERT(strcmp(name, m_flagNames[flag]) == 0);
	if(teamSide == SIDE_RIGHT)
		flag = m_flagToSymmetric[flag];
	return flag;
#else
	int i = 0;
	for(;i < FLAG_MAZ ; i++) {
		if(strcmp(name, m_flagNames[i]) == 0) {
			if(teamSide == SIDE_RIGHT)
				i = m_flagToSymmetric[i];
			ASSERT(0 <= i && i < FLAG_MAZ);
			return (Flag)(Flag_t)i;
		}
	}
	ASSERT(false);
	return FLAG_C;
#endif
}
