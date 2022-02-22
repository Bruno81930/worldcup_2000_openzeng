// Blue.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "../common.hxx"
#include "Blue.hxx"

#ifndef NDEBUG
#  include "Blue.inl"
#endif

#if OS_TYPE == WINDOWS
#include <process.h>
inline int getpid() {
	return _getpid();
}
#endif
#include "time.h"

/////////////////////////////////////////////////////////////////////////////
// Blue

Blue::~Blue()
{
	if(m_fp)
		fclose(m_fp);
}

void Blue::initialize(PlayMode playMode, int uniformNumber, bool goalie)
{
	Monoworld<WhiteBall, WhitePlayer, WhiteSelf>::initialize(playMode, uniformNumber, goalie);

	time_t t;
	::time(&t);
#ifdef _MSC_VER
	long s = ::timeGetTime();
#else
	long s = ::getpid();
#endif
	char filename[512];
	sprintf(filename, "_%d_%ld_%d.log", (int)t, (long)s, (int)uniformNumber);
	m_fp = fopen(filename, "wt");
}

void Blue::received(const PlayerSenseBody& sbi)
{
	Monoworld<WhiteBall, WhitePlayer, WhiteSelf>::received(sbi);
#if LEARNING
	if(m_fp) {
		if(sbi.m_inspiration && sbi.refereeTimer() > 0) {
			fprintf(m_fp, "%d\n", (int)sbi.refereeTimer());
			{
				// $B<+J,$N0LCV$N8m:9$r5-O?(B
				Vector correct = sbi.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).position();
				Vector estimated = self().position();
				Vector delta = estimated - correct;
				fprintf(m_fp, "sb_pos\t%.20lf\n", (double)delta.norm());
			}
			{
				// $B<+J,$NB.EY$N8m:9$r5-O?(B
				Vector correct = sbi.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).velocity();
				Vector estimated = self().safeVelocity();
				Vector delta = estimated - correct;
				fprintf(m_fp, "sb_vel\t%.20lf\n", (double)delta.norm());
			}
			//fflush(m_fp);
		}
	}
#endif
}
void Blue::received(const PlayerSee& si)
{
	Monoworld<WhiteBall, WhitePlayer, WhiteSelf>::received(si);

#if LEARNING
	if(m_fp) {
		if(si.m_inspiration && si.refereeTimer() > 0) {
			fprintf(m_fp, "%d\n", (int)si.refereeTimer());
			{
				// $B<+J,$N0LCV$N8m:9$r5-O?(B
				Vector correct = si.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).position();
				Vector estimated = self().position();
				Vector delta = estimated - correct;
				fprintf(m_fp, "see_pos\t%.20lf\n", (double)delta.norm());
			}
			{
				// $B<+J,$NB.EY$N8m:9$r5-O?(B
				Vector correct = si.m_inspirationSee.player(SIDE_LEFT, self().uniformNumber()).velocity();
				Vector estimated = self().safeVelocity();
				Vector delta = estimated - correct;
				fprintf(m_fp, "see_vel\t%.20lf\n", (double)delta.norm());
			}
			//fflush(m_fp);
		}
	}
#endif
}
void Blue::updateSelf(const PlayerSee& si)
{
	// $BBN$N8~$-$N99?7(B
	Angle direction;
	if(estimateDirection(si, self(), direction)) {
		m_self.updateSightDirection(direction);
		ASSERT(abs(self().sightDirection() - direction).degrees() <= 0.001);
	}
	// $B0LCV$N99?7(B
	Vector position;
	if(estimatePosition(si, self().sightDirection(), position)) {
		m_self.updatePosition(position);
	}
	// $BB.EY$N99?7(B
	if(parameter >= 4.0) {
	} else if(parameter >= 3.0) {
		// $B$b$C$H$b6a$$$b$N$+$i5a$a$k(B
		double minDistance = DBL_MAX;
		const PlayerSeeObject* minFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->distanceChange().valid() && o->flagId().valid()) {
				if(o->distance() < minDistance) {
					minDistance = o->distance();
					minFlag = o;
				}
			}
		}
		if(minFlag != 0) {
			const PlayerSeeObject* flag = minFlag;
			Vector position = Vector(flag->distance(), flag->direction() + self().sightDirection());
			double distance = position.norm();
			Vector e = position / distance;
			double x = flag->directionChange() * e.x() - flag->directionChange() * (PI/180) * distance * e.y();
			double y = flag->directionChange() * e.y() - flag->directionChange() * (PI/180) * distance * e.x();
			//double coefficient = 1/(distance+0.001);
			m_self.updateVelocity(Vector(-x, -y));
		}
	} else if(parameter >= 2.0) {
		// $B=E$_IU$-J?6Q$r<h$k(B($B5wN%$KH?HfNc$N=E$_(B)
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* flag = *flags;
			if(flag->distanceChange().valid()) {
				Vector position = Vector(flag->distance(), flag->direction() + self().sightDirection());
				double distance = position.norm();
				Vector e = position / distance;
				double x = flag->directionChange() * e.x() - flag->directionChange() * (PI/180) * distance * e.y();
				double y = flag->directionChange() * e.y() - flag->directionChange() * (PI/180) * distance * e.x();
				double coefficient = 1/(distance+0.001);
				sum += coefficient;
				vector += Vector(-x, -y) * coefficient;
			}
		}
		if(sum != 0.0) {
			m_self.updateVelocity(vector / sum);
		}
	} else {
		// $B=E$_IU$-J?6Q$r<h$k(B($BDj?t$N5wN%>h$N=E$_(B)
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* flag = *flags;
			if(flag->distanceChange().valid()) {
				Vector position = Vector(flag->distance(), flag->direction() + self().sightDirection());
				double distance = position.norm();
				Vector e = position / distance;
				double x = flag->directionChange() * e.x() - flag->directionChange() * (PI/180) * distance * e.y();
				double y = flag->directionChange() * e.y() - flag->directionChange() * (PI/180) * distance * e.x();
				//double coefficient = 1/(distance+0.001);
				double coefficient = pow(parameter, distance);
				sum += coefficient;
				vector += Vector(-x, -y) * coefficient;
			}
		}
		if(sum != 0.0) {
			m_self.updateVelocity(vector / sum);
		}
	}
	// $B;~9o$N99?7(B
	m_self.updateTime(m_time);
}

