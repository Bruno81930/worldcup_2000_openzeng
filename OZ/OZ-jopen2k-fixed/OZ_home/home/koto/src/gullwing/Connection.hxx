// Connection.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CONNECTION_HXX__INCLUDED) // {
#define CONNECTION_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Connection

/**
* 接続に関するエラーを表す例外。
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
	* 例外を発生させた原因を表す文字列を得る。
	*/
	const char* what() const;
};

/**
* Soccer Server との文字列通信を行う。
* 次のいずれかの状態を取る。
* ・未初期化
* ・初期化済み
* オブジェクトが作成された直後は未初期化の状態である。 initialize の呼び出しが
* 成功すると初期化済みの状態へ移行する。初期化済みであっても、initializeによっ
* て再初期化することができる。
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
	* 初期化済みの場合 true、未初期化の場合 false を返す。
	*/
	bool isInitialized() const;
	
	/**
	* 初期化する。
	* @param	hostName
	*		最初のメッセージの送信先になるホスト名。"localhost" や
	*		"127.0.0.1" など。
	* @param	port
	*		最初のメッセージの送信先になるポート番号。
	* @throws	ConnectionException
	*		何らかのエラーが発生した。
	*/
	void initialize(const char* hostName, int port);
	/**
	* メッセージを送信する。初期化済みである必要がある。
	* 初期化直後は、initialize で指定されたポートへメッセージを送信する。     
    * receiveでメッセージを受信した後は、受信したメッセージの送信元のポートへ 
    * 送信する。
	* @param	message
	*		送信する文字列。
	* @throws	ConnectionException
	*		何らかのエラーが発生した。
	*/
	void send(const char* message);
	/**
	* メッセージが届くまで待つ。初期化済みである必要がある。メッセージはバッフ
    * ァにためられ、次の receiveで受け取ることができる。
	* @return
	*		timeOut の時間待ってもメッセージを受信しなかった場合 false。受信し
	*		た場合 true。
	* @param	timeout
	*		メッセージの受信を待つ時間（ミリ秒単位）。 -1 が渡された場合、無限
	*		大として扱われる。
	* @throws	ConnectionException
	*		何らかのエラーが発生した。
	*/
	bool wait(long timeOut);
	/**
	* メッセージを受信する。初期化済みである必要がある。
	* @return
	*		timeOut の時間待ってもメッセージを受信しなかった場合 false。受信し
	*		た場合 true。
	* @param	buffer
	*		メッセージを受信するバッファへのポインタ。
	* @param	size
	*		buffer が受け取れるメッセージのサイズ。
	* @param	timeout
	*		メッセージの受信を待つ時間（ミリ秒単位）。 -1 が渡された場合、無限
	*		大として扱われる。
	* @throws	ConnectionException
	*		何らかのエラーが発生した。
	*/
	bool receive(char* buffer, int size, long timeOut);
};


/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Connection.inl"
#endif

#endif // } !defined(CONNECTION_HXX__INCLUDED)
