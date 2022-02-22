// Sample.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SAMPLE_HXX__INCLUDED) // {
#define SAMPLE_HXX__INCLUDED

#include "../Monobrain.hxx"
#include "../White.hxx"
#include "../PlayerClient.hxx"
#include "../Random.hxx"

/////////////////////////////////////////////////////////////////////////////
// SamplePremise

class Sample;
class SamplePremise : public Premise
{
protected:
	Sample* m_brain;
public:
	SamplePremise(Sample* brain);
};

/////////////////////////////////////////////////////////////////////////////
// Sample

class Sample : public Monobrain<White>
{
private:
	Sample(const Sample&);
	Sample& operator= (const Sample&);
	bool operator== (const Sample&) const;

protected:
	Random m_random;

public:
	virtual ~Sample();
	Sample();

	virtual void go();
	virtual void act();
	
	void movable();
	void normal();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Sample.inl"
#endif

#endif // } !defined(SAMPLE_HXX__INCLUDED)
