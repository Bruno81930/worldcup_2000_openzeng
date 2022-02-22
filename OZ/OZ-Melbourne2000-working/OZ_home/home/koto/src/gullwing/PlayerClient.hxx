// PlayerClient.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERCLIENT_HXX__INCLUDED) // {
#define PLAYERCLIENT_HXX__INCLUDED

class Window;
class Screen;

/////////////////////////////////////////////////////////////////////////////
// PlayerClient

/**
* プレイヤークライアント
*/
class PlayerClient
{
private:
	
protected:
	Window* m_window;
	
public:
	virtual ~PlayerClient();
	PlayerClient();
	
	/** クライアントの状態を表示するウィンドウを設定する。*/
	virtual void setWindow(Window* window);
	/** クライアントの状態を表示するウィンドウを解除する。*/
	virtual void removeWindow(Window* window);
	/** クライアントの状態を screen へ表示する。*/
	virtual void paintClient(Screen& screen) = 0;
	
	/**
	* サーバへ init コマンドで接続する。最初にゴールキーパーとして接続を試みて、
    * 失敗したら普通のプレイヤーとしての登録を試みる。
	*/
	virtual bool connect(const char* hostName, int port, const char* teamName) = 0;
	/**
	* サーバへ reconnect コマンドで接続する。
	*/
	virtual bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber) = 0;
	/**
	* クライアントを実行する。
	*/
	virtual void go() = 0;
};

/**
* デフォルトのチーム名を返す。
*/
const char* getDefaultTeamName();

/**
* 新しい PlayerClient を new して返す。
*/
PlayerClient* newPlayerClient(const std::map<std::string, std::string>& parameters);

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerClient.inl"
#endif

#endif // } !defined(PLAYERCLIENT_HXX__INCLUDED)
