// Monobrain.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MONOBRAIN_HXX__INCLUDED) // {
#define MONOBRAIN_HXX__INCLUDED

#include "Premise.hxx"
#include "Synchronizer.hxx"
#include "PlayerClient.hxx"
#include "Window.hxx"
#include "Screen.hxx"

/////////////////////////////////////////////////////////////////////////////
// Monobrain

/**
* $B!JC10l?M3J7?!K%V%l%$%s!#9=C[8e!"(Bconnect $B$"$k$$$O(B reconnect $B%a%s%P4X?t$G%5!<(B 
* $B%P$X@\B3$7!"(Bgo $B%a%s%P4X?t$G;OF0$9$k!#(Bact $B%a%s%P4X?t$K!"%W%l%$%d!<$rA`:n$9$k(B 
* $B%"%k%4%j%:%`$,5-=R$5$l$k!#8=:_$N%P!<%8%g%s$G$O!"%/%i%$%"%s%H$N;W9M;~4V$H!"%/(B
* $B%i%$%"%s%H%5!<%P4V$N%a%C%;!<%8$N1}I|$K$+$+$k;~4V$H$NOB$,!"#1%7%_%e%l!<%?%5%$(B
* $B%/%k$KBP$7$F==J,>.$5$$$3$H$r2>Dj$7$F$$$k!#(B
* 
* $B!!(Bgo $B%a%s%P4X?t$O!"%5!<%P$H$N@\B3$,@ZCG$5$l$J$$8B$j!"(Bact $B%a%s%P4X?t$r8F$S=P(B 
* $B$7$D$E$1$k!J(Bact $B%a%s%P4X?t$,Nc30$rJV$7$?>l9g$G$b!"%5!<%P$H$N@\B3$,@ZCG$5$l$?(B
* $B$3$H$rI=$9Nc30$G$J$$8B$j!"(Bact $B%a%s%P4X?t$r8F$S=P$7$D$E$1$k!K!#$?$@$7!"(Bact $B%a(B
* $B%s%P4X?t$,%3%s%7%e!<%^!J#1%7%_%e%l!<%7%g%s%5%$%/%k$K#12s$7$+<B9T$G$-$J$$%3%^(B
* $B%s%I$r%3%s%7%e!<%^$H8F$V$3$H$K$9$k!K$rH/9T$;$:$K=*N;$7$?>l9g!"%S%8!<%k!<%W$r(B
* $BKI;_$9$k$?$a$K!"(Brest $B%a%s%P4X?t$r#12s8F$S=P$9!#(B
* 
* $B!!(Bact $B%a%s%P4X?tCf$G$O!"(Bworld $B%a%s%P4X?t$K$h$C$F!V%o!<%k%I%b%G%k!W$X%"%/%;%9(B
* $B$9$k$3$H$,$G$-$k!#!V%o!<%k%I%b%G%k!W$O(B act $B%a%s%P4X?t$,8=:_A*Br$7$h$&$H$7$F(B 
* $B$$$k%3%^%s%I$,<B9T$5$l$kD>A0$N@$3&$N>uBV$rI=$9!J>-Mh$N%P!<%8%g%s$G$O!"%5!<%P(B
* $B$H$NDL?.$K;~4V$,$+$+$k>l9g!"%3%^%s%I$,<B:]$K<u?.$5$l$k;~9o$ND>A0$N@$3&$rM=B,(B
* $B$7$?%o!<%k%I%b%G%k$,Ds6!$5$l$k$+$b$7$l$J$$!K!#(B
* 
* $B!!(Bact $B%a%s%P4X?t$G$O!"%3%^%s%I$KBP1~$9$k%a%s%P4X?t$rMQ$$$F!"%3%^%s%I$r%5!<%P(B
* $B$XAw?.$9$k!#%3%^%s%I!?%3%s%7%e!<%^!?Hs%3%s%7%e!<%^$KBP1~$9$k%a%s%P4X?t$r$=$l(B
* $B$>$l!"%3%^%s%I4X?t!?%3%s%7%e!<%^4X?t!?Hs%3%s%7%e!<%^4X?t$H8F$V$3$H$K$9$k!#%3(B
* $B%^%s%I4X?t$O%5!<%P$HF14|$r$H$C$FF0:n$9$k!#%3%^%s%I4X?t$N0z$-?t$O!"%5!<%P$N%^(B
* $B%K%e%"%k$GDj$a$i$l$kHO0OFb$KL5$1$l$P$J$i$J$$!JHO0OFb$K$J$$>l9g(B assert $B$GCF$+(B
* $B$l$k!K!#(B
* $B!!%3%s%7%e!<%^4X?t$,=*N;$7$?;~E@$G!"<!$N%7%_%e%l!<%7%g%s%5%$%/%k$K$*$1$k9TF0(B
* $B$r;W9M$9$kMQ0U$,@0$C$F$$$k$3$H$,J]>Z$5$l$k!#$9$J$o$A!"%o!<%k%I%b%G%k$,99?7$5(B
* $B$l!JI,MW$J>pJs$,F@$i$l$k$^$G!"4X?t$O=*N;$7$J$$!K!"<!$K8F$S=P$9%3%s%7%e!<%^4X(B
* $B?t$K$h$k%3%^%s%I$O!"<!$N%7%_%e%l!<%7%g%s%5%$%/%k$G<B9T$5$l$k!#(B
* $B!!Hs%3%s%7%e!<%^4X?t$r8F$S=P$7$?>l9g!"<!$N%3%s%7%e!<%^4X?t$N8F$S=P$7$K$h$C$F(B
* $BAw?.$5$l$k%3%s%7%e!<%^$,!"%5!<%P$K$h$C$F<u?.$5$l$k$N$HF1$8%7%_%e%l!<%7%g%s%5(B
* $B%$%/%k$K$*$$$F!"$=$NHs%3%s%7%e!<%^$G$"$k%3%^%s%I$r%5!<%P$,<u?.$9$k$h$&$KAw?.(B
* $B$5$l$k!J$7$?$,$C$F!"Hs%3%s%7%e!<%^!&%3%s%7%e!<%^$NAw?.=g=x$OJ]>Z$5$l$J$$!K!#(B
* $B8=:_$N%P!<%8%g%s$G$O!"Hs%3%s%7%e!<%^4X?t$,8F$S=P$5$l$F$+$i!"<!$N%3%s%7%e!<%^(B
* $B4X?t$,8F$S=P$5$l$k$^$G!"%o!<%k%I%b%G%k$OITDj$H$9$k!#!JHs%3%s%7%e!<%^$rH?1G$9(B
* $B$Y$-$+$I$&$+7kO@$,=P$F$$$J$$$?$a!K!#(B
* $B!!$3$N%7%9%F%`$G$O!V2?$b$7$J$$!W$3$H$rL@<($9$k$?$a$K!"%3%s%7%e!<%^(B rest $B$,2>(B 
* $BA[E*$KMQ0U$5$l$F$$$k!J<B:]$K$O%5!<%P$X%3%^%s%I$rAw?.$7$J$$$,!"$3$N%7%9%F%`$N(B
* $B>e$G$O(B rest $B%3%^%s%I$,B8:_$9$k!K!#(B
* 
* $B!!A0Ds$r@_Dj$9$k$3$H$,$G$-$k!#%3%^%s%I4X?t$G$O!"<B9T$NA08e$GA0Ds$,%A%'%C%/$5(B
* $B$l!"Jx$l$?$b$N$,$"$k>l9g!"(BPremiseException $B$,(B throw $B$5$l$k!#(B
* 
* $B!!(BaddObserver $B$K$h$C$F!"%5!<%P$H$NAw<u?.$r4F;k$9$k%*%V%8%'%/%H$rMQ0U$9$k$3$H(B
* $B$,$G$-$k!J$,!"$H$j$"$($::#$N$H$3$m!"DLCN$N%?%$%_%s%0$d=gHV$rJ]>Z$7$J$$;EMM$K(B
* $B$7$F$$$k$N$G!"%P!<%8%g%s$K0MB8$7$J$$%W%m%0%i%`$O$[$H$s$I=q$1$J$$!K!#(B
* 
* $B$o$+$j$K$/$$@bL@$G$9$$$^$;$s!#(B
* 
* @param	ColorWorld
*				$B%o!<%k%I%b%G%k%/%i%9!#(B
*/
class White;
template<class ColorWorld = White>
class Monobrain : public PremiseOwner, public PlayerClient
{
private:
	Monobrain(const Monobrain&);
	Monobrain& operator= (const Monobrain&);
	bool operator== (const Monobrain&) const;

public:
	typedef ColorWorld World;
	typedef typename ColorWorld::Ball Ball;
	typedef typename ColorWorld::Player Player;
	typedef typename ColorWorld::Players Players;
	typedef typename ColorWorld::Self Self;
	
private:
	Synchronizer m_synchronizer;
	class Observer : public PlayerObserver
	{
		const Monobrain* m_brain;
	public:
		Observer(const Monobrain* brain) {
			m_brain = brain;
		}
		virtual void intoWaiting() {
			if(m_brain->m_window) {
				Vector toBall = m_brain->ball().position() - m_brain->self().position();
				m_brain->m_window->update(m_brain, toBall.norm());
			}
		}
	};
	friend class Observer;
	Observer m_observer;
	
protected:
	World m_world;

