// White.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "White.hxx"

#ifndef NDEBUG
#  include "White.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// WhiteObject

WhiteObject::~WhiteObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// WhiteBall

double WhiteBall::size() const
{
	return server().ball_size();
}
double WhiteBall::decay() const
{
	return server().ball_decay();
}
double WhiteBall::weight() const
{
	return server().ball_weight();
}
double WhiteBall::maxSpeed() const
{
	return server().ball_speed_max();
}

/////////////////////////////////////////////////////////////////////////////
// WhitePlayer

double WhitePlayer::size() const
{
	return server().player_size();
}
double WhitePlayer::decay() const
{
	return server().player_decay();
}
double WhitePlayer::weight() const
{
	return server().player_weight();
}
double WhitePlayer::maxSpeed() const
{
	return server().player_speed_max();
}

/////////////////////////////////////////////////////////////////////////////
// WhiteSelf

/////////////////////////////////////////////////////////////////////////////
// White

/*
White::~White()
{
}

bool White::estimateDirection(const PlayerSee& si, const WhiteSelf& lastSelf, Angle& result)
{
	const PlayerSeeObject* line;
	int edge;
	
	const PlayerSeeObject* const* lines = si.lines();
	if(!lines[0]) {
		// ラインが１本も見えなかったら、計算できない。
		return false;
	} else if(!lines[1]) {
		// １本だけ見えたら、プレイヤーはフィールドの中にいる。
		line = lines[0];
		edge = line->edge();
	} else if(!lines[2]) {
		// ２本見えたら、プレイヤーはフィールドの外にいる。
		line = lines[0];
		edge = -line->edge();
	} else {
		// ３本以上見えたら、サーバーの仕様が変更された。
		ASSERT(false);
		return false;
	}
	
	ASSERT(line);
	double degrees;
	double lineDegrees = line->direction().degrees();
	if(lineDegrees == 0) {
		double lastSightDegrees = lastSelf.sightDirection().degrees();
		switch(edge) {
		case EDGE_TOP:
		case EDGE_BOTTOM:
			if(-90 <= lastSightDegrees && lastSightDegrees < +90)
				degrees = 0;
			else
				degrees = +180;
			break;
		case EDGE_RIGHT:
		case EDGE_LEFT:
			if(0 < lastSightDegrees)
				degrees = +90;
			else
				degrees = -90;
			break;
		default:
			ASSERT(false);
			return false;
		}
	} else if(fabs(lineDegrees) == 90) {
		switch(edge) {
		case EDGE_TOP:     degrees = -90;       break;
		case EDGE_RIGHT:   degrees = 0;         break;
		case EDGE_BOTTOM:  degrees = 90;        break;
		case EDGE_LEFT:    degrees = 180;       break;
		default:
			ASSERT(false);
			return false;
		}
	} else {
		switch(edge) {
		case EDGE_TOP:
			if(lineDegrees < 0)
				degrees = -lineDegrees - 180;
			else
				degrees = -lineDegrees;
			break;
		case EDGE_BOTTOM:
			if(lineDegrees < 0)
				degrees = -lineDegrees;
			else
				degrees = 180 - lineDegrees;
			break;
		case EDGE_LEFT:
			if(lineDegrees < 0)
				degrees = 90 - lineDegrees;
			else
				degrees = -90 - lineDegrees;
			break;
		case EDGE_RIGHT:
			if(lineDegrees < 0)
				degrees = -90 - lineDegrees;
			else
				degrees = 90 - lineDegrees;
			break;
		default:
			ASSERT(false);
			return false;
		}
	}
	result = Angle(degrees);

	return true;
}

bool White::estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result)
{
	double sum = 0.0;
	Vector vector(0, 0);
	const PlayerSeeObject* const* flags = si.flags();
	for(; *flags; flags++) {
		const PlayerSeeObject* o = *flags;
		if(o->distance().valid() && o->flagId().valid()) {
			ASSERT(abs(o->type()) == TYPE_FLAG);
			double distance = o->distance();
			Vector toFlag = Vector(distance, direction + o->direction());
			const double coefficient = 1.0 / (distance + 0.00001);
			Vector position = flagHelper.flagToPosition(o->flagId()) - toFlag;
			vector += position * coefficient;
			sum += coefficient;
		}
	}
	if(sum == 0.0)
		return false;
	
	result = vector / sum;
	return true;
}*/
