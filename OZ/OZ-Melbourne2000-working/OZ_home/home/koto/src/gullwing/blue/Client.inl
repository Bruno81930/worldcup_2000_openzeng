// Client.inl
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
// Client

inline ClientPremise::ClientPremise(Client* brain) : Premise(brain)
{
	m_brain = brain;
}

/*inline Client::Client(Client const& source)
{
	Client();
	operator= (source);
}
inline Client& Client::operator= (Client const& lhs)
{
	if(this == &lhs)
		return *this;
	return *this;
}
inline bool Client::operator== (Client const& lhs) const
{
	if(this == &lhs)
		return true;
}*/

/////////////////////////////////////////////////////////////////////////////
#undef inline