	/**
	* Soccer Server $B$X(B init $B%3%^%s%I$K$h$C$F@\B3$9$k!#:G=i$K%-!<%Q!<$H$7$F$NEP(B
    * $BO?$r;n$_!"<:GT$7$?$iDL>o$N%W%l%$%d!<$H$7$FEPO?$9$k!#(B
	* @param	hostName
	*				$B%5!<%P$N%[%9%HL>!#(B
	* @param	port
	*				$B%5!<%P$N%]!<%HHV9f!#(B
	* @param	teamName
	*				$B%A!<%`$NL>A0!#(B
	* @return	$B@.8y$7$?$i(B true$B!"<:GT$7$?$i(B false$B!#(B
	*/
	bool connect(const char* hostName, int port, const char* teamName) {
		return m_synchronizer.connect(hostName, port, teamName);
	}
	/**
	* Soccer Server $B$X(B reconnect $B%3%^%s%I$K$h$C$F@\B3$9$k!#:G=i$K%-!<%Q!<$H$7$F$NEP(B
    * $BO?$r;n$_!"<:GT$7$?$iDL>o$N%W%l%$%d!<$H$7$FEPO?$9$k!#(B
	* @param	hostName
	*				$B%5!<%P$N%[%9%HL>!#(B
	* @param	port
	*				$B%5!<%P$N%]!<%HHV9f!#(B
	* @param	teamName
	*				$B%A!<%`$NL>A0!#(B
	* @param	uniformNumber
	*				$B%3%s%H%m!<%k$9$k%W%l%$%d!<$NGXHV9f!J(B0 $B0J>e(B 10 $BL$K~$NCM!K!#(B
	* @return	$B@.8y$7$?$i(B true$B!"<:GT$7$?$i(B false$B!#(B
	*/
	bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber) {
		return m_synchronizer.reconnect(hostName, port, teamName, uniformNumber);
	}

	/** $B%V%l%$%s$r<B9T$9$k!#%5!<%P$H$N@\B3$,@Z$l$k$^$G=*N;$7$J$$!#(B*/
	virtual void go();
	/**
	* $BGI@8%/%i%9$GDj5A$5$l$k%V%l%$%s$N;W9M%k!<%A%s!#$3$N%k!<%A%s$NCf$G$O!"(B
	* $B!&(BaddObserver $B$H(B removeObserver
	* $B!&(BaddPremise$B!"(BremovePremise $B$H!"(BcheckPremises
	* $B!&(Bdash $B$d(B turn $B$J$I$N%5!<%P$X%3%^%s%I$rAw?.$9$k%a%s%P4X?t(B
	* $B!&(Bworld $B$d(B self $B$J$I$N(B public $B%a%s%P4X?t(B
	* $B$r8F$S=P$9$3$H$,$G$-$k!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	virtual void act() = 0;

	/** $BDL?.$r4F;k$9$k%*%V%6!<%P!<$rDI2C$9$k!#(B*/
	void addObserver(PlayerObserver* observer) {
		m_synchronizer.addObserver(observer);
	}
	/** $B%*%V%6!<%P!<$r:o=|$9$k!#(B*/
	void removeObserver(PlayerObserver* observer) {
		m_synchronizer.removeObserver(observer);
	}

	/* $B%3%s%7%e!<%^4X?t$,=*N;$7!"@)8f$,8F$S$@$7B&$KLa$kD>A0$K8F$S=P$5$l$k!#(B*/
	virtual void steped() { }

	/**
	* dash $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	power
	*				$B%Q%o!<(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void dash(double power) {
		checkPremises();
		if(power == 0)
			return;
		m_synchronizer.dash(power);
		checkPremises();
		steped();
	}
	/**
	* dash $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	length
	*				$B%@%C%7%e$9$k5wN%(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void dashTo(double length) {
		checkPremises();
		if(length == 0)
			return;
		length = length / (1.0 + server().player_decay());	// $B#2%5%$%/%k$GE~Ce(B
		double power = length / self().effort() / server().dash_power_rate();
		power = suppress(0.0, power, server().maxpower());
		m_synchronizer.dash(power);
		checkPremises();
		steped();
	}
	/**
	* turn $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	angle
	*				$B<+J,$NBN$N8~$-$rCf?4$H$7$?!"2sE>8e$NJ}8~$rI=$9AjBP3QEY!#$?(B
	*				$B$@$7!"#1%5%$%/%k$G2sE>$G$-$k3QEY$h$jBg$-$$>l9g!"2DG=$J8B$j(B
	*				$B$N2sE>$r9T$&!#(B
	* @param	block
	*				$B2sE>$,H?1G$5$l$?(B see $B>pJs$r<u?.$9$k$^$G!"4X?t$+$iLa$i$J$$!)(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void turn(const Angle& angle, bool block = false) {
		checkPremises();
		if(angle == Angle(0))
			return;
		double degrees = angle.degrees();
		double moment = degrees * (1.0 + server().inertia_moment() * self().safeVelocity().norm());
		moment = suppress(server().minmoment(), moment, server().maxmoment());
		m_synchronizer.turn(moment);
		checkPremises();
		int last = world().time();
		if(block) {
			while(self().time() < last) {
				m_synchronizer.rest();
				checkPremises();
			}
		}
		steped();
	}
	/**
	* turn $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	direction
	*				$B2sE>8e$NJ}8~$rI=$9@dBPJ}8~!#$=$A$i$NJ}8~$r8~$/$^$G!"=*N;$7(B
	*				$B$J$$$N$G!"$3$N4X?t$N8F=P$7$G!"#1%5%$%/%k0J>e7P2a$9$k>l9g$,(B
	*				$B$"$k!#(B
	* @param	block
	*				$B2sE>$,H?1G$5$l$?(B see $B>pJs$r<u?.$9$k$^$G!"4X?t$+$iLa$i$J$$!)(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void turnTo(Angle direction, bool block = false) {
		checkPremises();
		int last = world().time();
		for(;;) {
			double degrees = (direction - self().bodyDirection()).degrees();
			if(degrees == 0)
				break;
			double moment = degrees * (1.0 + server().inertia_moment() * self().safeVelocity().norm());
			double rounded = suppress(server().minmoment(), moment, server().maxmoment());
			m_synchronizer.turn(rounded);
			last = world().time();
			if(rounded == moment)
				break;
		}
		checkPremises();
		if(block) {
			while(self().time() < last) {
				m_synchronizer.rest();
				checkPremises();
			}
		}
		steped();
	}
	/**
	* kick $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	power
	*				$B%Q%o!<(B
	* @param	direction
	*				$BJ}8~(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void kick(double power, const Angle& direction) {
		checkPremises();
		m_synchronizer.kick(power, direction);
		checkPremises();
		steped();
	}
	/**
	* kick $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	acceleration
	*				$B%\!<%k$XM?$($k@dBP2CB.EY(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void kick(Vector acceleration) {
		checkPremises();
		double norm = acceleration.norm();
		if(norm == 0)
			return;
		Angle direction = acceleration.angle() - self().bodyDirection();
		Vector toBall = ball().position() - self().position();
		double distanceToBall = toBall.norm();
		Angle directionToBall = ((distanceToBall == 0) ? Angle(0) : toBall.angle());
		double power = norm / server().kick_power_rate();
		power /= 1.0 - 0.25 * abs(directionToBall - self().bodyDirection()).degrees()/180
			- 0.25 * (distanceToBall - server().player_size() - server().ball_size()) / server().kickable_margin();
		power = suppress(0.0, power, server().maxpower());
		m_synchronizer.kick(power, direction);
		checkPremises();
		steped();
	}
	/**
	* catch $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	direction
	*				$BJ}8~(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void catchBall(const Angle& direction) {
		checkPremises();
		m_synchronizer.catchBall(direction);
		checkPremises();
		//SkickTsteped();
	}
	/**
	* move $B$9$k!J%3%s%7%e!<%^!K!#(B
	* @param	position
	*				$B0\F0@h(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void move(const Vector& position) {
		checkPremises();
		m_synchronizer.move(position);
		checkPremises();
		steped();
	}
	/**
	* $B2?$b$7$J$$!J%3%s%7%e!<%^!K!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void rest() {
		checkPremises();
		m_synchronizer.rest();
		checkPremises();
		steped();
	}
	
	/**
	* $B<s$r2s$9!JHs%3%s%7%e!<%^!K!##1%7%_%e%l!<%7%g%s%5%$%/%k$K!"#12s$7$+<s$r2s(B
    * $B$9$3$H$O$G$-$J$$!#(B
	* @param	angle
	*				$B<s$r2s$93QEY!J8=:_$N3QEY$KBP$9$kAjBP3QEY!K!#2s$7$?7k2L!"<s(B
	*				$B$NBN$KBP$9$k3QEY$,(B [server().minneckang(), server().      
	*				maxneckang()] $B$NHO0O30$K$J$C$F$O$J$i$J$$!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void turnNeck(const Angle& angle) {
		checkPremises();
		m_synchronizer.turnNeck(angle);
	}
	/**
	* $B<s$r2s$9!JHs%3%s%7%e!<%^!K!##1%7%_%e%l!<%7%g%s%5%$%/%k$K!"#12s$7$+<s$r2s(B
    * $B$9$3$H$O$G$-$J$$!#(B
	* @param	angle
	*				$B2s$7$?8e$K$=$&$J$k$Y$-!"<s$NBN$KBP$9$kAjBP3QEY!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	void turnNeckTo(Angle angle) {
		checkPremises();
		angle = Angle(suppress(server().minneckang()+0.1, angle.degrees(), server().maxneckang()-0.1));
		ASSERT(server().minneckang() <= angle.degrees() && angle.degrees() <= server().maxneckang());
		Angle relative = angle - self().neckAngle();
		relative = Angle(suppress(server().minneckmoment()+0.1, relative.degrees(), server().maxneckmoment()-0.1));
		ASSERT(server().minneckmoment() <= relative.degrees() && relative.degrees() <= server().maxneckmoment());
		m_synchronizer.turnNeck(relative);
	}
	
	/**
	* say $B$9$k!JHs%3%s%7%e!<%^!K!#(B
	* @param	i
	*				$B@0?t7?$NJQ?t$G!"6+$P$l$k%a%C%;!<%8!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	template<class T> void say(T i)
	{
		m_synchronizer.say(i);
	}
	/**
	* say $B$9$k!JHs%3%s%7%e!<%^!K!#(B
	* @param	i
	*				$B@0?t7?$NJQ?t$G!"6+$P$l$k%a%C%;!<%8!#(B
	* @param	bits
	*				i $B$N2<0L(B bits $B%S%C%H$r6+$V!#(B
	* @throws	PremiseException
	*				$BA0Ds$,Jx$l$?!#(B
	*/
	template<class T> void say(T i, int bits)
	{
		m_synchronizer.say(i, bits);
	}
	
	/**
	* $B%S%e!<%b!<%I$rJQ99$9$k!JHs%3%s%7%e!<%^!K!#(B
	*/
	void changeView(ViewWidth width, ViewQuality quality) {
		if(self().viewWidth() != width || self().viewQuality() != quality)
			m_synchronizer.changeView(width, quality);
	}
	
