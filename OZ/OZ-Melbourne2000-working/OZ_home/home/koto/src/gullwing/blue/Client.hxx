// Client.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CLIENT_HXX__INCLUDED) // {
#define CLIENT_HXX__INCLUDED

#include "../Monobrain.hxx"
#include "Blue.hxx"

/////////////////////////////////////////////////////////////////////////////
// ClientPremise

class Client;
class ClientPremise : public Premise
{
protected:
	Client* m_brain;
public:
	ClientPremise(Client* brain);
};

/////////////////////////////////////////////////////////////////////////////
// Client

class Client : public Monobrain<Blue>
{
private:
	Client(const Client&);
	Client& operator= (const Client&);
	bool operator== (const Client&) const;

protected:
	virtual void go();
	virtual void act();
	
	void movable();
	void normal();

public:
	virtual ~Client();
	Client(const std::map<std::string, std::string>& parameters);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Client.inl"
#endif

#endif // } !defined(CLIENT_HXX__INCLUDED)
