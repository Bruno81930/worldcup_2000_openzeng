// PlayerClient.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "PlayerClient.hxx"

#ifndef NDEBUG
#  include "PlayerClient.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerClient

PlayerClient::~PlayerClient()
{
}

void PlayerClient::setWindow(Window* window)
{
	ASSERT(window);
	m_window = window;
}
void PlayerClient::removeWindow(Window* window)
{
	ASSERT(m_window == window);
	m_window = 0;
}
