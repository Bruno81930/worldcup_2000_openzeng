// PlayerHear.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERHEAR_HXX__INCLUDED) // {
#define PLAYERHEAR_HXX__INCLUDED

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// PlayerHear

class PlayerHear
{
private:
	int m_refereeTimer;
	SenderType m_senderType;
	Angle m_direction;
	char* m_message;
	int m_refereeMessage;
	int m_extra;
public:
	/**
	* 審判の時計の指している時刻（Soccer Monitor に表示されているのと同じも   
    * の）を返す。
	*/
	int refereeTimer() const;
	/**
	* 送信者の種類を返す。次のいずれか。
	*		SENDER_REFEREE		レフリーからのメッセージ
	*		SENDER_SELF			自分自身からのメッセージ
	*		SENDER_DIRECTION	他のプレイヤーからのメッセージ
	*/
	SenderType senderType() const;
	/**
	* senderType() == SENDER_DIRECTION の場合、送信者の方向。
	*/
	Angle direction() const;
	/**
	* senderType() == SENDER_REFEREE の場合、審判のメッセージ。次のいずれか。
	*		PLAYMODE_*			プレイモード変更のメッセージ。ただし、プレイヤ
	*							ーのチームが右サイドの場合、左右の区別を持つプ
	*							レイモードは、左右を逆転したもの。
	*		REFREEMESSAGE_*		その他のメッセージ。ただし、プレイヤーのチーム
	*							が右サイドの場合、左右の区別を持つプレイモード
	*							は、左右を逆転したもの。
	*		REFREEMESSAGE_COACH	コーチからのメッセージ？
	*/
	int refereeMessage() const;
	/**
	* refereeMessage() == PLAYMODE_GOAL_L || refereeMessage() == PLAYMODE_GOAL
    * _R のとき、何点目が入ったかを返す。
	*/
	int score() const;
	/**
	* 受信したメッセージを返す。
	*/
	const char* message() const;
	
	~PlayerHear();
	PlayerHear();
	PlayerHear(const PlayerHear&);
	void initialize();
	/**
	* Soccer Server から得たメッセージから、オブジェクトを初期化する。
	* @param	body
	*		Soccer Server から得た hear メッセージの、time パラメタの先頭の文 
	*		字へのポインタ。
	* @param	teamSide
	*		プレイヤーのチームのサイド。
	*/
	void initialize(const char* body, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerHear.inl"
#endif

#endif // } !defined(PLAYERHEAR_HXX__INCLUDED)
