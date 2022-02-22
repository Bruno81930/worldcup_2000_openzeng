// Premise.inl
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
// Premise

inline Premise::Premise(PremiseOwner* owner, bool add)
{
	m_added = false;
	m_owner = owner;
	if(add)
		addToOwner();
}

inline void Premise::addToOwner()
{
	m_added = true;
	m_owner->addPremise(this);
}

/////////////////////////////////////////////////////////////////////////////
// PremiseOwner

inline void PremiseOwner::addPremise(Premise* premise)
{
	m_premises.push_back(premise);
}
inline void PremiseOwner::removePremise(Premise* premise)
{
	m_premises.remove(premise);
}
inline void PremiseOwner::checkPremises()
{
#if NEVER_THROW
#  error
#else
	Premises::iterator it = m_premises.begin();
	for(; it != m_premises.end(); it++) {
		Premise* premise = *it;
		if(!premise->checkPremise())
			throw PremiseException();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// PremiseIf

inline PremiseIf::PremiseIf(PremiseOwner* owner) : Premise(owner, false)
{
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
