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
* $B%W%l%$%d!<%/%i%$%"%s%H$N$?$a$K!"%5!<%P$H$N@\B3$r4IM}$7!"DL?.>u67$r(B          
* PlayerObserver $B$XDLCN$9$k!#(B
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
		/** $B%5!<%P$XAw?.$5$l$k%a%C%;!<%8$r(B buffer $B$X=q$-9~$`(B */
		virtual void format(char* buffer, int size) const = 0;
		/** $B%*%V%6!<%P$XDLCN$9$k!#(B*/
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
		int m_key;					// say $B0E9f2=$N%-!<(B
		char* m_teamName;			// $B%A!<%`L>(B
		Side m_teamSide;			// $B%A!<%`$N%5%$%I(B
		Connection m_connection;
		bool m_connected;			// $B@\B3:Q$_$+!)(B
		PlayMode m_playMode;		// $B%W%l%$%b!<%I(B
		int m_countReceived;		// $B<u?.$7$?%a%C%;!<%8$N?t(B
		int m_refereeTimer;			// $B8=:_$N?3H=$N;~7W(B
		int m_uniformNumber;		// $B%W%l%$%d!<$NGXHV9f(B
		typedef std::list<Command*> Commands;
		Commands m_commands;		// $B%P%C%U%!%j%s%0$7$F$$$kHs%3%s%7%e!<%^(B
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
		* $B%5!<%P$X%W%l%$%d!<$rEPO?$9$k!#:G=i$K%-!<%Q!<$H$7$FEPO?$r;n$_$F!"<:GT$7$?(B
		* $B$iIaDL$N%W%l%$%d!<$H$7$FEPO?$9$k!#(B
		* @return
		*		$BEPO?$G$-$?$i(B true$B!"$G$-$J$+$C$?$i(B false$B!#(B
		*/
		bool connect(const char* hostName, int port, const char* teamName);
		/**
		* $B%5!<%P$X%W%l%$%d!<$r:FEPO?$9$k!#(B
		* @return
		*		$BEPO?$G$-$?$i(B true$B!"$G$-$J$+$C$?$i(B false$B!#(B
		*/
		bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber);
		
		/**
		* $B%a%C%;!<%8$,%P%C%U%!$KB8:_$9$k$^$GBT$D!#;XDj;~4VFb$K%a%C%;!<%8$r<u?.(B
        * $B$7$?>l9g!"4X?t$O=*N;$9$k!#(B
		* @param	timeOut
		*				$B%?%$%`%"%&%H!J(Bmsec $BC10L!K!#(B-1 $B$GL58BBg!#(B
		* @return
		*				$B%?%$%`%"%&%H$N;~4V$,7P2a$9$kA0$K%a%C%;!<%8$r<u$1<h$C$?(B
		*				$B>l9g(B true$B!#$=$&$G$J$$>l9g(B false$B!#(B
		*/
		bool wait(long timeOut = -1);
		/**
		* $B%a%C%;!<%8$,%P%C%U%!$KB8:_$9$k$^$GBT$A!"%P%C%U%!$+$i%a%C%;!<%8$r#1$D(B
        * $B<h$j=P$7!"%*%V%6!<%P$XDLCN$9$k!#%a%C%;!<%8$N<oN`$rJV$9!#(B
		*/
		InformationType eat();
		
		/** $B<!$O%3%s%7%e!<%^4X?t(B */
		void dash(double power);
		void turn(double moment);
		void kick(double power, const Angle& direction);
		void catchBall(const Angle& direction);
		void move(const Vector& position);
		void rest();
		
		/** $B0J2<$N%3%^%s%I$O!"%3%s%7%e!<%^4X?t$r8F$S=P$7$?;~$KAw?.$5$l$k!#(B*/
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