bool Blue::estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result)
{
	if(parameter >= 4.0) {
		// $B:81&#4#5EY$K6a$$%U%i%0#2K\$+$i@~$r?-$P$7!"8rE@$N:BI8$r:NMQ$9$kJ}<0(B
		ASSERT(direction.valid());
		double leftValue = DBL_MAX;
		const PlayerSeeObject* leftFlag = 0;
		double rightValue = DBL_MAX;
		const PlayerSeeObject* rightFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->direction().valid() && o->flagId().valid()) {
				if(o->distance() == 0) {
					result = flagHelper.flagToPosition(o->flagId());
					return true;
				}
				double degrees = o->direction().degrees();
				if(abs(degrees - -45) < leftValue) {
					leftValue = abs(degrees - -45);
					leftFlag = o;
				}
				if(abs(degrees - +45) < rightValue) {
					rightValue = abs(degrees - +45);
					rightFlag = o;
				}
			}
		}
		if(leftFlag != 0 && (rightFlag->direction() - leftFlag->direction()).degrees() >= 30) {
			Vector left = flagHelper.flagToPosition(leftFlag->flagId());
			Vector right = flagHelper.flagToPosition(rightFlag->flagId());
			Vector leftToRight = right - left;
			Vector rightToLeft = -leftToRight;
			double l = leftToRight.norm();
			Angle alpha = (leftFlag->direction() + direction) - rightToLeft.angle();
			Angle beta = leftToRight.angle() - (rightFlag->direction() + direction);
			//ASSERT(-0.001 <= alpha.degrees() && alpha.degrees() <= 180.001);
			//ASSERT(-0.001 <= beta.degrees() && beta.degrees() <= 180.001);
			if(0 < alpha.degrees() && alpha.degrees() < 180 && 0 < beta.degrees() && beta.degrees() < 180) {
				// $B<!$NO"N)J}Dx<0$r2r$/(B
				// p*sin(alpha) == q*sin(beta)       -- [1]
				// p*cos(alpha) + q*cos(beta) == l   -- [2]
				//
				// [1] $B$N:81&$rF~$l49$($?$b$N$r(B [3] $B$H$9$k!#(B
				// [2]*sin(b) - [3]*cos(b): p*cos(a)*sin(b) == l*sin(b) - p*sin(a)*cos(b)
				//                          p == l * sin(b) / (cos(a)*sin(b) + sin(a)*cos(b))
				double sa = sin(alpha);
				double ca = cos(alpha);
				double sb = sin(beta);
				double cb = cos(beta);
				double p = l * sb / (ca * sb + sa * cb);
				ASSERT(p > 0);
				Vector delta = Vector(-p, leftFlag->direction() + direction);
				result = left + delta;
				return true;
			}
		}
		// $B$3$NJ}K!$G$O7W;;$G$-$J$$!#2<$XB3$/!#(B
	}
	if(parameter >= 3.0) {
		// $B$b$C$H$b6a$$$b$N$+$i5a$a$kJ}<0(B
		ASSERT(direction.valid());
		double minDistance = DBL_MAX;
		const PlayerSeeObject* minFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->flagId().valid()) {
				if(o->distance() < minDistance) {
					minDistance = o->distance();
					minFlag = o;
				}
			}
		}
		if(minFlag == 0)
			return false;
		
		Vector toFlag = Vector(minDistance, direction + minFlag->direction());
		result = flagHelper.flagToPosition(minFlag->flagId()) - toFlag;
		return true;
	} else if(parameter >= 2.0) {
		// $B=E$_IU$-J?6Q$r<h$k(B($B5wN%$KH?HfNc$N=E$_(B)
		ASSERT(direction.valid());
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->flagId().valid()) {
				if(o->distance() == 0) {
					result = flagHelper.flagToPosition(o->flagId());
					return true;
				}
				ASSERT(abs(o->type()) == TYPE_FLAG);
				double distance = o->distance();
				Vector toFlag = Vector(distance, direction + o->direction());
				double coefficient = 1/(distance+0.001);
				Vector position = flagHelper.flagToPosition(o->flagId()) - toFlag;
				vector += position * coefficient;
				sum += coefficient;
			}
		}
		if(sum == 0.0)
			return false;
		
		result = vector / sum;
		return true;
	} else {
		// $B=E$_IU$-J?6Q$r<h$k(B($BDj?t$N5wN%>h$N=E$_(B)
		ASSERT(direction.valid());
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->flagId().valid()) {
				if(o->distance() == 0) {
					result = flagHelper.flagToPosition(o->flagId());
					return true;
				}
				ASSERT(abs(o->type()) == TYPE_FLAG);
				double distance = o->distance();
				Vector toFlag = Vector(distance, direction + o->direction());
				double coefficient = pow(parameter, distance);
				Vector position = flagHelper.flagToPosition(o->flagId()) - toFlag;
				vector += position * coefficient;
				sum += coefficient;
			}
		}
		if(sum == 0.0)
			return false;
		
		result = vector / sum;
		return true;
	}
}
