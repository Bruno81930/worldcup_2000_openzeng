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
	// 前提:プレイモードが変わらない。
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
    // アルゴリズム本体。for(;;) { act(); } されている。
	
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
	// キックオフの前に実行されるコードをここに記述する。
	
	// ランダムな位置へ move する。
	double x = -m_random.next() * server().PITCH_LENGTH()/2;
	double y = m_random.next() * server().PITCH_WIDTH() - server().PITCH_WIDTH()/2;
	move(Vector(x, y));
	
	// move したらあたりを見回す。
	for(;;) {
		// 視覚の幅より少し小さい角度だけ回転する。
		turn(Angle(self().viewWidthDegrees() * 0.9), true);
	}
	
	// 注：一見無限ループだが、Premise が崩れると、act の先頭からやり直す。
}

void Sample::normal()
{
	// 通常のプレイのコードをここに記述する。
	
	// ボールを見失ったら, 回転する。
	if(world().time() - ball().time() > 10) {
		// 視覚の幅より少し小さい角度だけ回転する。
		turn(Angle(self().viewWidthDegrees() * 0.9), true);
		// act の始めから処理をやり直す。
		return;
	}
	
	// 自分から, ボールへのベクトル
	Vector toBall = ball().position() - self().position();
	// 自分の体の向きを中心にした, ボールの位置の角度
	Angle directionToBall = toBall.angle() - self().bodyDirection();
	
	// ボールが近くにあれば, ゴールへ向けて蹴る。
	if(toBall.norm() <= server().kickable_area()) {
		// 自分から, ゴールへのベクトル
		Vector toGoal = Vector(server().PITCH_LENGTH()/2, 0) - self().position();
		// 自分の体の向きを中心にした, ゴールの位置の角度
		Angle direction = toGoal.angle() - self().bodyDirection();
		// ゴールへ向けて, 思いっきり蹴る。
		kick(server().maxpower(), direction);
		// act の始めから処理をやり直す。
		return;
	}
	
	// ボールの方を向く
	if(abs(directionToBall.degrees()) >= 15) {
		// ボールの方を向く
		turn(directionToBall);
		// act の始めから処理をやり直す。
		return;
	}
	
	// 他に近い味方がいれば, 譲る
	// ワールドモデルに記憶されている全てのプレイヤーに対して, 以下をしらべる。
	Players::const_iterator it = others().begin();
	for(; it != others().end(); ++it) {
		const Player& player = *it;
		
		// チームが分かっていて, 味方チームで,
		if(player.team() == SIDE_LEFT) {
			Vector himToBall = ball().position() - player.position();
			// 自分よりボールに近ければ
			if(himToBall.norm() < toBall.norm() * 0.9) {
				// act の始めから処理をやり直す。
				return;
			}
		}
	}
	
	// ボールへ向かって走る。
	dash(server().maxpower());
	
	// act の始めから処理をやり直す。
	return;
}