public:
	virtual ~Monobrain() {
	}
	Monobrain(int key) : m_synchronizer(key), m_observer(this) {
		m_synchronizer.addObserver(&m_world.asObserver());
		m_synchronizer.addObserver(&m_observer);
	}

	virtual void paintClient(Screen& screen);
	
	/**
	* $B<u?.$7$?%a%C%;!<%8$N?t!#(B
	*/
	int countReceived() const {
		return m_synchronizer.countReceived();
	}
	/**
	* $B%o!<%k%I%b%G%k$rJV$9!#%G%U%)%k%H$G$O(B White $B%*%V%8%'%/%H$rJV$9!#(B
	*/
	const World& world() const {
		return m_world;
	}
	/**
	* world().ball() $B$rJV$9!#%G%U%)%k%H$G$O(B WhiteBall $B%*%V%8%'%/%H$rJV$9!#(B
	*/
	const Ball& ball() const {
		return world().ball();
	}
	/**
	* world().others() $B$rJV$9!#%G%U%)%k%H$G$O(B WhitePlayer $B%*%V%8%'%/%H$rJV$9!#(B
	*/
	const Players& others() const {
		return world().others();
	}
	/**
	* world().self() $B$rJV$9!#%G%U%)%k%H$G$O(B WhiteSelf $B%*%V%8%'%/%H$rJV$9!#(B
	*/
	const Self& self() const {
		return world().self();
	}
};

