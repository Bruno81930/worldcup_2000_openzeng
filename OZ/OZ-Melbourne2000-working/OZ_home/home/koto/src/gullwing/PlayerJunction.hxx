// PlayerJunction.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERJUNCTION_HXX__INCLUDED) // {
#define PLAYERJUNCTION_HXX__INCLUDED

#include "basic.hxx"
#include "Connection.hxx"
#include "Mutex.hxx"
#include "PlayerObserver.hxx"
#include "PlayerSenseBody.hxx"
#include "PlayerSee.hxx"
#include "PlayerHear.hxx"
class SayBuffer;

/////////////////////////////////////////////////////////////////////////////
// PlayerJunction

/**
* プレイヤークライアントのために、サーバとの接続を管理し、通信状況を          
* PlayerObserver へ通知する。
*/
class PlayerJunction
{
private:
	PlayerJunction(const PlayerJunction&);
	PlayerJunction& operator= (const PlayerJunction&);
	bool operator== (const PlayerJunction&) const;
private:
	class Command {
	public:
		Command();
		/** サーバへ送信されるメッセージを buffer へ書き込む */
		virtual void format(char* buffer, int size) const = 0;
		/** オブザーバへ通知する。*/
		virtual void cast(PlayerObserver& o) const = 0;
	};
	class Consumer : public Command {
	};
	class Nonconsumer : public Command {
	};
	class Dash : public Consumer {
		double m_power;
	public:
		Dash(double power);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Turn : public Consumer {
		double m_moment;
	public:
		Turn(double moment);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Kick : public Consumer {
		double m_power;
		Angle m_direction;
	public:
		Kick(double power, const Angle& direction);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Catch : public Consumer {
		Angle m_direction;
	public:
		Catch(const Angle& direction);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Move : public Consumer {
		Vector m_position;
	public:
		Move(const Vector& position);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Rest : public Consumer {
	public:
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class TurnNeck : public Nonconsumer {
		Angle m_angle;
	public:
		TurnNeck(const Angle& angle);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class Say : public Nonconsumer {
		char* m_message;
	public:
		~Say();
		Say(const char* message);
		Say(const SayBuffer& buffer, int key, int time);
		Say(const Say&);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	class ChangeView : public Nonconsumer {
		ViewWidth m_width;
		ViewQuality m_quality;
	public:
		ChangeView(ViewWidth width, ViewQuality quality);
		virtual void format(char* buffer, int size) const;
		virtual void cast(PlayerObserver& o) const;
	};
	private:
		int m_key;					// say 暗号化のキー
		char* m_teamName;			// チーム名
		Side m_teamSide;			// チームのサイド
		Connection m_connection;
		bool m_connected;			// 接続済みか？
		PlayMode m_playMode;		// プレイモード
		int m_countReceived;		// 受信したメッセージの数
		int m_refereeTimer;			// 現在の審判の時計
		int m_uniformNumber;		// プレイヤーの背番号
		typedef std::list<Command*> Commands;
		Commands m_commands;		// バッファリングしている非コンシューマ
		Say* m_say;
		typedef std::list<PlayerObserver*> Observers;
		Observers m_observers;
		Mutex m_observersMutex;
#ifndef NDEBUG
		PlayerObserver m_dummyObserver;
#endif
		bool m_broadcasting;
		
		PlayerSenseBody m_senseBodyInformation;
		PlayerSee m_seeInformation;
		PlayerHear m_hearInformation;
		
		void initialize(int uniformNumber, const char* playModeString);
		InformationType onMessage(/*non-const*/ char* message);
		void onSenseBody(const PlayerSenseBody& sbi);
		void onSee(const PlayerSee& si);
		void onHear(const PlayerHear& hi);
		
		void broadcast(const Command& command);
		void send(const Consumer& command);
		void push(Nonconsumer* command);
		
	public:
		virtual ~PlayerJunction();
		PlayerJunction(int key);
		
		int countReceived() const;

		void addObserver(PlayerObserver* observer);
		void removeObserver(PlayerObserver* observer);
		
		/**
		* サーバへプレイヤーを登録する。最初にキーパーとして登録を試みて、失敗した
		* ら普通のプレイヤーとして登録する。
		* @return
		*		登録できたら true、できなかったら false。
		*/
		bool connect(const char* hostName, int port, const char* teamName);
		/**
		* サーバへプレイヤーを再登録する。
		* @return
		*		登録できたら true、できなかったら false。
		*/
		bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber);
		
		/**
		* メッセージがバッファに存在するまで待つ。指定時間内にメッセージを受信
        * した場合、関数は終了する。
		* @param	timeOut
		*				タイムアウト（msec 単位）。-1 で無限大。
		* @return
		*				タイムアウトの時間が経過する前にメッセージを受け取った
		*				場合 true。そうでない場合 false。
		*/
		bool wait(long timeOut = -1);
		/**
		* メッセージがバッファに存在するまで待ち、バッファからメッセージを１つ
        * 取り出し、オブザーバへ通知する。メッセージの種類を返す。
		*/
		InformationType eat();
		
		/** 次はコンシューマ関数 */
		void dash(double power);
		void turn(double moment);
		void kick(double power, const Angle& direction);
		void catchBall(const Angle& direction);
		void move(const Vector& position);
		void rest();
		
		/** 以下のコマンドは、コンシューマ関数を呼び出した時に送信される。*/
		void turnNeck(const Angle& angle);
		void say(const SayBuffer& buffer);
		void say(const char* message);
		void changeView(ViewWidth width, ViewQuality quality);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerJunction.inl"
#endif

#endif // } !defined(PLAYERJUNCTION_HXX__INCLUDED)
