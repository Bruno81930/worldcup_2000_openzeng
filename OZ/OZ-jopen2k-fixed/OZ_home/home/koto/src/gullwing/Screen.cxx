// Screen.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Screen.hxx"

#ifndef NDEBUG
#  include "Screen.inl"
#endif

#include "basic.hxx"
#include "Server.hxx"

/////////////////////////////////////////////////////////////////////////////
// Screen

Screen::~Screen()
{
}

void Screen::rectangle(double x0, double y0, double x1, double y1)
{
	line(x0, y0, x1, y0);
	line(x1, y0, x1, y1);
	line(x1, y1, x0, y1);
	line(x0, y1, x0, y0);
}

void Screen::paintField()
{
	const double l = -server().PITCH_LENGTH() / 2;
	const double r = server().PITCH_LENGTH() / 2;
	const double t = -server().PITCH_WIDTH() / 2;
	const double b = server().PITCH_WIDTH() / 2;
	const double rad = acos((server().PENALTY_AREA_LENGTH() - server().PENALTY_SPOT_DIST()) / server().CENTER_CIRCLE_R());
	const double pi = 3.14159265358979323846;
	
	color(colorBackground());
	fillBackground();
	
	color(colorLine());
	// 外枠
	rectangle(l, t, r, b);
	// センターライン
	line(0, t, 0, b);
	// センターサークル
	circle(0, 0, server().CENTER_CIRCLE_R());
	// ペナルティエリア
	rectangle(l, -server().PENALTY_AREA_WIDTH()/2,
		l + server().PENALTY_AREA_LENGTH(), server().PENALTY_AREA_WIDTH()/2);
	rectangle(r - server().PENALTY_AREA_LENGTH(), -server().PENALTY_AREA_WIDTH()/2,
		r, server().PENALTY_AREA_WIDTH()/2);
	// ゴールエリア
	rectangle(l, -server().GOAL_AREA_WIDTH()/2,
		l + server().GOAL_AREA_LENGTH(), server().GOAL_AREA_WIDTH()/2);
	rectangle(r - server().GOAL_AREA_LENGTH(), -server().GOAL_AREA_WIDTH()/2,
		r, server().GOAL_AREA_WIDTH()/2);
	// ゴール
	rectangle(l - server().GOAL_DEPTH(), -server().goal_width()/2,
		l, server().goal_width()/2);
	rectangle(r, -server().goal_width()/2,
		r + server().GOAL_DEPTH(), server().goal_width()/2);
	// ペナルティアーク
	arc(l + server().PENALTY_SPOT_DIST(), 0, server().CENTER_CIRCLE_R(), -rad, rad);
	arc(r - server().PENALTY_SPOT_DIST(), 0, server().CENTER_CIRCLE_R(), -rad+pi, rad+pi);
	// コーナーアーク
	arc(l, b, server().CORNER_ARC_R(), -pi/2, 0);
	arc(l, t, server().CORNER_ARC_R(),     0, pi/2);
	arc(r, t, server().CORNER_ARC_R(),  pi/2, pi);
	arc(r, b, server().CORNER_ARC_R(),    pi, -pi/2);
	// フラグ
	color(colorFlag());
	for(int i=0; i<FLAG_MAZ; i++) {
		const Vector& v = flagHelper.flagToPosition(i);
		const double d = 0.5;
		line(v.x() - d, v.y() - d, v.x() + d, v.y() + d);
		line(v.x() + d, v.y() - d, v.x() - d, v.y() + d);
	}
}
