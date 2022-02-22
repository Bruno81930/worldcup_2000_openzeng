// PlayerSee.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERSEE_HXX__INCLUDED) // {
#define PLAYERSEE_HXX__INCLUDED

#include "basic.hxx"
#if LEARNING
#include "CoachSee.hxx"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerSee

class PlayerSeeObject
{
#if LEARNING
public:
	// inspiration で知った正しいチームと背番号
	Side m_inspirationTeam;
	int m_inspirationUniformNumber;
#endif
private:
	Integer m_type;
	Side m_team;
	Integer m_id;
	Double m_distance;
	Angle m_direction;
	Double m_distanceChange;
	Double m_directionChange;
	Angle m_bodyDirection;
	Angle m_faceDirection;
public:
	/**
	* オブジェクトのタイプを返す。次のいずれか。
	*		TYPE_BALL				ボール
	*		TYPE_PLAYER				プレイヤー
	*		TYPE_FLAG				フラグかゴール
	*		TYPE_LINE				ライン
	*		上の値に-1を掛けた値	上のショートディスタンスセンサーで得られた物
	*/
	Integer type() const;
	/**
	* type() == TYPE_PLAYER の場合、プレイヤーが味方ならば SIDE_LEFT、敵ならば
    * SIDE_RIGHT。
	*/
	Side team() const;
	/**
	* type() == TYPE_PLAYER の場合、プレイヤーの背番号（０〜１０）。
	*/
	Integer uniformNumber() const;
	/**
	* type() == TYPE_FLAG の場合、フラグの種類。ただし、プレイヤーのチームが右
	* サイドの場合、ピッチの中心に関して点対称な位置にあるフラグの種類。
	*/
	Flag flagId() const;
	/**
	* type() == TYPE_LINE の場合、ラインの種類。ただし、プレイヤーのチームが右
    * サイドの場合、ピッチの中心に関して点対称な位置にあるラインの種類。
	*/
	Edge edge() const;
	Double distance() const;
	Angle direction() const;
	Double distanceChange() const;
	Double directionChange() const;
	Angle bodyDirection() const;
	Angle faceDirection() const;
	
	/**
	* Soccer Server から得たメッセージから、オブジェクトを初期化する。
	* @param	cursor
	*		[in]	see メッセージ中の、PlayerSeeObject 全体をくくる（開き）括弧への
	*				ポインタ。
	*		[out]	対応する閉じ括弧の右のスペースの右の文字へのポインタ。
	* @param	teamName
	*		チーム名。
	* @param	teamSide
	*		チームのサイド。
	*/
	void initialize(const char* &cursor, const char* teamName, Side teamSide);
};

class PlayerSee
{
#if LEARNING
public:
	bool m_inspiration;	// inspiration を得たか？
	CoachSee m_inspirationSee;
#endif
private:
	int m_refereeTimer;
	PlayerSeeObject* m_pBall;
	PlayerSeeObject* m_players[MAX_PLAYER*2 + 1];
	PlayerSeeObject* m_flags[FLAG_MAZ + 1];
	PlayerSeeObject* m_lines[4 + 1];
	PlayerSeeObject m_seeObjects[1 + MAX_PLAYER*2 + FLAG_MAZ + 4];
public:
	/**
	* 審判の時計の指している時刻（Soccer Monitor に表示されているのと同じも   
    * の）を返す。
	*/
	int refereeTimer() const;
	/**
	* 視覚情報にボールが含まれている場合、その情報へのポインタ。含まれていない
    * 場合 0。
	*/
	const PlayerSeeObject* ball() const;
	/**
	* 視覚情報に含まれるプレイヤーのリストへのポインタ。リストの末尾は 0 であ 
    * る。
	*/
	const PlayerSeeObject* const* players() const;
	/**
	* 視覚情報に含まれるフラグとゴールのリストへのポインタ。リストの末尾は 0  
    * である。
	*/
	const PlayerSeeObject* const* flags() const;
	/**
	* 視覚情報に含まれるラインのリストへのポインタ。リストの末尾は 0 である。
	*/
	const PlayerSeeObject* const* lines() const;
	
	PlayerSee();
	PlayerSee(const PlayerSee&);
	void initialize();
	
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
#  include "PlayerSee.inl"
#endif

#endif // } !defined(PLAYERSEE_HXX__INCLUDED)
