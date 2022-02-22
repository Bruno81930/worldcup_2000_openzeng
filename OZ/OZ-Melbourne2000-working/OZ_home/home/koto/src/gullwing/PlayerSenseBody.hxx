// PlayerSenseBody.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERSENSEBODY_HXX__INCLUDED) // {
#define PLAYERSENSEBODY_HXX__INCLUDED

#include "basic.hxx"
#if LEARNING
#include "CoachSee.hxx"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerSenseBody

class PlayerSenseBody
{
#if LEARNING
public:
	bool m_inspiration;	// inspiration を得たか？
	CoachSee m_inspirationSee;
#endif
private:
	int m_refereeTimer;
	ViewQuality m_viewQuality;
	ViewWidth m_viewWidth;
	double m_stamina;
	double m_effort;
	double m_speed;
	Angle m_neckAngle;
	int m_kickCount;
	int m_dashCount;
	int m_turnCount;
	int m_sayCount;
	int m_turnNeckCount;
public:
	/**
	* 審判の時計の指している時刻（Soccer Monitor に表示されているのと同じも   
    * の）を返す。
	*/
	int refereeTimer() const;
	ViewQuality viewQuality() const;
	ViewWidth viewWidth() const;
	double stamina() const;
	double effort() const;
	double speed() const;
	/**
	* 戻り値は常に valid() である。
	*/
	Angle neckAngle() const;
	int kickCount() const;
	int dashCount() const;
	int turnCount() const;
	int sayCount() const;
	int turnNeckCount() const;
	
	PlayerSenseBody();
	void initialize();
	/**
	* Soccer Server から得たメッセージから、オブジェクトを初期化する。
	* @param	body
	*		Soccer Server から得た sense_body メッセージの、time パラメタの先 
	*		頭の文字へのポインタ。
	* @param	teamName
	*		チーム名。
	* @param	teamSide
	*		チームのサイド。
	*/
	void initialize(const char* body, const char* teamName, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerSenseBody.inl"
#endif

#endif // } !defined(PLAYERSENSEBODY_HXX__INCLUDED)
