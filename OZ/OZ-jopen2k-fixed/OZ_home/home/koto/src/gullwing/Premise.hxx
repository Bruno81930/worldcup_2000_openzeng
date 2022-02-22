// Premise.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PREMISE_HXX__INCLUDED) // {
#define PREMISE_HXX__INCLUDED

class PremiseOwner;

/////////////////////////////////////////////////////////////////////////////
// PremiseException

/**
* 前提が崩れた。
*/
class PremiseException
{
};

/////////////////////////////////////////////////////////////////////////////
// Premise

/**
* 前提。
*/
class Premise
{
private:
	
protected:
	bool m_added;
	PremiseOwner* m_owner;
	/** m_owner へ登録する */
	void addToOwner();
	
public:
	/*dummy*/
	/** （登録されていれば、m_owner から削除する。）*/
	virtual ~Premise();
	/**
	* 構築する。
	* @param	owner
	*				この前提を管理するオブジェクト。
	* @param	add
	*				構築と同時に、owner へ登録する？
	*/
	Premise(PremiseOwner* owner, bool add=true);
	
	/**
	* 前提が崩れた場合 false、成り立っている場合 true を返す。
	*/
	virtual bool checkPremise() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// PremiseOwner

/**
* 前提を管理するオブジェクト。
*/
class PremiseOwner
{
private:
	typedef std::list<Premise*> Premises;
	Premises m_premises;
public:
	/** 前提を追加する */
	void addPremise(Premise* premise);
	/** 前提を削除する */
	void removePremise(Premise* premise);
	/**
	* 登録されている前提をチェックする。１つでも崩れた前提がある場合、        
    * PremiseException を throw する。
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
