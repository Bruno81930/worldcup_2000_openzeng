// Connection.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Connection

inline ConnectionException::~ConnectionException()
{
	delete[] m_what;
}
inline ConnectionException::ConnectionException(const ConnectionException& src)
{
	int length = strlen(src.what());
	m_what = new char[length+1];
	strcpy(m_what, src.what());
}
inline ConnectionException::ConnectionException(const char* what)
{
	ASSERT(what);
	int length = strlen(what);
	m_what = new char[length+1];
	strcpy(m_what, what);
}
inline const char* ConnectionException::what() const
{
	return m_what;
}


inline Connection::~Connection()
{
	if(m_socket != INVALID_SOCKET)
		CLOSESOCKET(m_socket);
}
inline Connection::Connection()
{
	m_socket = INVALID_SOCKET;
}

inline bool Connection::isInitialized() const
{
	return m_socket != INVALID_SOCKET;
}

inline void Connection::send(const char* message)
{
	ASSERT(message);
	ASSERT(isInitialized());
	
	int length = strlen(message);
	int sent = sendto(m_socket, message, length, 0, (const sockaddr*)&m_serverAddress, sizeof(m_serverAddress));
	if(sent != length) {
		report("error: can't sendto(\"%s\")\n", message);
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("sendto");
#endif
	}
}

inline bool Connection::wait(long timeOut)
{
	ASSERT(timeOut >= -1);
	ASSERT(isInitialized());
	if(timeOut == -1)
		timeOut = LONG_MAX;
	
	// メッセージの受信を timeOut だけ待つ。
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_socket, &fds);
	timeval timeout;
	//memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = timeOut / 1000;
	timeout.tv_usec = timeOut % 1000;
	int exist = select(m_socket+1, &fds, NULL, NULL, &timeout);
	ASSERT(exist != SOCKET_ERROR);
	if(!exist)
		return false;

	return true;
	
}
inline bool Connection::receive(char* buffer, int size, long timeOut)
{
	ASSERT(buffer && size >= 2 && timeOut >= -1);
	ASSERT(isInitialized());
	
	// メッセージの受信を timeOut だけ待つ。
	if(!wait(timeOut))
		return false;
	
	// 受信する。
	struct sockaddr_in sai;
	socklen_t sizeof_sai= sizeof(sai);
	int length = recvfrom(m_socket, buffer, size-1, 0, (sockaddr*)&sai, &sizeof_sai);
	if(length < 0) {
		perror("error: recvfrom");
#if NEVER_THROW
		exit(1);
#else
		throw ConnectionException("recvfrom");
#endif
	}
	
	m_serverAddress.sin_port = sai.sin_port;
	ASSERT(length < size);
	buffer[length] = '\0';
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