template<class World>
void Monobrain<World>::go()
{
	int count = countReceived();
	for(;;) {
#if NEVER_THROW
#  error
#else
		try {
			if(count == countReceived())
				rest();	// $B%S%8!<%k!<%WKI;_(B
			count = countReceived();
			act();
		} catch(PremiseException) {
		} catch(ConnectionException& e) {
			if(strcmp(e.what(), "time-out") == 0)
				return;
			report("error: ConnectionException '%s'\n", (const char*)e.what());
		} catch(std::exception& e) {
			report("error: exception '%s'\n", (const char*)e.what());
#ifdef NDEBUG
		} catch(...) {
			report("error: Unknown\n");
#endif
		}
#endif
	}
}

template<class World>
void Monobrain<World>::paintClient(Screen& screen)
{
	screen.paintField();
	m_world.lock();
	{
		Screen& s = screen;

		// $B>pJs$rI=<((B
		char buffer[1024];
		int len = sprintf(buffer,
			"#%d stamina:%f effort:%f  [%s]  neckAngle:%f  time:%d (world/self/ball:/%d/%d/%d)",
			self().uniformNumber(), self().stamina(), self().effort(),
			playModeHelper.toString(world().playMode()), (double)self().neckAngle().degrees(),
			(int)world().refereeTimer(), (int)world().time(), (int)self().time(), (int)ball().time());
		ASSERT(len < 1023);
		if(ball().velocity().valid()) {
			len = sprintf(buffer+len,
				"  ball_speed:%f",
				ball().velocity().norm());
			ASSERT(len < 1023);
		}
		s.write(buffer);
		
		// $B%S%e!<%3!<%s$J$I$rI=<((B
		s.color(s.colorGreen());
		s.circle(self().position().x(), self().position().y(), server().player_size());
		s.circle(self().position().x(), self().position().y(), server().player_size() + server().kickable_margin());
		double x = self().position().x();
		double y = self().position().y();
		s.line(x, y, x + server().CENTER_CIRCLE_R() * cos(self().bodyDirection().radians()),
			y + server().CENTER_CIRCLE_R() * sin(self().bodyDirection().radians()));
		double widthRadians = self().viewWidthDegrees() * (PI/180.0);
		double startRadians = self().sightDirection().radians() - widthRadians/2;
		double endRadians = self().sightDirection().radians() + widthRadians/2;
		s.arc(x, y, server().visible_distance(), endRadians, startRadians);
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(startRadians), y + 2*server().PITCH_LENGTH() * sin(startRadians));
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(endRadians), y + 2*server().PITCH_LENGTH() * sin(endRadians));
		s.arc(x, y, server().UNUM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().UNUM_TOOFAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_TOOFAR_LENGTH(), startRadians, endRadians);

		// $B%U%i%0$rI=<((B
		{
			s.color(s.colorFlag());
			typename World::Vectors::const_iterator it = world().flags().begin();
			for(; it != world().flags().end(); it++) {
				const double d = 0.8;
				const Vector& v = *it;
				s.circle(v.x(), v.y(), d);
			}
		}
		
		// $B%W%l%$%d!<$rI=<((B
		{
			typename Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				int color = s.colorYellow();
				if(p.team().valid()) {
					switch(p.team()) {
					case SIDE_LEFT:
						color = s.colorGreen();
						break;
					case SIDE_RIGHT:
						color = s.colorRed();
						break;
					default:
						ASSERT(false);
					}
				}
				s.color(color);
				if(p.uniformNumber().valid()) {
					s.circle(p.position().x(), p.position().y(), server().player_size());
				} else {
					s.circleFill(p.position().x(), p.position().y(), server().player_size());
				}
			}
		}

		// $B%\!<%k$rI=<((B
		{
			const double d = 0.5;
			if(ball().time() < world().time())
				s.color(s.colorBallAnticipated());
			else
				s.color(s.colorBall());
			const Vector& v = ball().position();
			s.circle(v.x(), v.y(), server().ball_size());
			s.line(v.x() - d, v.y(), v.x() + d, v.y());
			s.line(v.x(), v.y() - d, v.x(), v.y() + d);
		}
	}
	m_world.unlock();
}

/////////////////////////////////////////////////////////////////////////////

#endif // } !defined(MONOBRAIN_HXX__INCLUDED)
