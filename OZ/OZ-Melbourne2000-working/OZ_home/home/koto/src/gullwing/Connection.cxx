// Connection.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Connection.hxx"

#ifndef NDEBUG
#  include "Connection.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Connection

void Connection::initialize(const char* hostName, int port)
{
	ASSERT(hostName);
	
	if(m_socket != INVALID_SOCKET)
		CLOSESOCKET(m_socket);
	m_socket = INVALID_SOCKET;
	
	report("initializing connection. host='%s' port=%d\n", hostName, port);
	
#if 1
	// ホストを得る。
	unsigned long address = inet_addr(hostName);
	if(address == INADDR_NONE) {
		hostent* he = gethostbyname(hostName);
		if(he) {
			//address = htonl(*(unsigned long*)he->h_addr); ???
			in_addr* pia = (struct in_addr*)*(he->h_addr_list);
			const char* ip = inet_ntoa(*pia) ;
			if(ip)
				address = inet_addr(ip);
		}
	}
	if(address == INADDR_NONE) {
		perror("error: can't get address.");
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("get address");
#endif
	}
#else
	// ホストを得る。
	unsigned long address = 0;
	hostent* he = gethostbyname(hostName);
	if(he) {
		//address = htonl(*(unsigned long*)he->h_addr); ???
		in_addr* pia = (struct in_addr*)*(he->h_addr_list);
		const char* ip = inet_ntoa(*pia) ;
		if(ip) {
			address = inet_addr(ip);
			if(address == INADDR_NONE)
				address = 0;
		}
	} else {
		address = inet_addr(hostName);
		if(address == INADDR_NONE)
			address = 0;
	}
	if(address == 0) {
		perror("error: can't get address.");
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("get address");
#endif
	}
#endif
	
	// socket を作成する。
	SOCKET s = socket(PF_INET, SOCK_DGRAM, 0);
	if(s == INVALID_SOCKET) {
		perror("error: can't create a socket.");
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("socket");
#endif
	}
	
	// bind する。
	sockaddr_in sai;
	memset(&sai, 0, sizeof(sai));
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = htonl(INADDR_ANY);
	sai.sin_port = htons(0);
	if(bind(s, (const sockaddr*)&sai, sizeof(sai)) == SOCKET_ERROR) {
		perror("error: can't bind(...)");
		CLOSESOCKET(s);
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("bind");
#endif
	}
	
	// メンバ変数を設定する。
	m_socket = s;
	memset(&m_serverAddress, 0, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_addr.s_addr = address;
	m_serverAddress.sin_port = htons((unsigned short)port);
}
