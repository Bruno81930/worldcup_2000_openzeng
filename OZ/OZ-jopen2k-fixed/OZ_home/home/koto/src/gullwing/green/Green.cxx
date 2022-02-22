// Green.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "../common.hxx"
#include "Green.hxx"

#ifndef NDEBUG
#  include "Green.inl"
#endif

#include "../SayBuffer.hxx"

/////////////////////////////////////////////////////////////////////////////
// Green

#if LEARNING
void Green::received(const PlayerSenseBody& sbi)
{
	Monoworld<GreenBall, GreenPlayer, GreenSelf>::received(sbi);

	m_inspiration = sbi.m_inspiration;
	if(m_inspiration)
		m_inspirationSee = sbi.m_inspirationSee;

	if(config().logFile()) {
		config().log().set(sbi.refereeTimer());
		config().log().set(0, Side::invalidValue(), 0, ball().position(), Angle(0));
		config().log().set(1, self().team(), self().uniformNumber(), self().position(), self().bodyDirection(), true);
		int i = 2;
		Players::const_iterator it = others().begin();
		for(; it != others().end(); it++) {
			const Player& p = *it;
			config().log().set(i++, p.team(), p.uniformNumber(), p.position(), p.bodyDirection());
		}
		config().log().write();

		if(m_inspiration && sbi.refereeTimer() > 0) {
			{
				// 自分の位置の誤差を記録
				Vector correct = sbi.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).position();
				Vector estimated = self().position();
				Vector delta = estimated - correct;
				fprintf(config().logFile(), "mypos\t%.20lf\n", (double)delta.norm());
			}
			{
				// 自分の速度の誤差を記録
				Vector correct = sbi.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).velocity();
				Vector estimated = self().velocity();
				Vector delta = estimated - correct;
				fprintf(config().logFile(), "myvel\t%.20lf\n", (double)delta.norm());
			}
			//fflush(config().logFile());
		}
	}
}
#endif

void Green::received(const PlayerSee& si)
{
	tryToStep(INFORMATION_SEE, si.refereeTimer());
	updateSelf(si);
	updateBall(si);
	updateFlags(si);
	
	unsigned int i;
	const PlayerSeeObject* const* seens;
	Players::iterator it;
	
	// とりあえず、絶対座標に変換する。
	Vector positions[MAX_PLAYER * 2];
	for(i=0, seens = si.players(); *seens; i++, seens++) {
		const PlayerSeeObject& o = **seens;
		if(o.distance().valid()) {
			Vector delta = Vector(o.distance(), o.direction() + self().sightDirection());
			positions[i] = self().position() + delta;
		} else {
			positions[i] = Vector::invalidValue();
		}
	}
	
	// テーブルの初期化
	int players[2][MAX_PLAYER];		// チームと背番号から、m_others の添え字へのテーブル
	for(i=0; i<COUNT_OF(players); i++) {
		for(int j=0; j<COUNT_OF(players[i]); j++)
			players[i][j] = -1;
	}
	for(i=0, it=m_others.begin(); it != m_others.end(); i++, it++) {
		Player& p = *it;
		if(p.team().valid() && p.uniformNumber().valid()) {
			int t = (p.team() == SIDE_LEFT) ? 0 : 1;
			int u = p.uniformNumber();
			ASSERT(t < COUNT_OF(players) && u < COUNT_OF(players[t]));
			players[t][u] = i;
		}
	}

	std::vector<int> table(m_others.size());
	for(i=0; i<(int)table.size(); i++)
		table[i] = -2;
	bool used[MAX_PLAYER * 2] = { 0 };
	// table[i] == j >= 0 for some i  if and only if  used[j] == true
	
	// 現在のワールドモデルの others のそれぞれについて、
	// ・そのまま残す(table == -2)。
	// ・削除する(table == -1)。
	// ・更新する(table >= 0、table の値の see で更新する)。
	// のいずれかの処理を行う。また、see で得られる players のそれぞれについて、
	// ・ワールドモデルへ加える(used == false)
	// ・結果的に、既存のプレイヤーを更新する(used == true)
	// のいずれかの処理が行われる。
	
	// 最初に、明らかに一致するものを調べる。
	for(i=0, seens = si.players(); *seens; i++, seens++) {
		const PlayerSeeObject& o = **seens;
		if(o.team().valid() && o.uniformNumber().valid()) {
			int t = (o.team() == SIDE_LEFT) ? 0 : 1;
			int u = o.uniformNumber();
			ASSERT(t < COUNT_OF(players) && u < COUNT_OF(players[t]));
			if(players[t][u] != -1) {
				table[players[t][u]] = i;
				used[i] = true;
			}
		}
	}
	
	// 次に、一致しそうなものを調べる
	/*for(i=0, seens = si.players(); *seens; i++, seens++) {
		const PlayerSeeObject& o = **seens;
		if(!used[i]) {
		}
	}*/

	// 残像の削除
	const double halfWidthDegrees = self().viewWidthDegrees() / 2;
	for(i=0, it=m_others.begin(); it != m_others.end(); i++, it++) {
		Player& p = *it;
		if(table[i] == -2 && p.position().valid()) {
			table[i] = -1;
			Vector toPlayer = p.position() - self().position();
			if(toPlayer.norm() > server().visible_distance()) {
				Angle angle = toPlayer.angle() - self().sightDirection();
				if(abs(angle).degrees() >= halfWidthDegrees)
					table[i] = -2;
			}
		}
	}
	
	// 削除と更新
	for(i=0, it=m_others.begin(); it != m_others.end(); i++) {
		switch(table[i]) {
		default:
			it->update(self(), m_time, *si.players()[table[i]]);
			// 下へ続く
		case -2:
			it++;
			break;
		case -1:
			it = m_others.erase(it);
			break;
		}
	}
	// 追加
	for(i=0, seens = si.players(); *seens; i++, seens++) {
		if(!used[i]) {
			m_others.push_front(Player());
			m_others.front().initialize(self(), m_time, **seens);
		}
	}
}

void Green::updateBall(const PlayerSee& si)
{
	// ball の更新
	if(si.ball()) {
		bool avarage = false;
		if((ball().position() - self().position()).norm() > server().kickable_area()) {
			Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				if((ball().position() - it->position()).norm() <= server().kickable_area())
					break;
			}
			if(it == others().end())
				avarage = true;
		}
		if(avarage) {
			m_ball.average(self(), m_time, *si.ball());
		} else {
			//Vector anticipated = m_ball.position();
			m_ball.update(self(), m_time, *si.ball());
			/*if(!m_ball.velocity().valid()) {
				Vector toAnticipated = anticipated - self().position();
				Vector toNow = m_ball.position() - self().position();
				if(toLast != Vector(0, 0) && toNow != Vector(0, 0)) {
					if(abs(toLast.angle() - toNow.angle()).degrees() > 120)
						m_ball.setVelocity(m_ball.safeVelocity() * -0.1);
				}
			}*/
		}
	}
}

void Green::step() {
	Players::iterator it = m_others.begin();
	for(; it != m_others.end(); it++)
		it->step();
	m_self.step();
	m_ball.step(m_self);
}
