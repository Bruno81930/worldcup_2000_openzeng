// Connection.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CONNECTION_HXX__INCLUDED) // {
#define CONNECTION_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Connection

/**
* $B@\B3$K4X$9$k%(%i!<$rI=$9Nc30!#(B
*/
class ConnectionException
{
private:
	ConnectionException& operator= (const ConnectionException&);
	bool operator== (const ConnectionException&) const;
private:
	char* m_what;
public:
	~ConnectionException();
	ConnectionException(const ConnectionException& src);
	explicit ConnectionException(const char* what);
	/**
	* $BNc30$rH/@8$5$;$?860x$rI=$9J8;zNs$rF@$k!#(B
	*/
	const char* what() const;
};

/**
* Soccer Server $B$H$NJ8;zNsDL?.$r9T$&!#(B
* $B<!$N$$$:$l$+$N>uBV$r<h$k!#(B
* $B!&L$=i4|2=(B
* $B!&=i4|2=:Q$_(B
* $B%*%V%8%'%/%H$,:n@.$5$l$?D>8e$OL$=i4|2=$N>uBV$G$"$k!#(B initialize $B$N8F$S=P$7$,(B
* $B@.8y$9$k$H=i4|2=:Q$_$N>uBV$X0\9T$9$k!#=i4|2=:Q$_$G$"$C$F$b!"(Binitialize$B$K$h$C(B
* $B$F:F=i4|2=$9$k$3$H$,$G$-$k!#(B
*/
class Connection
{
private:
	SOCKET m_socket;
	sockaddr_in m_serverAddress;
	
public:
	~Connection();
	Connection();
	Connection(const Connection&);
	
	/**
	* $B=i4|2=:Q$_$N>l9g(B true$B!"L$=i4|2=$N>l9g(B false $B$rJV$9!#(B
	*/
	bool isInitialized() const;
	
	/**
	* $B=i4|2=$9$k!#(B
	* @param	hostName
	*		$B:G=i$N%a%C%;!<%8$NAw?.@h$K$J$k%[%9%HL>!#(B"localhost" $B$d(B
	*		"127.0.0.1" $B$J$I!#(B
	* @param	port
	*		$B:G=i$N%a%C%;!<%8$NAw?.@h$K$J$k%]!<%HHV9f!#(B
	* @throws	ConnectionException
	*		$B2?$i$+$N%(%i!<$,H/@8$7$?!#(B
	*/
	void initialize(const char* hostName, int port);
	/**
	* $B%a%C%;!<%8$rAw?.$9$k!#=i4|2=:Q$_$G$"$kI,MW$,$"$k!#(B
	* $B=i4|2=D>8e$O!"(Binitialize $B$G;XDj$5$l$?%]!<%H$X%a%C%;!<%8$rAw?.$9$k!#(B     
    * receive$B$G%a%C%;!<%8$r<u?.$7$?8e$O!"<u?.$7$?%a%C%;!<%8$NAw?.85$N%]!<%H$X(B 
    * $BAw?.$9$k!#(B
	* @param	message
	*		$BAw?.$9$kJ8;zNs!#(B
	* @throws	ConnectionException
	*		$B2?$i$+$N%(%i!<$,H/@8$7$?!#(B
	*/
	void send(const char* message);
	/**
	* $B%a%C%;!<%8$,FO$/$^$GBT$D!#=i4|2=:Q$_$G$"$kI,MW$,$"$k!#%a%C%;!<%8$O%P%C%U(B
    * $B%!$K$?$a$i$l!"<!$N(B receive$B$G<u$1<h$k$3$H$,$G$-$k!#(B
	* @return
	*		timeOut $B$N;~4VBT$C$F$b%a%C%;!<%8$r<u?.$7$J$+$C$?>l9g(B false$B!#<u?.$7(B
	*		$B$?>l9g(B true$B!#(B
	* @param	timeout
	*		$B%a%C%;!<%8$N<u?.$rBT$D;~4V!J%_%jICC10L!K!#(B -1 $B$,EO$5$l$?>l9g!"L58B(B
	*		$BBg$H$7$F07$o$l$k!#(B
	* @throws	ConnectionException
	*		$B2?$i$+$N%(%i!<$,H/@8$7$?!#(B
	*/
	bool wait(long timeOut);
	/**
	* $B%a%C%;!<%8$r<u?.$9$k!#=i4|2=:Q$_$G$"$kI,MW$,$"$k!#(B
	* @return
	*		timeOut $B$N;~4VBT$C$F$b%a%C%;!<%8$r<u?.$7$J$+$C$?>l9g(B false$B!#<u?.$7(B
	*		$B$?>l9g(B true$B!#(B
	* @param	buffer
	*		$B%a%C%;!<%8$r<u?.$9$k%P%C%U%!$X$N%]%$%s%?!#(B
	* @param	size
	*		buffer $B$,<u$1<h$l$k%a%C%;!<%8$N%5%$%:!#(B
	* @param	timeout
	*		$B%a%C%;!<%8$N<u?.$rBT$D;~4V!J%_%jICC10L!K!#(B -1 $B$,EO$5$l$?>l9g!"L58B(B
	*		$BBg$H$7$F07$o$l$k!#(B
	* @throws	ConnectionException
	*		$B2?$i$+$N%(%i!<$,H/@8$7$?!#(B
	*/
	bool receive(char* buffer, int size, long timeOut);
};


/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Connection.inl"
#endif

#endif // } !defined(CONNECTION_HXX__INCLUDED)
