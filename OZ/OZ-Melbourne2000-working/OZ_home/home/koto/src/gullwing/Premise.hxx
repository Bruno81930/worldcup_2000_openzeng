// Premise.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PREMISE_HXX__INCLUDED) // {
#define PREMISE_HXX__INCLUDED

class PremiseOwner;

/////////////////////////////////////////////////////////////////////////////
// PremiseException

/**
* $BA0Ds$,Jx$l$?!#(B
*/
class PremiseException
{
};

/////////////////////////////////////////////////////////////////////////////
// Premise

/**
* $BA0Ds!#(B
*/
class Premise
{
private:
	
protected:
	bool m_added;
	PremiseOwner* m_owner;
	/** m_owner $B$XEPO?$9$k(B */
	void addToOwner();
	
public:
	/*dummy*/
	/** $B!JEPO?$5$l$F$$$l$P!"(Bm_owner $B$+$i:o=|$9$k!#!K(B*/
	virtual ~Premise();
	/**
	* $B9=C[$9$k!#(B
	* @param	owner
	*				$B$3$NA0Ds$r4IM}$9$k%*%V%8%'%/%H!#(B
	* @param	add
	*				$B9=C[$HF1;~$K!"(Bowner $B$XEPO?$9$k!)(B
	*/
	Premise(PremiseOwner* owner, bool add=true);
	
	/**
	* $BA0Ds$,Jx$l$?>l9g(B false$B!"@.$jN)$C$F$$$k>l9g(B true $B$rJV$9!#(B
	*/
	virtual bool checkPremise() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// PremiseOwner

/**
* $BA0Ds$r4IM}$9$k%*%V%8%'%/%H!#(B
*/
class PremiseOwner
{
private:
	typedef std::list<Premise*> Premises;
	Premises m_premises;
public:
	/** $BA0Ds$rDI2C$9$k(B */
	void addPremise(Premise* premise);
	/** $BA0Ds$r:o=|$9$k(B */
	void removePremise(Premise* premise);
	/**
	* $BEPO?$5$l$F$$$kA0Ds$r%A%'%C%/$9$k!##1$D$G$bJx$l$?A0Ds$,$"$k>l9g!"(B        
    * PremiseException $B$r(B throw $B$9$k!#(B
	*/
	void checkPremises();
};

/////////////////////////////////////////////////////////////////////////////
// PremiseIf

class PremiseIf : public Premise
{
public:
	PremiseIf(PremiseOwner* owner);
	virtual bool validThenAdd() = 0;
};

#define PIF(promise) { promise p(this); if(p.validThenAdd())
#define PFI }

#if 0
void SomeBrain::test() {
	class Check;
	PIF(Check) {
		code...;
		cpde...;
	} PFI
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Premise.inl"
#endif

#endif // } !defined(PREMISE_HXX__INCLUDED)
