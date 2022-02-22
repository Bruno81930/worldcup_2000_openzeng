// Blue.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BLUE_HXX__INCLUDED) // {
#define BLUE_HXX__INCLUDED

#include "../Monoworld.hxx"
#include "../White.hxx"

/////////////////////////////////////////////////////////////////////////////
// Blue

class Blue : public Monoworld<WhiteBall, WhitePlayer, WhiteSelf>
{
private:
	Blue(const Blue&);
	Blue& operator= (const Blue&);
	bool operator== (const Blue&) const;

protected:
	FILE *m_fp;
	virtual void initialize(PlayMode playMode, int uniformNumber, bool goalie);
	virtual void received(const PlayerSenseBody& sbi);
	virtual void received(const PlayerSee& si);
	virtual void updateSelf(const PlayerSee& si);
	virtual bool estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result);

public:
	virtual ~Blue();
	Blue();

	double parameter;
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Blue.inl"
#endif

#endif // } !defined(BLUE_HXX__INCLUDED)
