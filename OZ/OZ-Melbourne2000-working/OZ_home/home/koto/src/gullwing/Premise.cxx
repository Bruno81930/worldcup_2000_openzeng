// Premise.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Premise.hxx"

#ifndef NDEBUG
#  include "Premise.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Premise

Premise::~Premise()
{
	if(m_added)
		m_owner->removePremise(this);
}

bool Premise::checkPremise()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// PremiseIf

bool PremiseIf::validThenAdd()
{
	bool result = checkPremise();
	if(result)
		addToOwner();
	return result;
}
