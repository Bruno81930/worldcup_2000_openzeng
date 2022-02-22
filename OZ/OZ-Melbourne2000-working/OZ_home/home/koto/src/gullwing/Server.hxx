// Server.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SERVER_HXX__INCLUDED) // {
#define SERVER_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Server

/**
* Soccer Server $B$N;}$D%+%9%?%^%$%:2DG=$JCM!J%W%m%Q%F%#$H8F$V!K$rDs6!$9$k!#(B
*/
class Server
{
public:
	typedef const char* chars;
	static Server theServer;
	
private:
#	define P(type_, name_, value_)   type_ m_##name_;
#	define Q(type_, name_, value_)   type_ m_##name_##0;	type_ m_##name_;
#	include "parameters.hxx"
#	undef P
#	undef Q
	mutable int m_sayBits;
	
public:
#	define P(type_, name_, value_)   type_ name_() const { return m_##name_; }
#	define Q(type_, name_, value_)   type_ name_() const { return m_##name_; }
#	include "parameters.hxx"
#	undef P
#	undef Q
	int sayBits() const;
	double kickable_area() const;
	
	~Server();
	Server();
	Server(const Server&);

	void normalize();
	
	/**
	* $B@_Dj%U%!%$%k$+$i%W%m%Q%F%#$rFI$_9~$`!#(B
	* @param	filename
	*		$B@_Dj%U%!%$%k$N%U%!%$%kL>!#(B
	*/
	bool readConfigFile(const char* filename);
	
	/**
	* $B%W%m%Q%F%#$r99?7$9$k!#8=:_$N%P!<%8%g%s$G$O!"J8;zNs7?$N%W%m%Q%F%#$O99?7$G(B
    * $B$-$J$$!#(B
	* @return
	*		$B%(%i!<$,H/@8$7$?>l9g(B false$B!"@5>o$K99?7$5$l$?>l9g(B true$B!#(B
	* @param	name
	*		$B99?7$5$l$k%W%m%Q%F%#$NL>A0(B
	* @param	value
	*		$B99?7$5$l$k%W%m%Q%F%#$NCM$rI=$9J8;zNs$X$N%]%$%s%?!#(B
	*/
	bool setValue(const char* name, const char* value);
	
#ifdef NEVER_DEFINED
	// $B%(%G%#%?Jd405!G=MQ%@%_!<(B
	chars		SAY_MESSAGE_LETTERS();
	int			TIMEDELTA();
	double		PITCH_LENGTH();
	double		PITCH_WIDTH();
	double		PITCH_MARGIN();
	double		CENTER_CIRCLE_R();
	double		PENALTY_AREA_LENGTH();
	double		PENALTY_AREA_WIDTH();
	double		GOAL_AREA_LENGTH();
	double		GOAL_AREA_WIDTH();
	double		GOAL_DEPTH();
	double		PENALTY_SPOT_DIST();
	double		CORNER_ARC_R();
	double		KICK_OFF_CLEAR_DISTANCE();
	double		UNUM_FAR_LENGTH();
	double		UNUM_TOOFAR_LENGTH();
	double		TEAM_FAR_LENGTH();
	double		TEAM_TOOFAR_LENGTH();
	chars		LEFT_STR();
	chars		RIGHT_STR();
	double		goal_width();
	double		player_size();
	double		player_decay();
	double		player_rand();
	double		player_weight();
	double		player_speed_max();
	double		stamina_max();
	double		stamina_inc_max();
	double		recover_dec_thr();
	double		recover_min();
	double		recover_dec();
	double		effort_dec_thr();
	double		effort_min();
	double		effort_dec();
	double		effort_inc_thr();
	double		effort_inc();
	double		ball_size();
	double		ball_decay();
	double		ball_rand();
	double		ball_weight();
	double		ball_speed_max();
	double		dash_power_rate();
	double		kick_power_rate();
	double		kickable_margin();
	double		control_radius();
	double		catch_probability();
	double		catchable_area_l();
	double		catchable_area_w();
	double		maxpower();
	double		minpower();
	double		maxmoment();
	double		minmoment();
	double		maxneckmoment();
	double		minneckmoment();
	double		maxneckang();
	double		minneckang();
	double		visible_angle();
	double		visible_distance();
	double		audio_cut_dist();
	double		quantize_step();
	double		quantize_step_l();
	double		ckick_margin();
	double		wind_dir();
	double		wind_force();
	double		wind_rand();
	double		inertia_moment();
	bool		wind_none();
	bool		wind_random();
	int			half_time();
	int			port();
	int			coach_port();
	int			olcoach_port();
	int			say_coach_cnt_max();
	int			say_coach_msg_size();
	int			simulator_step();
	int			send_step();
	int			recv_step();
	int			sense_body_step();
	int			say_msg_size();
	int			hear_max();
	int			hear_inc();
	int			hear_decay();
	int			catch_ban_cycle();
	bool		coach();
	bool		coach_w_referee();
	bool		old_coach_hear();
	int			send_vi_step();
	bool		use_offside();
	double		offside_active_area_size();
	bool		forbid_kick_off_offside();
	chars		log_file();
	chars		record();
	int			record_version();
	bool		record_log();
	bool		send_log();
	bool		verbose();
	chars		replay();
	double		offside_kick_margin();
#endif
};

inline const Server& server() {
	return Server::theServer;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Server.inl"
#endif

#endif // } !defined(SERVER_HXX__INCLUDED)
