// CoachSee.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(COACHSEE_HXX__INCLUDED) // {
#define COACHSEE_HXX__INCLUDED

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// CoachSee

class CoachSeeObject
{
private:
	bool m_valid;
	ObjectType m_type;
	Side m_team;
	Integer m_uniformNumber;
	Vector m_position;
	Vector m_velocity;
	Angle m_bodyDirection;
	Angle m_neckAngle;
	
public:
	bool valid() const;
	ObjectType type() const;
	Side team() const;
	int uniformNumber() const;
	Vector position() const;
	Vector velocity() const;
	Angle bodyDirection() const;
	Angle neckAngle() const;

	CoachSeeObject();
	
	void initialize();
	/**
	* Soccer Server から得たメッセージから、オブジェクトを初期化する。
	* @param	cursor
	*		[in]	see メッセージ中の、CoachSeeObject 全体をくくる（開き）括弧への
	*				ポインタ。
	*		[out]	対応する閉じ括弧の左のスペースの左の文字へのポインタ。
	* @param	teamName
	*		チーム名。
	* @param	teamSide
	*		チームのサイド。
	*/
	void initialize(const char* &cursor, const char* teamName, Side teamSide);
};

class CoachSee
{
public:
	typedef std::vector<CoachSeeObject> Objects;

private:
	/*CoachSee(const CoachSee&);
	CoachSee& operator= (const CoachSee&);
	bool operator== (const CoachSee&) const;*/
	
private:
	int m_refreeTimer;
	CoachSeeObject m_goals[2];
	CoachSeeObject m_ball;
	CoachSeeObject m_players[2][MAX_PLAYER];
	
public:
	const CoachSeeObject& ball() const;
	const CoachSeeObject& player(Side team, int uniformNumber) const;
	
	CoachSee();
	
	/**
	* Soccer Server から得たメッセージから、オブジェクトを初期化する。
	* @param	body
	*		Soccer Server から得た see メッセージの、time パラメタの先頭の文字
	*		へのポインタ。
	* @param	teamName
	*		チーム名。
	* @param	teamSide
	*		チームのサイド。
	*/
	void initialize(const char* body, const char* teamName, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "CoachSee.inl"
#endif

#endif // } !defined(COACHSEE_HXX__INCLUDED)
