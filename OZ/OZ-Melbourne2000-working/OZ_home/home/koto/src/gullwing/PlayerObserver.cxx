// PlayerObserver.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "PlayerObserver.hxx"

#ifndef NDEBUG
#  include "PlayerObserver.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerObserver

PlayerObserver::~PlayerObserver()
{
}

void PlayerObserver::intoWaiting()
{
}
void PlayerObserver::outOfWaiting()
{
}

void PlayerObserver::initialize(PlayMode /*playMode*/, int uniformNumber, bool /*goalie*/)
{
	ASSERT(0 <= uniformNumber && uniformNumber < MAX_PLAYER);
}
void PlayerObserver::connectionBroken()
{
}

void PlayerObserver::received(const PlayerSenseBody& /*sbi*/)
{
}
void PlayerObserver::received(const PlayerSee& /*si*/)
{
}
void PlayerObserver::received(const PlayerHear& /*hi*/, int /*key*/)
{
}

void PlayerObserver::turned(double moment)
{
	ASSERT(server().minmoment() <= moment && moment <= server().maxmoment());
}
void PlayerObserver::dashed(double power)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
}
void PlayerObserver::kicked(double power, Angle direction)
{
	ASSERT(server().minpower() <= power && power <= server().maxpower());
	ASSERT(server().minmoment() <= direction.degrees() && direction.degrees() <= server().maxmoment());
}
void PlayerObserver::ballCaught(Angle direction)
{
	ASSERT(server().minmoment() <= direction.degrees() && direction.degrees() <= server().maxmoment());
}
void PlayerObserver::moved(Vector /*position*/)
{
}

void PlayerObserver::neckTurned(Angle angle)
{
	ASSERT(server().minneckmoment() <= angle.degrees() && angle.degrees() <= server().maxneckmoment());
}
void PlayerObserver::said(const char* /*m_message*/)
{
}
void PlayerObserver::viewChanged(ViewWidth /*width*/, ViewQuality /*quality*/)
{
}
